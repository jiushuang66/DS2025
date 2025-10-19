#include "Stack.h"
#include "Vector.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>

using namespace std;

// ����1: ʹ�õ���ջ - ʱ�临�Ӷ� O(n)
int largestRectangleArea_monotonic_stack(const Vector<int>& heights) {
    int n = heights.size();
    if (n == 0) return 0;
    if (n == 1) return heights[0];
    
    Stack<int> stack; // �洢�����ĵ���ջ
    int maxArea = 0;
    
    for (int i = 0; i < n; ++i) {
        // ����ǰ�߶�С��ջ���߶�ʱ��������ջ��Ϊ�ߵľ������
        while (!stack.empty() && heights[i] < heights[stack.top()]) {
            int height = heights[stack.pop()];
            int width = stack.empty() ? i : i - stack.top() - 1;
            maxArea = max(maxArea, height * width);
        }
        stack.push(i);
    }
    
    // ����ջ��ʣ���Ԫ��
    while (!stack.empty()) {
        int height = heights[stack.pop()];
        int width = stack.empty() ? n : n - stack.top() - 1;
        maxArea = max(maxArea, height * width);
    }
    
    return maxArea;
}

// ����2: �����ⷨ - ʱ�临�Ӷ� O(n2)��������֤��ȷ��
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

// ���������������
Vector<int> generateRandomHeights(int minLength, int maxLength, int maxHeight) {
    int length = minLength + rand() % (maxLength - minLength + 1);
    Vector<int> heights;
    
    for (int i = 0; i < length; ++i) {
        heights.push_back(rand() % (maxHeight + 1));
    }
    
    return heights;
}

