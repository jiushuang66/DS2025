#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

template <typename T>
class Vector {
private:
    int size_;       // Current number of elements
    int capacity_;   // Current capacity
    T* elements_;    // Array to store elements

    // Double the capacity when full
    void expand() {
        if (size_ < capacity_) return;
        capacity_ = max(capacity_, 1);
        T* old = elements_;
        elements_ = new T[capacity_ *= 2];
        for (int i = 0; i < size_; ++i) {
            elements_[i] = old[i];
        }
        delete[] old;
    }

    // Halve the capacity when less than 1/4 full
    void shrink() {
        if (capacity_ < 4 || size_ * 4 > capacity_) return;
        T* old = elements_;
        elements_ = new T[capacity_ /= 2];
        for (int i = 0; i < size_; ++i) {
            elements_[i] = old[i];
        }
        delete[] old;
    }

    // 归并排序辅助函数：合并两个有序子区间 [left, mid] 和 [mid+1, right]
    void merge(int left, int mid, int right) {
        // 创建临时数组存储合并结果
        T* temp = new T[right - left + 1];
        int i = left, j = mid + 1, k = 0;

        // 合并两个子区间
        while (i <= mid && j <= right) {
            if (elements_[i] <= elements_[j]) {
                temp[k++] = elements_[i++];
            } else {
                temp[k++] = elements_[j++];
            }
        }

        // 处理剩余元素
        while (i <= mid) temp[k++] = elements_[i++];
        while (j <= right) temp[k++] = elements_[j++];

        // 复制回原数组
        for (k = 0; k < right - left + 1; ++k) {
            elements_[left + k] = temp[k];
        }

        delete[] temp;  // 释放临时数组内存
    }

    // 归并排序递归函数
    void mergeSort(int left, int right) {
        if (left >= right) return;  // 递归终止条件
        int mid = left + (right - left) / 2;  // 避免溢出
        mergeSort(left, mid);                 // 排序左半部分
        mergeSort(mid + 1, right);            // 排序右半部分
        merge(left, mid, right);              // 合并结果
    }

public:
    // Constructor
    Vector(int cap = 10) : size_(0), capacity_(cap) {
        elements_ = new T[capacity_];
    }

    // Constructor from array
    Vector(T* arr, int n) : size_(n), capacity_(n * 2) {
        elements_ = new T[capacity_];
        for (int i = 0; i < n; ++i) {
            elements_[i] = arr[i];
        }
    }

    // Copy constructor
    Vector(const Vector<T>& other) : size_(other.size_), capacity_(other.capacity_) {
        elements_ = new T[capacity_];
        for (int i = 0; i < size_; ++i) {
            elements_[i] = other.elements_[i];
        }
    }

    // Destructor
    ~Vector() {
        delete[] elements_;
    }

    // Basic properties
    int size() const { return size_; }
    int capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    // Element access
    T& operator[](int index) { return elements_[index]; }
    const T& operator[](int index) const { return elements_[index]; }

    // Modifiers
    void push_back(const T& val) {
        expand();
        elements_[size_++] = val;
    }

    void insert(int pos, const T& val) {
        if (pos < 0 || pos > size_) return;
        expand();
        for (int i = size_; i > pos; --i) {
            elements_[i] = elements_[i - 1];
        }
        elements_[pos] = val;
        size_++;
    }

    T pop_back() {
        if (empty()) exit(1);
        T val = elements_[--size_];
        shrink();
        return val;
    }

    T erase(int pos) {
        if (pos < 0 || pos >= size_) exit(1);
        T val = elements_[pos];
        for (int i = pos; i < size_ - 1; ++i) {
            elements_[i] = elements_[i + 1];
        }
        size_--;
        shrink();
        return val;
    }

    // Search
    int find(const T& val) const {
        for (int i = 0; i < size_; ++i) {
            if (elements_[i] == val) return i;
        }
        return -1;
    }

    // Sort (quick sort)
    void sort() {
        quick_sort(0, size_ - 1);
    }

    // 归并排序接口（供外部调用）
    void sort_merge() {
        if (size_ <= 1) return;  // 空数组或单元素数组无需排序
        mergeSort(0, size_ - 1);
    }

    // Output all elements
    void print() const {
        cout << "[";
        for (int i = 0; i < size_; ++i) {
            cout << elements_[i];
            if (i != size_ - 1) cout << ", ";
        }
        cout << "]" << endl;
    }

private:
    // Quick sort helper
    void quick_sort(int left, int right) {
        if (left >= right) return;
        int pivot = partition(left, right);
        quick_sort(left, pivot - 1);
        quick_sort(pivot + 1, right);
    }

    int partition(int left, int right) {
        T pivot = elements_[right];
        int i = left - 1;
        for (int j = left; j < right; ++j) {
            if (elements_[j] <= pivot) {
                swap(elements_[++i], elements_[j]);
            }
        }
        swap(elements_[i + 1], elements_[right]);
        return i + 1;
    }
};

#endif // VECTOR_H
    
    