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

    // 褰掑苟鎺掑簭杈呭姪鍑芥暟锛氬悎骞朵袱涓湁搴忓瓙鍖洪棿 [left, mid] 鍜?[mid+1, right]
    void merge(int left, int mid, int right) {
        // 鍒涘缓涓存椂鏁扮粍瀛樺偍鍚堝苟缁撴灉
        T* temp = new T[right - left + 1];
        int i = left, j = mid + 1, k = 0;

        // 鍚堝苟涓や釜瀛愬尯闂?
        while (i <= mid && j <= right) {
            if (elements_[i] <= elements_[j]) {
                temp[k++] = elements_[i++];
            } else {
                temp[k++] = elements_[j++];
            }
        }

        // 澶勭悊鍓╀綑鍏冪礌
        while (i <= mid) temp[k++] = elements_[i++];
        while (j <= right) temp[k++] = elements_[j++];

        // 澶嶅埗鍥炲師鏁扮粍
        for (k = 0; k < right - left + 1; ++k) {
            elements_[left + k] = temp[k];
        }

        delete[] temp;  // 閲婃斁涓存椂鏁扮粍鍐呭瓨
    }

    // 褰掑苟鎺掑簭閫掑綊鍑芥暟
    void mergeSort(int left, int right) {
        if (left >= right) return;  // 閫掑綊缁堟鏉′欢
        int mid = left + (right - left) / 2;  // 閬垮厤婧㈠嚭
        mergeSort(left, mid);                 // 鎺掑簭宸﹀崐閮ㄥ垎
        mergeSort(mid + 1, right);            // 鎺掑簭鍙冲崐閮ㄥ垎
        merge(left, mid, right);              // 鍚堝苟缁撴灉
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

    // 褰掑苟鎺掑簭鎺ュ彛锛堜緵澶栭儴璋冪敤锛?
    void sort_merge() {
        if (size_ <= 1) return;  // 绌烘暟缁勬垨鍗曞厓绱犳暟缁勬棤闇€鎺掑簭
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
    
    // Assignment operator
    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] elements_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            elements_ = new T[capacity_];
            for (int i = 0; i < size_; ++i) {
                elements_[i] = other.elements_[i];
            }
        }
        return *this;
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
    
    
