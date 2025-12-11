#ifndef NMS_H
#define NMS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <iomanip>
#include "Sort.h"

namespace NMS {
    
    // 边界框结构体
    struct BoundingBox {
        double x, y;          // 中心坐标
        double width, height; // 宽高
        double confidence;    // 置信度
        int class_id;         // 类别ID
        
        BoundingBox(double x = 0, double y = 0, double w = 0, double h = 0, 
                   double conf = 0, int cls = 0)
            : x(x), y(y), width(w), height(h), confidence(conf), class_id(cls) {}
        
        // 计算IoU（交并比）
        double iou(const BoundingBox& other) const {
            // 计算两个框的坐标
            double x1 = x - width / 2;
            double y1 = y - height / 2;
            double x2 = x + width / 2;
            double y2 = y + height / 2;
            
            double ox1 = other.x - other.width / 2;
            double oy1 = other.y - other.height / 2;
            double ox2 = other.x + other.width / 2;
            double oy2 = other.y + other.height / 2;
            
            // 计算交集区域
            double inter_x1 = std::max(x1, ox1);
            double inter_y1 = std::max(y1, oy1);
            double inter_x2 = std::min(x2, ox2);
            double inter_y2 = std::min(y2, oy2);
            
            double inter_width = std::max(0.0, inter_x2 - inter_x1);
            double inter_height = std::max(0.0, inter_y2 - inter_y1);
            double inter_area = inter_width * inter_height;
            
            // 计算并集区域
            double area1 = width * height;
            double area2 = other.width * other.height;
            double union_area = area1 + area2 - inter_area;
            
            if (union_area == 0) return 0;
            return inter_area / union_area;
        }
        
        // 用于排序的比较函数（按置信度降序）
        bool operator<(const BoundingBox& other) const {
            return confidence > other.confidence; // 降序
        }
    };
    
    // 生成随机分布的边界框
    std::vector<BoundingBox> generateRandomBoxes(int n, double image_width = 640, double image_height = 480) {
        std::vector<BoundingBox> boxes;
        boxes.reserve(n);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> pos_dis_x(0, image_width);
        std::uniform_real_distribution<> pos_dis_y(0, image_height);
        std::uniform_real_distribution<> size_dis(10, 100);
        std::uniform_real_distribution<> conf_dis(0.1, 0.99);
        std::uniform_int_distribution<> class_dis(0, 9);
        
        for (int i = 0; i < n; i++) {
            double x = pos_dis_x(gen);
            double y = pos_dis_y(gen);
            double w = size_dis(gen);
            double h = size_dis(gen);
            double conf = conf_dis(gen);
            int cls = class_dis(gen);
            
            boxes.emplace_back(x, y, w, h, conf, cls);
        }
        
        return boxes;
    }
    
    // 生成聚集分布的边界框
    std::vector<BoundingBox> generateClusteredBoxes(int n, double image_width = 640, double image_height = 480) {
        std::vector<BoundingBox> boxes;
        boxes.reserve(n);
        
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // 生成几个聚类中心
        int clusters = std::max(1, n / 20);
        std::vector<std::pair<double, double>> cluster_centers(clusters);
        std::uniform_real_distribution<> center_dis_x(50, image_width - 50);
        std::uniform_real_distribution<> center_dis_y(50, image_height - 50);
        
        for (int i = 0; i < clusters; i++) {
            cluster_centers[i] = std::make_pair(center_dis_x(gen), center_dis_y(gen));
        }
        
        // 围绕聚类中心生成边界框
        std::normal_distribution<> normal_dis(0.0, 30.0);
        std::uniform_real_distribution<> size_dis(10, 60);
        std::uniform_real_distribution<> conf_dis(0.1, 0.99);
        std::uniform_int_distribution<> class_dis(0, 9);
        
        int boxes_per_cluster = n / clusters;
        int generated = 0;
        
        for (int c = 0; c < clusters; c++) {
            int to_generate = (c == clusters - 1) ? (n - generated) : boxes_per_cluster;
            
            for (int i = 0; i < to_generate; i++) {
                double x = cluster_centers[c].first + normal_dis(gen);
                double y = cluster_centers[c].second + normal_dis(gen);
                double w = size_dis(gen);
                double h = size_dis(gen);
                double conf = conf_dis(gen);
                int cls = class_dis(gen);
                
                // 确保边界框在图像范围内
                x = std::max(w/2, std::min(image_width - w/2, x));
                y = std::max(h/2, std::min(image_height - h/2, y));
                
                boxes.emplace_back(x, y, w, h, conf, cls);
                generated++;
            }
        }
        
        return boxes;
    }
    
    // 基础NMS算法（使用标准库排序）
    std::vector<BoundingBox> basicNMS(const std::vector<BoundingBox>& boxes, 
                                     double iou_threshold = 0.5) {
        if (boxes.empty()) return {};
        
        // 复制并排序（按置信度降序）
        std::vector<BoundingBox> sorted_boxes = boxes;
        std::sort(sorted_boxes.begin(), sorted_boxes.end());
        
        std::vector<BoundingBox> selected;
        std::vector<bool> suppressed(sorted_boxes.size(), false);
        
        for (size_t i = 0; i < sorted_boxes.size(); i++) {
            if (suppressed[i]) continue;
            
            selected.push_back(sorted_boxes[i]);
            
            for (size_t j = i + 1; j < sorted_boxes.size(); j++) {
                if (!suppressed[j] && sorted_boxes[i].class_id == sorted_boxes[j].class_id) {
                    double iou = sorted_boxes[i].iou(sorted_boxes[j]);
                    if (iou > iou_threshold) {
                        suppressed[j] = true;
                    }
                }
            }
        }
        
        return selected;
    }
    
