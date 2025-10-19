#include "Stack.h"
#include "Vector.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>

using namespace std;

// 方法1: 使用单调栈 - 时间复杂度 O(n)
int largestRectangleArea_monotonic_stack(const Vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;
    if (n == 1) return heights[0];
    
    Stack<int> stack; // 存储索引的单调栈
    int maxArea = 0;
    
    for (int i = 0; i < n; ++i) {
        // 当当前高度小于栈顶高度时，计算以栈顶为高的矩形面积
        while (!stack.empty() && heights[i] < heights[stack.top()]) {
            int height = heights[stack.pop()];
            int width = stack.empty() ? i : i - stack.top() - 1;
            maxArea = max(maxArea, height * width);
        }
        stack.push(i);
    }
    
    // 处理栈中剩余的元素
    while (!stack.empty()) {
        int height = heights[stack.pop()];
        int width = stack.empty() ? n : n - stack.top() - 1;
        maxArea = max(maxArea, height * width);
    }
    
    return maxArea;
}

// 方法2: 暴力解法 - 时间复杂度 O(n2)，用于验证正确性
int largestRectangleArea_brute_force(const Vector<int>& heights) {
    int n = heights.size();
    int maxArea = 0;
    
    for (int i = 0; i < n; ++i) {
        int minHeight = INT_MAX;
        for (int j = i; j < n; ++j) {
            minHeight = min(minHeight, heights[j]);
            int area = minHeight * (j - i + 1);
            maxArea = max(maxArea, area);
        }
    }
    
    return maxArea;
}

// 生成随机测试数据
Vector<int> generateRandomHeights(int minLength, int maxLength, int maxHeight) {
    int length = minLength + rand() % (maxLength - minLength + 1);
    Vector<int> heights;
    
    for (int i = 0; i < length; ++i) {
        heights.push_back(rand() % (maxHeight + 1));
    }
    
    return heights;
}

