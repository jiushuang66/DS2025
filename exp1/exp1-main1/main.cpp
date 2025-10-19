#include "Complex.h"
#include "Vector.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

// �����������
template <typename T>
Complex<T> generateRandomComplex(T minVal, T maxVal) {
    T real = minVal + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX/(maxVal - minVal)));
    T imag = minVal + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX/(maxVal - minVal)));
    return Complex<T>(real, imag);
}

// ��������
template <typename T>
void bubbleSort(Vector<Complex<T>>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            // �Ƚ�ģ����ģ����ͬʱ�Ƚ�ʵ��
            if (vec[j].mod() > vec[j+1].mod() || 
                (fabs(vec[j].mod() - vec[j+1].mod()) < 1e-6 && vec[j].getReal() > vec[j+1].getReal())) {
                swap(vec[j], vec[j+1]);
            }
        }
    }
}

// ������ң�����ģ����[m1,m2)������Ԫ��
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

// ����Ψһ����ȥ���ظ�Ԫ�أ�
template <typename T>
void unique(Vector<Complex<T>>& vec) {
    if (vec.empty()) return;
    
    // ������
    vec.sort_merge();
    
    // ��ȥ��
    int writeIndex = 0;
    for (int readIndex = 1; readIndex < vec.size(); ++readIndex) {
        if (!(vec[readIndex] == vec[writeIndex])) {
            vec[++writeIndex] = vec[readIndex];
        }
    }
    
    // ������С
    while (vec.size() > writeIndex + 1) {
        vec.pop_back();
    }
}

// ��ӡ������Ϣ
template <typename T>
void printVectorInfo(const Vector<Complex<T>>& vec, const string& name) {
    cout << name << " (size: " << vec.size() << "): ";
    vec.print();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    const int NUM_COMPLEX = 20;  // ��������
    Vector<Complex<double>> complexVector;
    
    cout << "=== ���������������� ===" << endl;
    
    // 1. ��������������������ظ��
    cout << "\n1. ���������������:" << endl;
    for (int i = 0; i < NUM_COMPLEX; ++i) {
        complexVector.push_back(generateRandomComplex(-5.0, 5.0));
    }
    printVectorInfo(complexVector, "ԭʼ����");
    
    // 2. �������Ҳ���
    cout << "\n2. �������Ҳ���:" << endl;
    for (int i = 0; i < complexVector.size(); ++i) {
        int j = rand() % complexVector.size();
        swap(complexVector[i], complexVector[j]);
    }
    printVectorInfo(complexVector, "���Һ�����");
    
    // 3. ���Բ��Ҳ���
    cout << "\n3. ���Բ��Ҳ���:" << endl;
    Complex<double> searchTarget(complexVector[0].getReal(), complexVector[0].getImag());
    int foundIndex = complexVector.find(searchTarget);
    if (foundIndex != -1) {
        cout << "�ҵ�Ԫ�� " << searchTarget << " ��λ�� " << foundIndex << endl;
    } else {
        cout << "δ�ҵ�Ԫ�� " << searchTarget << endl;
    }
    
    // 4. ���Բ������
    cout << "\n4. ���Բ������:" << endl;
    Complex<double> newComplex(99.0, 99.0);
    complexVector.insert(complexVector.size() / 2, newComplex);
    printVectorInfo(complexVector, "������Ԫ�غ�");
    
    // 5. ����ɾ������
    cout << "\n5. ����ɾ������:" << endl;
    Complex<double> removed = complexVector.erase(complexVector.size() / 2);
    cout << "ɾ����Ԫ��: " << removed << endl;
    printVectorInfo(complexVector, "ɾ��Ԫ�غ�");
    
    // 6. ����Ψһ������
    cout << "\n6. ����Ψһ������:" << endl;
    // ���һЩ�ظ�Ԫ��
    complexVector.push_back(complexVector[0]);
    complexVector.push_back(complexVector[1]);
    cout << "����ظ�Ԫ�غ�:" << endl;
    printVectorInfo(complexVector, "����ظ���");
    
    unique(complexVector);
    printVectorInfo(complexVector, "Ψһ����");
    
    cout << "\n=== ����Ч�ʱȽ� ===" << endl;
    
    // ׼�����ֲ�ͬ˳�������
    Vector<Complex<double>> sequentialVector;    // ˳��
    Vector<Complex<double>> randomVector;        // ����  
    Vector<Complex<double>> reverseVector;       // ����
    
    // ���ɲ�������
    for (int i = 0; i < 1000; ++i) {
        double real = i * 0.1;
        double imag = i * 0.1;
        sequentialVector.push_back(Complex<double>(real, imag));
        randomVector.push_back(generateRandomComplex(-50.0, 50.0));
        reverseVector.push_back(Complex<double>((999-i) * 0.1, (999-i) * 0.1));
    }
    
    // ������������Ч��
    cout << "\n��������Ч�ʱȽ�:" << endl;
    
    Vector<Complex<double>> testVec = sequentialVector;
    clock_t start = clock();
    bubbleSort(testVec);
    clock_t end = clock();
    cout << "˳������: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = randomVector;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    cout << "��������: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = reverseVector;
    start = clock();
    bubbleSort(testVec);
    end = clock();
    cout << "��������: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    // ���Թ鲢����Ч��
    cout << "\n�鲢����Ч�ʱȽ�:" << endl;
    
    testVec = sequentialVector;
    start = clock();
    testVec.sort_merge();
    end = clock();
    cout << "˳������: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = randomVector;
    start = clock();
    testVec.sort_merge();
    end = clock();
    cout << "��������: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    testVec = reverseVector;
    start = clock();
    testVec.sort_merge();
    end = clock();
    cout << "��������: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
    
    cout << "\n=== ������Ҳ��� ===" << endl;
    
    // �ȶ���������
    complexVector.sort_merge();
    printVectorInfo(complexVector, "���������");
    
    // �������
    double m1 = 2.0, m2 = 5.0;
    Vector<Complex<double>> rangeResult = rangeSearch(complexVector, m1, m2);
    printVectorInfo(rangeResult, "ģ��[" + to_string(m1) + "," + to_string(m2) + ")��Ԫ��");
    
    // ��֤������ҽ��
    cout << "\n��֤������ҽ��:" << endl;
    for (int i = 0; i < rangeResult.size(); ++i) {
        double mod = rangeResult[i].mod();
        cout << rangeResult[i] << " (ģ=" << mod << ")" << endl;
    }
    
    return 0;
}