    // 使用指定排序算法的NMS
    std::vector<BoundingBox> NMSWithSort(const std::vector<BoundingBox>& boxes, 
                                        SortUtils::SortAlgorithm sort_algo,
                                        double iou_threshold = 0.5) {
        if (boxes.empty()) return {};
        
        // 提取置信度用于排序
        std::vector<double> confidences;
        confidences.reserve(boxes.size());
        for (const auto& box : boxes) {
            confidences.push_back(box.confidence);
        }
        
        // 使用指定算法排序
        SortUtils::sort(confidences, sort_algo);
        
        // 根据排序后的置信度选择边界框
        std::vector<bool> suppressed(boxes.size(), false);
        std::vector<BoundingBox> selected;
        
        for (size_t i = 0; i < confidences.size(); i++) {
            // 找到当前置信度对应的边界框
            int box_idx = -1;
            for (size_t j = 0; j < boxes.size(); j++) {
                if (!suppressed[j] && std::abs(boxes[j].confidence - confidences[i]) < 1e-6) {
                    box_idx = j;
                    break;
                }
            }
            
            if (box_idx == -1) continue;
            
            selected.push_back(boxes[box_idx]);
            suppressed[box_idx] = true;
            
            // 抑制重叠框
            for (size_t j = 0; j < boxes.size(); j++) {
                if (!suppressed[j] && boxes[box_idx].class_id == boxes[j].class_id) {
                    double iou = boxes[box_idx].iou(boxes[j]);
                    if (iou > iou_threshold) {
                        suppressed[j] = true;
                    }
                }
            }
        }
        
        return selected;
    }
    
    // 测试NMS性能
    void testNMSPerformance(int num_boxes, bool clustered = false, double iou_threshold = 0.5) {
        std::cout << "\n=== Testing NMS Performance ===" << std::endl;
        std::cout << "Number of boxes: " << num_boxes << std::endl;
        std::cout << "Distribution: " << (clustered ? "Clustered" : "Random") << std::endl;
        std::cout << "IoU Threshold: " << iou_threshold << std::endl;
        
        // 生成边界框
        auto boxes = clustered ? generateClusteredBoxes(num_boxes) : generateRandomBoxes(num_boxes);
        
        // 测试不同排序算法的NMS性能
        std::vector<std::pair<SortUtils::SortAlgorithm, std::string>> algorithms = {
            std::make_pair(SortUtils::QUICK_SORT, "Quick Sort"),
            std::make_pair(SortUtils::MERGE_SORT, "Merge Sort"),
            std::make_pair(SortUtils::HEAP_SORT, "Heap Sort"),
            std::make_pair(SortUtils::SELECTION_SORT, "Selection Sort"),
            std::make_pair(SortUtils::BUBBLE_SORT, "Bubble Sort")
        };
        
        std::vector<int> selected_counts;
        std::vector<double> execution_times;
        
        for (size_t idx = 0; idx < algorithms.size(); idx++) {
            const auto& algo_pair = algorithms[idx];
            SortUtils::SortAlgorithm algo = algo_pair.first;
            const std::string& name = algo_pair.second;
            
            auto start = std::chrono::high_resolution_clock::now();
            auto result = NMSWithSort(boxes, algo, iou_threshold);
            auto end = std::chrono::high_resolution_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            selected_counts.push_back(result.size());
            execution_times.push_back(static_cast<double>(duration.count()));
            
            std::cout << name << ": " << result.size() << " boxes selected, " 
                      << duration.count() << " μs" << std::endl;
        }
        
        // 输出性能对比
        std::cout << "\nPerformance Summary:" << std::endl;
        double min_time = *std::min_element(execution_times.begin(), execution_times.end());
        
        for (size_t i = 0; i < algorithms.size(); i++) {
            double speedup = min_time / execution_times[i];
            std::cout << algorithms[i].second << ": " << execution_times[i] << " μs, "
                      << "Speedup: " << std::fixed << std::setprecision(2) 
                      << speedup << "x" << std::endl;
        }
    }
    
    // 验证NMS结果正确性
    bool validateNMSResult(const std::vector<BoundingBox>& result, double iou_threshold = 0.5) {
        for (size_t i = 0; i < result.size(); i++) {
            for (size_t j = i + 1; j < result.size(); j++) {
                if (result[i].class_id == result[j].class_id) {
                    double iou = result[i].iou(result[j]);
                    if (iou > iou_threshold) {
                        std::cout << "Validation failed: Box " << i << " and " << j 
                                  << " have IoU " << iou << " > threshold " << iou_threshold << std::endl;
                        return false;
                    }
                }
            }
        }
        return true;
    }
    
} // namespace NMS

#endif // NMS_H
