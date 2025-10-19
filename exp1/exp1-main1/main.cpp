#include "Complex.h"
#include "Vector.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

// 生成随机复数
template <typename T>
Complex<T> generateRandomComplex(T minVal, T maxVal) {
    T real = minVal + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX/(maxVal - minVal)));
    T imag = minVal + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX/(maxVal - minVal)));
    return Complex<T>(real, imag);
}

// 起泡排序
template <typename T>
void bubbleSort(Vector<Complex<T>>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            // 比较模长，模长相同时比较实部
            if (vec[j].mod() > vec[j+1].mod() || 
                (fabs(vec[j].mod() - vec[j+1].mod()) < 1e-6 && vec[j].getReal() > vec[j+1].getReal())) {
                swap(vec[j], vec[j+1]);
            }
        }
    }
}

// 区间查找：查找模介于[m1,m2)的所有元素
template <typename T>
Vector<Complex<T>> rangeSearch(const Vector<Complex<T>>& vec, double m1, double m2) {
    Vector<Complex<T>> result;
    for (int i = 0; i < vec.size(); ++i) {
        double mod = vec[i].mod();
        if (mod >= m1 && mod < m2) {
            result.push_back(vec[i]);
        }
    }
    return result;
}

// 向量唯一化（去除重复元素）
template <typename T>
void unique(Vector<Complex<T>>& vec) {
    if (vec.empty()) return;
    
    // 先排序
    vec.sort_merge();
    
    // 再去重
    int writeIndex = 0;
    for (int readIndex = 1; readIndex < vec.size(); ++readIndex) {
        if (!(vec[readIndex] == vec[writeIndex])) {
            vec[++writeIndex] = vec[readIndex];
        }
    }
    
    // 调整大小
    while (vec.size() > writeIndex + 1) {
        vec.pop_back();
    }
}

// 打印向量信息
template <typename T>
void printVectorInfo(const Vector<Complex<T>>& vec, const string& name) {
    cout << name << " (size: " << vec.size() << "): ";
    vec.print();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    const int NUM_COMPLEX = 20;  // 复数数量
    Vector<Complex<double>> complexVector;
    
    cout << "=== 复数向量操作测试 ===" << endl;
    
    // 1. 生成随机复数向量（有重复项）
    cout << "\n1. 生成随机复数向量:" << endl;
    for (int i = 0; i < NUM_COMPLEX; ++i) {
        complexVector.push_back(generateRandomComplex(-5.0, 5.0));
    }
    printVectorInfo(complexVector, "原始向量");
    
    // 2. 测试置乱操作
    cout << "\n2. 测试置乱操作:" << endl;
    for (int i = 0; i < complexVector.size(); ++i) {
        int j = rand() % complexVector.size();
        swap(complexVector[i], complexVector[j]);
    }
    printVectorInfo(complexVector, "置乱后向量");
    
    // 3. 测试查找操作
    cout << "\n3. 测试查找操作:" << endl;
    Complex<double> searchTarget(complexVector[0].getReal(), complexVector[0].getImag());
    int foundIndex = complexVector.find(searchTarget);
    if (foundIndex != -1) {
        cout << "找到元素 " << searchTarget << " 在位置 " << foundIndex << endl;
    } else {
        cout << "未找到元素 " << searchTarget << endl;
    }
    
    // 4. 测试插入操作
    cout << "\n4. 测试插入操作:" << endl;
    Complex<double> newComplex(99.0, 99.0);
    complexVector.insert(complexVector.size() / 2, newComplex);
    printVectorInfo(complexVector, "插入新元素后");
    
    // 5. 测试删除操作
    cout << "\n5. 测试删除操作:" << endl;
    Complex<double> removed = complexVector.erase(complexVector.size() / 2);
    cout << "删除的元素: " << removed << endl;
    printVectorInfo(complexVector, "删除元素后");
    
    // 6. 测试唯一化操作
    cout << "\n6. 测试唯一化操作:" << endl;
    // 添加一些重复元素
    complexVector.push_back(complexVector[0]);
    complexVector.push_back(complexVector[1]);
    cout << "添加重复元素后:" << endl;
    printVectorInfo(complexVector, "添加重复后");
    
    unique(complexVector);
    printVectorInfo(complexVector, "唯一化后");
    
    cout << "\n=== 排序效率比较 ===" << endl;
    
    // 准备三种不同顺序的向量
    Vector<Complex<double>> sequentialVector;    // 顺序
    Vector<Complex<double>> randomVector;        // 乱序  
    Vector<Complex<double>> reverseVector;       // 逆序
    
    // 生成测试数据
    for (int i = 0; i < 1000; ++i) {
        double real = i * 0.1;
        double imag = i * 0.1;
        sequentialVector.push_back(Complex<double>(real, imag));
        randomVector.push_back(generateRandomComplex(-50.0, 50.0));
        reverseVector.push_back(Complex<double>((999-i) * 0.1, (999-i) * 0.1));
    }
    
    // 测试起泡排序效率
    cout << "\n起泡排序效率比较:" << endl;
    
    Vector<Complex<double>> testVec = sequentialVector;
    clock_t start = clock();
    bubbleSort(testVec);
    clock_t end = clock();
    cout << "顺序数据: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = randomVector;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    cout << "乱序数据: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = reverseVector;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    cout << "逆序数据: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    // 测试归并排序效率
    cout << "\n归并排序效率比较:" << endl;
    
    testVec = sequentialVector;
    start = clock();
    testVec.sort_merge();
    end = clock();
    cout << "顺序数据: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = randomVector;
    start = clock();
    testVec.sort_merge();
    end = clock();
    cout << "乱序数据: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = reverseVector;
    start = clock();
    testVec.sort_merge();
    end = clock();
    cout << "逆序数据: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    cout << "\n=== 区间查找测试 ===" << endl;
    
    // 先对向量排序
    complexVector.sort_merge();
    printVectorInfo(complexVector, "排序后向量");
    
    // 区间查找
    double m1 = 2.0, m2 = 5.0;
    Vector<Complex<double>> rangeResult = rangeSearch(complexVector, m1, m2);
    printVectorInfo(rangeResult, "模在[" + to_string(m1) + "," + to_string(m2) + ")的元素");
    
    // 验证区间查找结果
    cout << "\n验证区间查找结果:" << endl;
    for (int i = 0; i < rangeResult.size(); ++i) {
        double mod = rangeResult[i].mod();
        cout << rangeResult[i] << " (模=" << mod << ")" << endl;
    }
    
    return 0;
}