// ��ӡ���Խ��
void printTestResult(const Vector<int>& heights, int result, int expected, bool passed) {
    cout << "�߶�����: [";
    for (int i = 0; i < heights.size(); ++i) {
        cout << heights[i];
        if (i != heights.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "������: " << result;
    if (expected != -1) {
        cout << " (����: " << expected << ")";
    }
    cout << " - " << (passed ? "? ͨ��" : "? ʧ��") << endl;
    cout << "----------------------------------------" << endl;
}

// ���в�������
void runTestCases() {
    cout << "=== ��״ͼ������������� ===" << endl;
    
    // ������������
    Vector<Vector<int>> testCases;
    Vector<int> expectedResults;
    
    // ��������1: [2,1,5,6,2,3]
    Vector<int> case1;
    case1.push_back(2); case1.push_back(1); case1.push_back(5);
    case1.push_back(6); case1.push_back(2); case1.push_back(3);
    testCases.push_back(case1);
    expectedResults.push_back(10);
    
    // ��������2: [2,4]
    Vector<int> case2;
    case2.push_back(2); case2.push_back(4);
    testCases.push_back(case2);
    expectedResults.push_back(4);
    
    // ��������3: [1]
    Vector<int> case3;
    case3.push_back(1);
    testCases.push_back(case3);
    expectedResults.push_back(1);
    
    // ��������4: [1,1,1,1]
    Vector<int> case4;
    case4.push_back(1); case4.push_back(1); case4.push_back(1); case4.push_back(1);
    testCases.push_back(case4);
    expectedResults.push_back(4);
    
    // ��������5: [1,2,3,4,5]
    Vector<int> case5;
    case5.push_back(1); case5.push_back(2); case5.push_back(3); case5.push_back(4); case5.push_back(5);
    testCases.push_back(case5);
    expectedResults.push_back(9);
    
    // ��������6: [5,4,3,2,1]
    Vector<int> case6;
    case6.push_back(5); case6.push_back(4); case6.push_back(3); case6.push_back(2); case6.push_back(1);
    testCases.push_back(case6);
    expectedResults.push_back(9);
    
    // ��������7: [2,1,2]
    Vector<int> case7;
    case7.push_back(2); case7.push_back(1); case7.push_back(2);
    testCases.push_back(case7);
    expectedResults.push_back(3);
    
    // ��������8: [0,0,0]
    Vector<int> case8;
    case8.push_back(0); case8.push_back(0); case8.push_back(0);
    testCases.push_back(case8);
    expectedResults.push_back(0);
    
    // ��������9: [3]
    Vector<int> case9;
    case9.push_back(3);
    testCases.push_back(case9);
    expectedResults.push_back(3);
    
    // ��������10: [1,3,2,1,2]
    Vector<int> case10;
    case10.push_back(1); case10.push_back(3); case10.push_back(2); case10.push_back(1); case10.push_back(2);
    testCases.push_back(case10);
    expectedResults.push_back(5);
    
    for (int i = 0; i < testCases.size(); ++i) {
        int result = largestRectangleArea_monotonic_stack(testCases[i]);
        int bruteResult = largestRectangleArea_brute_force(testCases[i]);
        bool passed = (result == expectedResults[i]) && (result == bruteResult);
        
        printTestResult(testCases[i], result, expectedResults[i], passed);
        
        // ��֤����ջ�ͱ����ⷨ���һ��
        if (result != bruteResult) {
            cout << "����: ����ջ�ⷨ(" << result << ")�뱩���ⷨ(" << bruteResult << ")�����һ��!" << endl;
        }
    }
}

// �����������
void runRandomTests(int numTests = 10) {
    cout << "\n=== ������� (" << numTests << " ������) ===" << endl;
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    for (int test = 1; test <= numTests; ++test) {
        cout << "\n���� " << test << ":" << endl;
        
        // ����������ݣ�С��ģ������֤��
        Vector<int> heights = generateRandomHeights(1, 20, 50);
        
        // ����������
        clock_t start = clock();
        int result = largestRectangleArea_monotonic_stack(heights);
        clock_t end = clock();
        double timeUsed = double(end - start) / CLOCKS_PER_SEC * 1000;
        
        // ��С��ģ�����ñ����ⷨ��֤��ȷ��
        int bruteResult = largestRectangleArea_brute_force(heights);
        
        cout << "���ݹ�ģ: " << heights.size() << " ��Ԫ��" << endl;
        cout << "������: " << result;
        cout << " (�����ⷨ: " << bruteResult << ")";
        if (result == bruteResult) {
            cout << " ? ��֤ͨ��";
        } else {
            cout << " ? ��֤ʧ��";
        }
        cout << endl;
        cout << "����ʱ��: " << timeUsed << " ms" << endl;
        
        // ��ӡǰ10��Ԫ����Ϊ����
        if (heights.size() > 10) {
            cout << "ǰ10���߶�: [";
            for (int i = 0; i < 10 && i < heights.size(); ++i) {
                cout << heights[i];
                if (i != 9 && i != heights.size() - 1) cout << ", ";
            }
            cout << ", ...]" << endl;
        } else {
            cout << "�߶�����: [";
            for (int i = 0; i < heights.size(); ++i) {
                cout << heights[i];
                if (i != heights.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
        cout << "----------------------------------------" << endl;
    }
}

// ���ܲ��ԣ����ģ����
void performanceTest() {
    cout << "\n=== ���ܲ��� (���ģ����) ===" << endl;
    
    Vector<int> largeHeights = generateRandomHeights(100000, 100000, 10000);
    cout << "���ݹ�ģ: " << largeHeights.size() << " ��Ԫ��" << endl;
    
    clock_t start = clock();
    int result = largestRectangleArea_monotonic_stack(largeHeights);
    clock_t end = clock();
    double timeUsed = double(end - start) / CLOCKS_PER_SEC * 1000;
    
    cout << "������: " << result << endl;
    cout << "����ʱ��: " << timeUsed << " ms" << endl;
    
    // ��֤С��������ȷ��
    Vector<int> sample;
    for (int i = 0; i < 10 && i < largeHeights.size(); ++i) {
        sample.push_back(largeHeights[i]);
    }
    int sampleResult1 = largestRectangleArea_monotonic_stack(sample);
    int sampleResult2 = largestRectangleArea_brute_force(sample);
    cout << "ǰ10��Ԫ�ص�������֤: ����ջ=" << sampleResult1 << ", ����=" << sampleResult2;
    cout << (sampleResult1 == sampleResult2 ? " ?" : " ?") << endl;
}

int main() {
    // ���й̶���������
    runTestCases();
    
    // �����������
    runRandomTests(10);
    
    // �������ܲ���
    performanceTest();
    
    return 0;
}
