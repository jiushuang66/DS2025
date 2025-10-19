#include "Stack.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <map>

using namespace std;

class StringCalculator {
private:
    // 运算符优先级表
    map<char, int> priority = {
        {'+', 1}, {'-', 1},
        {'*', 2}, {'/', 2}, {'%', 2},
        {'^', 3},
        {'(', 0}, {')', 0}
    };

    // 支持的函数列表
    map<string, int> functions = {
        {"sin", 1}, {"cos", 1}, {"tan", 1},
        {"log", 1}, {"ln", 1}, {"sqrt", 1},
        {"abs", 1}
    };

    // 检查字符是否为运算符
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || 
               c == '%' || c == '^' || c == '(' || c == ')';
    }

    // 检查字符串是否为函数名
    bool isFunction(const string& str) {
        return functions.find(str) != functions.end();
    }

    // 执行基本运算
    double calculate(double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw runtime_error("除零错误");
                return a / b;
            case '%': 
                if (static_cast<int>(b) == 0) throw runtime_error("模零错误");
                return fmod(a, b);
            case '^': return pow(a, b);
            default: throw runtime_error("未知运算符");
        }
    }

    // 执行函数运算
    double calculateFunction(const string& func, double x) {
        if (func == "sin") return sin(x);
        if (func == "cos") return cos(x);
        if (func == "tan") return tan(x);
        if (func == "log") {
            if (x <= 0) throw runtime_error("log参数必须大于0");
            return log10(x);
        }
        if (func == "ln") {
            if (x <= 0) throw runtime_error("ln参数必须大于0");
            return log(x);
        }
        if (func == "sqrt") {
            if (x < 0) throw runtime_error("sqrt参数不能为负");
            return sqrt(x);
        }
        if (func == "abs") return fabs(x);
        throw runtime_error("未知函数");
    }

    // 中缀表达式转后缀表达式
    string infixToPostfix(const string& infix) {
        Stack<char> opStack;
        stringstream postfix;
        string token;
        
        for (size_t i = 0; i < infix.length(); ++i) {
            char c = infix[i];
            
            // 跳过空格
            if (isspace(c)) continue;
            
            // 处理数字（包括小数点和负数）
            if (isdigit(c) || c == '.' || (c == '-' && (i == 0 || infix[i-1] == '('))) {
                // 读取完整数字
                while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.' || 
                       (infix[i] == '-' && (i == 0 || infix[i-1] == '(')))) {
                    postfix << infix[i];
                    i++;
                }
                i--; // 回退一个字符
                postfix << " "; // 数字间用空格分隔
                continue;
            }
            
            // 处理函数名
            if (isalpha(c)) {
                string func;
                while (i < infix.length() && isalpha(infix[i])) {
                    func += infix[i];
                    i++;
                }
                i--; // 回退一个字符
                
                if (isFunction(func)) {
                    opStack.push('@'); // 用特殊标记表示函数开始
                    for (char fc : func) {
                        opStack.push(fc);
                    }
                    opStack.push('#'); // 用特殊标记表示函数结束
                } else {
                    throw runtime_error("未知函数: " + func);
                }
                continue;
            }
            
            // 处理左括号
            if (c == '(') {
                opStack.push(c);
            }
            // 处理右括号
            else if (c == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    if (opStack.top() == '@') {
                        // 处理函数
                        opStack.pop(); // 弹出 '@'
                        string func;
                        while (!opStack.empty() && opStack.top() != '#') {
                            func = string(1, opStack.top()) + func;
                            opStack.pop();
                        }
                        if (!opStack.empty() && opStack.top() == '#') {
                            opStack.pop(); // 弹出 '#'
                        }
                        postfix << func << " ";
                    } else {
                        postfix << opStack.top() << " ";
                        opStack.pop();
                    }
                }
                if (opStack.empty()) {
                    throw runtime_error("括号不匹配");
                }
                opStack.pop(); // 弹出左括号
            }
            // 处理运算符
            else if (isOperator(c)) {
                while (!opStack.empty() && 
                       priority[opStack.top()] >= priority[c] && 
                       opStack.top() != '(') {
                    if (opStack.top() == '@') {
                        // 处理函数
                        opStack.pop(); // 弹出 '@'
                        string func;
                        while (!opStack.empty() && opStack.top() != '#') {
                            func = string(1, opStack.top()) + func;
                            opStack.pop();
                        }
                        if (!opStack.empty() && opStack.top() == '#') {
                            opStack.pop(); // 弹出 '#'
                        }
                        postfix << func << " ";
                    } else {
                        postfix << opStack.top() << " ";
                        opStack.pop();
                    }
                }
                opStack.push(c);
            } else {
                throw runtime_error("无效字符: " + string(1, c));
            }
        }
        
        // 弹出栈中剩余运算符
        while (!opStack.empty()) {
            if (opStack.top() == '@') {
                // 处理函数
                opStack.pop(); // 弹出 '@'
                string func;
                while (!opStack.empty() && opStack.top() != '#') {
                    func = string(1, opStack.top()) + func;
                    opStack.pop();
                }
                if (!opStack.empty() && opStack.top() == '#') {
                    opStack.pop(); // 弹出 '#'
                }
                postfix << func << " ";
            } else if (opStack.top() == '(') {
                throw runtime_error("括号不匹配");
            } else {
                postfix << opStack.top() << " ";
                opStack.pop();
            }
        }
        
        return postfix.str();
    }

    // 计算后缀表达式
    double evaluatePostfix(const string& postfix) {
        Stack<double> numStack;
        stringstream ss(postfix);
        string token;
        
        while (ss >> token) {
            // 如果是数字
            if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
                numStack.push(stod(token));
            }
            // 如果是函数
            else if (isFunction(token)) {
                if (numStack.empty()) throw runtime_error("表达式无效");
                double x = numStack.pop();
                double result = calculateFunction(token, x);
                numStack.push(result);
            }
            // 如果是运算符
            else if (isOperator(token[0])) {
                if (numStack.size() < 2) throw runtime_error("表达式无效");
                double b = numStack.pop();
                double a = numStack.pop();
                double result = calculate(a, b, token[0]);
                numStack.push(result);
            } else {
                throw runtime_error("无效token: " + token);
            }
        }
        
        if (numStack.size() != 1) {
            throw runtime_error("表达式无效");
        }
        
        return numStack.pop();
    }

