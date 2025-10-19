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
    // ��������ȼ���
    map<char, int> priority = {
        {'+', 1}, {'-', 1},
        {'*', 2}, {'/', 2}, {'%', 2},
        {'^', 3},
        {'(', 0}, {')', 0}
    };

    // ֧�ֵĺ����б�
    map<string, int> functions = {
        {"sin", 1}, {"cos", 1}, {"tan", 1},
        {"log", 1}, {"ln", 1}, {"sqrt", 1},
        {"abs", 1}
    };

    // ����ַ��Ƿ�Ϊ�����
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || 
               c == '%' || c == '^' || c == '(' || c == ')';
    }

    // ����ַ����Ƿ�Ϊ������
    bool isFunction(const string& str) {
        return functions.find(str) != functions.end();
    }

    // ִ�л�������
    double calculate(double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw runtime_error("�������");
                return a / b;
            case '%': 
                if (static_cast<int>(b) == 0) throw runtime_error("ģ�����");
                return fmod(a, b);
            case '^': return pow(a, b);
            default: throw runtime_error("δ֪�����");
        }
    }

    // ִ�к�������
    double calculateFunction(const string& func, double x) {
        if (func == "sin") return sin(x);
        if (func == "cos") return cos(x);
        if (func == "tan") return tan(x);
        if (func == "log") {
            if (x <= 0) throw runtime_error("log�����������0");
            return log10(x);
        }
        if (func == "ln") {
            if (x <= 0) throw runtime_error("ln�����������0");
            return log(x);
        }
        if (func == "sqrt") {
            if (x < 0) throw runtime_error("sqrt��������Ϊ��");
            return sqrt(x);
        }
        if (func == "abs") return fabs(x);
        throw runtime_error("δ֪����");
    }

    // ��׺���ʽת��׺���ʽ
    string infixToPostfix(const string& infix) {
        Stack<char> opStack;
        stringstream postfix;
        string token;
        
        for (size_t i = 0; i < infix.length(); ++i) {
            char c = infix[i];
            
            // �����ո�
            if (isspace(c)) continue;
            
            // �������֣�����С����͸�����
            if (isdigit(c) || c == '.' || (c == '-' && (i == 0 || infix[i-1] == '('))) {
                // ��ȡ��������
                while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.' || 
                       (infix[i] == '-' && (i == 0 || infix[i-1] == '(')))) {
                    postfix << infix[i];
                    i++;
                }
                i--; // ����һ���ַ�
                postfix << " "; // ���ּ��ÿո�ָ�
                continue;
            }
            
            // ��������
            if (isalpha(c)) {
                string func;
                while (i < infix.length() && isalpha(infix[i])) {
                    func += infix[i];
                    i++;
                }
                i--; // ����һ���ַ�
                
                if (isFunction(func)) {
                    opStack.push('@'); // �������Ǳ�ʾ������ʼ
                    for (char fc : func) {
                        opStack.push(fc);
                    }
                    opStack.push('#'); // �������Ǳ�ʾ��������
                } else {
                    throw runtime_error("δ֪����: " + func);
                }
                continue;
            }
            
            // ����������
            if (c == '(') {
                opStack.push(c);
            }
            // ����������
            else if (c == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    if (opStack.top() == '@') {
                        // ������
                        opStack.pop(); // ���� '@'
                        string func;
                        while (!opStack.empty() && opStack.top() != '#') {
                            func = string(1, opStack.top()) + func;
                            opStack.pop();
                        }
                        if (!opStack.empty() && opStack.top() == '#') {
                            opStack.pop(); // ���� '#'
                        }
                        postfix << func << " ";
                    } else {
                        postfix << opStack.top() << " ";
                        opStack.pop();
                    }
                }
                if (opStack.empty()) {
                    throw runtime_error("���Ų�ƥ��");
                }
                opStack.pop(); // ����������
            }
            // ���������
            else if (isOperator(c)) {
                while (!opStack.empty() && 
                       priority[opStack.top()] >= priority[c] && 
                       opStack.top() != '(') {
                    if (opStack.top() == '@') {
                        // ������
                        opStack.pop(); // ���� '@'
                        string func;
                        while (!opStack.empty() && opStack.top() != '#') {
                            func = string(1, opStack.top()) + func;
                            opStack.pop();
                        }
                        if (!opStack.empty() && opStack.top() == '#') {
                            opStack.pop(); // ���� '#'
                        }
                        postfix << func << " ";
                    } else {
                        postfix << opStack.top() << " ";
                        opStack.pop();
                    }
                }
                opStack.push(c);
            } else {
                throw runtime_error("��Ч�ַ�: " + string(1, c));
            }
        }
        
        // ����ջ��ʣ�������
        while (!opStack.empty()) {
            if (opStack.top() == '@') {
                // ������
                opStack.pop(); // ���� '@'
                string func;
                while (!opStack.empty() && opStack.top() != '#') {
                    func = string(1, opStack.top()) + func;
                    opStack.pop();
                }
                if (!opStack.empty() && opStack.top() == '#') {
                    opStack.pop(); // ���� '#'
                }
                postfix << func << " ";
            } else if (opStack.top() == '(') {
                throw runtime_error("���Ų�ƥ��");
            } else {
                postfix << opStack.top() << " ";
                opStack.pop();
            }
        }
        
        return postfix.str();
    }

    // �����׺���ʽ
    double evaluatePostfix(const string& postfix) {
        Stack<double> numStack;
        stringstream ss(postfix);
        string token;
        
        while (ss >> token) {
            // ���������
            if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
                numStack.push(stod(token));
            }
            // ����Ǻ���
            else if (isFunction(token)) {
                if (numStack.empty()) throw runtime_error("���ʽ��Ч");
                double x = numStack.pop();
                double result = calculateFunction(token, x);
                numStack.push(result);
            }
            // ����������
            else if (isOperator(token[0])) {
                if (numStack.size() < 2) throw runtime_error("���ʽ��Ч");
                double b = numStack.pop();
                double a = numStack.pop();
                double result = calculate(a, b, token[0]);
                numStack.push(result);
            } else {
                throw runtime_error("��Чtoken: " + token);
            }
        }
        
        if (numStack.size() != 1) {
            throw runtime_error("���ʽ��Ч");
        }
        
        return numStack.pop();
    }

