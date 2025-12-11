#ifndef SORT_H
#define SORT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <chrono>
#include <random>
#include <iomanip>

namespace SortUtils {
    
    // 测试数据结构体
    struct TestData {
        std::vector<double> arr;
        std::string name;
        int size;
        
        TestData(int n, const std::string& dist_type) : size(n), name(dist_type) {
            arr.resize(n);
        }
    };
    
    // 计时器类
    class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::string operation_name;
        
    public:
        Timer(const std::string& name) : operation_name(name) {
            start_time = std::chrono::high_resolution_clock::now();
        }
        
        ~Timer() {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << operation_name << " took " << duration.count() << " microseconds" << std::endl;
        }
        
        double getElapsed() {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            return static_cast<double>(duration.count());
        }
    };
    
    // 交换函数
    template<typename T>
    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
    
    // 1. 快速排序
    template<typename T>
    void quickSort(std::vector<T>& arr, int left, int right) {
        if (left >= right) return;
        
        int i = left, j = right;
        T pivot = arr[(left + right) / 2];
        
        while (i <= j) {
            while (arr[i] > pivot) i++;  // 降序排序，用于NMS（置信度高的在前）
            while (arr[j] < pivot) j--;
            if (i <= j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        
        quickSort(arr, left, j);
        quickSort(arr, i, right);
    }
    
    template<typename T>
    void quickSort(std::vector<T>& arr) {
        if (arr.empty()) return;
        quickSort(arr, 0, arr.size() - 1);
    }
    
    // 2. 归并排序
    template<typename T>
    void merge(std::vector<T>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        std::vector<T> L(n1), R(n2);
        
        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];
        
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] >= R[j]) {  // 降序排序
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
        
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }
    
    template<typename T>
    void mergeSort(std::vector<T>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
    
    template<typename T>
    void mergeSort(std::vector<T>& arr) {
        if (arr.empty()) return;
        mergeSort(arr, 0, arr.size() - 1);
    }
    
    // 3. 堆排序
    template<typename T>
    void heapify(std::vector<T>& arr, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < n && arr[left] < arr[largest])  // 建立最大堆，但用于降序排序
            largest = left;
        
        if (right < n && arr[right] < arr[largest])
            largest = right;
        
        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }
    
    template<typename T>
    void heapSort(std::vector<T>& arr) {
        int n = arr.size();
        
        // 建立最大堆
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);
        
        // 提取元素
        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
        
        // 堆排序得到的是升序，反转得到降序
        std::reverse(arr.begin(), arr.end());
    }
    
    // 4. 选择排序
    template<typename T>
    void selectionSort(std::vector<T>& arr) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            int max_idx = i;
            for (int j = i + 1; j < n; j++) {
                if (arr[j] > arr[max_idx])  // 降序排序
                    max_idx = j;
            }
            swap(arr[i], arr[max_idx]);
        }
    }
    
    // 5. 冒泡排序（额外实现）
    template<typename T>
    void bubbleSort(std::vector<T>& arr) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] < arr[j + 1])  // 降序排序
                    swap(arr[j], arr[j + 1]);
            }
        }
    }
    
    // 排序算法包装器
    enum SortAlgorithm {
        QUICK_SORT,
        MERGE_SORT,
        HEAP_SORT,
        SELECTION_SORT,
        BUBBLE_SORT
    };
    
    template<typename T>
    void sort(std::vector<T>& arr, SortAlgorithm algorithm) {
        switch (algorithm) {
            case QUICK_SORT:
                quickSort(arr);
                break;
            case MERGE_SORT:
                mergeSort(arr);
                break;
            case HEAP_SORT:
                heapSort(arr);
                break;
            case SELECTION_SORT:
                selectionSort(arr);
                break;
            case BUBBLE_SORT:
                bubbleSort(arr);
                break;
        }
    }
    
    // 生成随机分布数据
    TestData generateRandomData(int n) {
        TestData data(n, "Random");
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        for (int i = 0; i < n; i++) {
            data.arr[i] = dis(gen);
        }
        return data;
    }
    
    // 生成聚集分布数据
    TestData generateClusteredData(int n) {
        TestData data(n, "Clustered");
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // 生成一些聚类中心
        int clusters = std::max(1, n / 100);
        std::vector<double> cluster_centers(clusters);
        std::uniform_real_distribution<> center_dis(0.0, 1.0);
        for (int i = 0; i < clusters; i++) {
            cluster_centers[i] = center_dis(gen);
        }
        
        // 围绕聚类中心生成数据
        std::normal_distribution<> normal_dis(0.0, 0.1);
        int points_per_cluster = n / clusters;
        int idx = 0;
        
        for (int c = 0; c < clusters; c++) {
            int points = (c == clusters - 1) ? (n - idx) : points_per_cluster;
            for (int i = 0; i < points; i++) {
                double val = cluster_centers[c] + normal_dis(gen);
                // 确保在[0,1]范围内
                val = std::max(0.0, std::min(1.0, val));
                data.arr[idx++] = val;
            }
        }
        
        // 填充剩余位置
        while (idx < n) {
            data.arr[idx++] = center_dis(gen);
        }
        
        return data;
    }
    
    // 测试排序算法性能
    void testSortPerformance(SortAlgorithm algorithm, const std::string& algo_name, 
                             const std::vector<int>& sizes, bool use_clustered = false) {
        std::cout << "\n=== Testing " << algo_name << " (" 
                  << (use_clustered ? "Clustered" : "Random") << " Distribution) ===" << std::endl;
        
        for (int size : sizes) {
            // 生成数据
            TestData data = use_clustered ? generateClusteredData(size) : generateRandomData(size);
            
            // 复制数据用于排序
            std::vector<double> arr_copy = data.arr;
            
            // 计时排序
            auto start = std::chrono::high_resolution_clock::now();
            sort(arr_copy, algorithm);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            // 验证排序结果（降序）
            bool sorted = true;
            for (size_t i = 1; i < arr_copy.size(); i++) {
                if (arr_copy[i] > arr_copy[i-1]) {
                    sorted = false;
                    break;
                }
            }
            
            std::cout << "Size: " << std::setw(6) << size 
                      << " | Time: " << std::setw(8) << duration.count() << " μs"
                      << " | Sorted: " << (sorted ? "Yes" : "No") << std::endl;
        }
    }
    
} // namespace SortUtils

#endif // SORT_H