public:
    // 计算表达式的主函数
    double calculate(const string& expression) {
        try {
            // 预处理：去除空格，处理一些特殊情况
            string processed = expression;
            // 可以在这里添加更多的预处理逻辑
            
            string postfix = infixToPostfix(processed);
            cout << "后缀表达式: " << postfix << endl;
            return evaluatePostfix(postfix);
        } catch (const exception& e) {
            throw runtime_error("计算错误: " + string(e.what()));
        }
    }
};

// 测试函数
void runTests() {
    StringCalculator calc;
    
    // 测试用例
    vector<pair<string, double>> testCases = {
        {"2 + 3", 5},
        {"10 - 4", 6},
        {"3 * 5", 15},
        {"20 / 4", 5},
        {"(2 + 3) * 4", 20},
        {"2 + 3 * 4", 14},
        {"10 % 3", 1},
        {"2 ^ 3", 8},
        {"sin(0)", 0},
        {"cos(0)", 1},
        {"sqrt(16)", 4},
        {"abs(-5)", 5},
        {"log(100)", 2}
    };
    
    cout << "=== 字符串计算器测试 ===" << endl;
    
    for (const auto& testCase : testCases) {
        try {
            double result = calc.calculate(testCase.first);
            cout << testCase.first << " = " << result;
            if (fabs(result - testCase.second) < 1e-6) {
                cout << " ?" << endl;
            } else {
                cout << " ? (期望: " << testCase.second << ")" << endl;
            }
        } catch (const exception& e) {
            cout << testCase.first << " = 错误: " << e.what() << endl;
        }
    }
}

// 交互式计算器
void interactiveCalculator() {
    StringCalculator calc;
    string input;
    
    cout << "\n=== 交互式计算器 ===" << endl;
    cout << "支持的操作: +, -, *, /, %, ^, (), sin, cos, tan, log, ln, sqrt, abs" << endl;
    cout << "输入 'quit' 退出" << endl;
    
    while (true) {
        cout << "\n请输入表达式: ";
        getline(cin, input);
        
        if (input == "quit" || input == "exit") {
            break;
        }
        
        if (input.empty()) {
            continue;
        }
        
        try {
            double result = calc.calculate(input);
            cout << "结果: " << result << endl;
        } catch (const exception& e) {
            cout << "错误: " << e.what() << endl;
        }
    }
}

int main() {
    // 运行预设测试
    runTests();
    
    // 启动交互式计算器
    interactiveCalculator();
    
    return 0;
}