public:
    // ������ʽ��������
    double calculate(const string& expression) {
        try {
            // Ԥ����ȥ���ո񣬴���һЩ�������
            string processed = expression;
            // ������������Ӹ����Ԥ�����߼�
            
            string postfix = infixToPostfix(processed);
            cout << "��׺���ʽ: " << postfix << endl;
            return evaluatePostfix(postfix);
        } catch (const exception& e) {
            throw runtime_error("�������: " + string(e.what()));
        }
    }
};

// ���Ժ���
void runTests() {
    StringCalculator calc;
    
    // ��������
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
    
    cout << "=== �ַ������������� ===" << endl;
    
    for (const auto& testCase : testCases) {
        try {
            double result = calc.calculate(testCase.first);
            cout << testCase.first << " = " << result;
            if (fabs(result - testCase.second) < 1e-6) {
                cout << " ?" << endl;
            } else {
                cout << " ? (����: " << testCase.second << ")" << endl;
            }
        } catch (const exception& e) {
            cout << testCase.first << " = ����: " << e.what() << endl;
        }
    }
}

// ����ʽ������
void interactiveCalculator() {
    StringCalculator calc;
    string input;
    
    cout << "\n=== ����ʽ������ ===" << endl;
    cout << "֧�ֵĲ���: +, -, *, /, %, ^, (), sin, cos, tan, log, ln, sqrt, abs" << endl;
    cout << "���� 'quit' �˳�" << endl;
    
    while (true) {
        cout << "\n��������ʽ: ";
        getline(cin, input);
        
        if (input == "quit" || input == "exit") {
            break;
        }
        
        if (input.empty()) {
            continue;
        }
        
        try {
            double result = calc.calculate(input);
            cout << "���: " << result << endl;
        } catch (const exception& e) {
            cout << "����: " << e.what() << endl;
        }
    }
}

int main() {
    // ����Ԥ�����
    runTests();
    
    // ��������ʽ������
    interactiveCalculator();
    
    return 0;
}