// 打印测试结果
void printTestResult(const Vector<int>& heights, int result, int expected, bool passed) {
    cout << "高度数组: [";
    for (int i = 0; i < heights.size(); ++i) {
        cout << heights[i];
        if (i != heights.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "最大面积: " << result;
    if (expected != -1) {
        cout << " (期望: " << expected << ")";
    }
    cout << " - " << (passed ? "? 通过" : "? 失败") << endl;
    cout << "----------------------------------------" << endl;
}

// 运行测试用例
void runTestCases() {
    cout << "=== 柱状图最大矩形面积测试 ===" << endl;
    
    // 创建测试用例
    Vector<Vector<int>> testCases;
    Vector<int> expectedResults;
    
    // 测试用例1: [2,1,5,6,2,3]
    Vector<int> case1;
    case1.push_back(2); case1.push_back(1); case1.push_back(5);
    case1.push_back(6); case1.push_back(2); case1.push_back(3);
    testCases.push_back(case1);
    expectedResults.push_back(10);
    
    // 测试用例2: [2,4]
    Vector<int> case2;
    case2.push_back(2); case2.push_back(4);
    testCases.push_back(case2);
    expectedResults.push_back(4);
    
    // 测试用例3: [1]
    Vector<int> case3;
    case3.push_back(1);
    testCases.push_back(case3);
    expectedResults.push_back(1);
    
    // 测试用例4: [1,1,1,1]
    Vector<int> case4;
    case4.push_back(1); case4.push_back(1); case4.push_back(1); case4.push_back(1);
    testCases.push_back(case4);
    expectedResults.push_back(4);
    
    // 测试用例5: [1,2,3,4,5]
    Vector<int> case5;
    case5.push_back(1); case5.push_back(2); case5.push_back(3); case5.push_back(4); case5.push_back(5);
    testCases.push_back(case5);
    expectedResults.push_back(9);
    
    // 测试用例6: [5,4,3,2,1]
    Vector<int> case6;
    case6.push_back(5); case6.push_back(4); case6.push_back(3); case6.push_back(2); case6.push_back(1);
    testCases.push_back(case6);
    expectedResults.push_back(9);
    
    // 测试用例7: [2,1,2]
    Vector<int> case7;
    case7.push_back(2); case7.push_back(1); case7.push_back(2);
    testCases.push_back(case7);
    expectedResults.push_back(3);
    
    // 测试用例8: [0,0,0]
    Vector<int> case8;
    case8.push_back(0); case8.push_back(0); case8.push_back(0);
    testCases.push_back(case8);
    expectedResults.push_back(0);
    
    // 测试用例9: [3]
    Vector<int> case9;
    case9.push_back(3);
    testCases.push_back(case9);
    expectedResults.push_back(3);
    
    // 测试用例10: [1,3,2,1,2]
    Vector<int> case10;
    case10.push_back(1); case10.push_back(3); case10.push_back(2); case10.push_back(1); case10.push_back(2);
    testCases.push_back(case10);
    expectedResults.push_back(5);
    
    for (int i = 0; i < testCases.size(); ++i) {
        int result = largestRectangleArea_monotonic_stack(testCases[i]);
        int bruteResult = largestRectangleArea_brute_force(testCases[i]);
        bool passed = (result == expectedResults[i]) && (result == bruteResult);
        
        printTestResult(testCases[i], result, expectedResults[i], passed);
        
        // 验证单调栈和暴力解法结果一致
        if (result != bruteResult) {
            cout << "警告: 单调栈解法(" << result << ")与暴力解法(" << bruteResult << ")结果不一致!" << endl;
        }
    }
}

// 运行随机测试
void runRandomTests(int numTests = 10) {
    cout << "\n=== 随机测试 (" << numTests << " 组数据) ===" << endl;
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    for (int test = 1; test <= numTests; ++test) {
        cout << "\n测试 " << test << ":" << endl;
        
        // 生成随机数据（小规模用于验证）
        Vector<int> heights = generateRandomHeights(1, 20, 50);
        
        // 计算最大面积
        clock_t start = clock();
        int result = largestRectangleArea_monotonic_stack(heights);
        clock_t end = clock();
        double timeUsed = double(end - start) / CLOCKS_PER_SEC * 1000;
        
        // 对小规模数据用暴力解法验证正确性
        int bruteResult = largestRectangleArea_brute_force(heights);
        
        cout << "数据规模: " << heights.size() << " 个元素" << endl;
        cout << "最大面积: " << result;
        cout << " (暴力解法: " << bruteResult << ")";
        if (result == bruteResult) {
            cout << " ? 验证通过";
        } else {
            cout << " ? 验证失败";
        }
        cout << endl;
        cout << "计算时间: " << timeUsed << " ms" << endl;
        
        // 打印前10个元素作为样本
        if (heights.size() > 10) {
            cout << "前10个高度: [";
            for (int i = 0; i < 10 && i < heights.size(); ++i) {
                cout << heights[i];
                if (i != 9 && i != heights.size() - 1) cout << ", ";
            }
            cout << ", ...]" << endl;
        } else {
            cout << "高度数组: [";
            for (int i = 0; i < heights.size(); ++i) {
                cout << heights[i];
                if (i != heights.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
        cout << "----------------------------------------" << endl;
    }
}

// 性能测试：大规模数据
void performanceTest() {
    cout << "\n=== 性能测试 (大规模数据) ===" << endl;
    
    Vector<int> largeHeights = generateRandomHeights(100000, 100000, 10000);
    cout << "数据规模: " << largeHeights.size() << " 个元素" << endl;
    
    clock_t start = clock();
    int result = largestRectangleArea_monotonic_stack(largeHeights);
    clock_t end = clock();
    double timeUsed = double(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "最大面积: " << result << endl;
    cout << "计算时间: " << timeUsed << " ms" << endl;
    
    // 验证小样本的正确性
    Vector<int> sample;
    for (int i = 0; i < 10 && i < largeHeights.size(); ++i) {
        sample.push_back(largeHeights[i]);
    }
    int sampleResult1 = largestRectangleArea_monotonic_stack(sample);
    int sampleResult2 = largestRectangleArea_brute_force(sample);
    cout << "前10个元素的样本验证: 单调栈=" << sampleResult1 << ", 暴力=" << sampleResult2;
    cout << (sampleResult1 == sampleResult2 ? " ?" : " ?") << endl;
}

int main() {
    // 运行固定测试用例
    runTestCases();
    
    // 运行随机测试
    runRandomTests(10);
    
    // 运行性能测试
    performanceTest();
    
    return 0;
}
