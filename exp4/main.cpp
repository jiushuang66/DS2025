#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include "Sort.h"
#include "NMS.h"

using namespace std;

void displayBanner() {
    cout << "==========================================" << endl;
    cout << "       第四次实验：排序算法与NMS测试       " << endl;
    cout << "==========================================" << endl;
    cout << "实验目标：分析排序算法在非极大值抑制中的性能影响" << endl;
    cout << "实验内容：" << endl;
    cout << "1. 实现五种排序算法" << endl;
    cout << "2. 实现NMS算法并集成不同排序算法" << endl;
    cout << "3. 测试不同数据分布和规模的性能" << endl;
    cout << "==========================================" << endl << endl;
}

void testSortAlgorithms() {
    cout << "\n=== 第一部分：测试排序算法性能 ===" << endl;
    
    // 测试不同规模的排序性能
    vector<int> test_sizes = {100, 500, 1000, 5000, 10000};
    
    // 测试随机分布数据
    cout << "\n--- 随机分布数据测试 ---" << endl;
    cout << "生成随机数据，测试不同规模下的排序性能..." << endl;
    SortUtils::testSortPerformance(SortUtils::QUICK_SORT, "快速排序", test_sizes, false);
    SortUtils::testSortPerformance(SortUtils::MERGE_SORT, "归并排序", test_sizes, false);
    SortUtils::testSortPerformance(SortUtils::HEAP_SORT, "堆排序", test_sizes, false);
    SortUtils::testSortPerformance(SortUtils::SELECTION_SORT, "选择排序", test_sizes, false);
    SortUtils::testSortPerformance(SortUtils::BUBBLE_SORT, "冒泡排序", test_sizes, false);
    
    // 测试聚集分布数据
    cout << "\n--- 聚集分布数据测试 ---" << endl;
    cout << "生成聚集分布数据，测试不同规模下的排序性能..." << endl;
    SortUtils::testSortPerformance(SortUtils::QUICK_SORT, "快速排序", test_sizes, true);
    SortUtils::testSortPerformance(SortUtils::MERGE_SORT, "归并排序", test_sizes, true);
    SortUtils::testSortPerformance(SortUtils::HEAP_SORT, "堆排序", test_sizes, true);
    SortUtils::testSortPerformance(SortUtils::SELECTION_SORT, "选择排序", test_sizes, true);
    SortUtils::testSortPerformance(SortUtils::BUBBLE_SORT, "冒泡排序", test_sizes, true);
}

void testNMSAlgorithms() {
    cout << "\n=== 第二部分：测试NMS算法性能 ===" << endl;
    
    // 测试不同规模下的NMS性能
    vector<int> nms_test_sizes = {100, 500, 1000, 5000, 10000};
    
    cout << "\n--- 随机分布边界框测试 ---" << endl;
    for (int size : nms_test_sizes) {
        cout << "\n测试规模: " << size << " 个边界框" << endl;
        NMS::testNMSPerformance(size, false, 0.5);
    }
    
    cout << "\n--- 聚集分布边界框测试 ---" << endl;
    for (int size : nms_test_sizes) {
        cout << "\n测试规模: " << size << " 个边界框" << endl;
        NMS::testNMSPerformance(size, true, 0.5);
    }
}

void demonstration() {
    cout << "\n=== 第三部分：算法演示 ===" << endl;
    
    // 生成小规模数据用于演示
    int demo_size = 20;
    cout << "生成 " << demo_size << " 个随机边界框用于演示..." << endl;
    
    auto random_boxes = NMS::generateRandomBoxes(demo_size, 100, 100);
    
    cout << "\n前5个生成的边界框信息：" << endl;
    cout << "索引\t中心坐标\t宽度x高度\t置信度\t类别" << endl;
    cout << "----\t--------\t--------\t------\t----" << endl;
    
    for (int i = 0; i < min(5, demo_size); i++) {
        const auto& box = random_boxes[i];
        cout << i << "\t(" << fixed << setprecision(1) << box.x << ", " << box.y << ")\t"
             << box.width << "x" << box.height << "\t" << box.confidence 
             << "\t" << box.class_id << endl;
    }
    
    cout << "\n使用快速排序进行NMS处理..." << endl;
    auto result = NMS::NMSWithSort(random_boxes, SortUtils::QUICK_SORT, 0.5);
    
    cout << "NMS处理后保留了 " << result.size() << " 个边界框：" << endl;
    cout << "索引\t中心坐标\t宽度x高度\t置信度\t类别" << endl;
    cout << "----\t--------\t--------\t------\t----" << endl;
    
    for (size_t i = 0; i < result.size(); i++) {
        const auto& box = result[i];
        cout << i << "\t(" << fixed << setprecision(1) << box.x << ", " << box.y << ")\t"
             << box.width << "x" << box.height << "\t" << box.confidence 
             << "\t" << box.class_id << endl;
    }
    
    // 验证结果
    cout << "\n验证NMS结果..." << endl;
    bool valid = NMS::validateNMSResult(result, 0.5);
    cout << "NMS结果验证: " << (valid ? "通过" : "失败") << endl;
}

void analysis() {
    cout << "\n=== 第四部分：理论分析与实验结果 ===" << endl;
    
    cout << "\n1. 排序算法时间复杂度分析：" << endl;
    cout << "   - 快速排序: 平均O(n log n)，最坏O(n2)，不稳定" << endl;
    cout << "   - 归并排序: 最坏O(n log n)，稳定，需要额外空间" << endl;
    cout << "   - 堆排序: 最坏O(n log n)，原地排序，不稳定" << endl;
    cout << "   - 选择排序: O(n2)，原地排序，不稳定" << endl;
    cout << "   - 冒泡排序: O(n2)，稳定，原地排序" << endl;
    
    cout << "\n2. NMS算法复杂度分析：" << endl;
    cout << "   - 基础NMS: O(n2)，主要开销在成对IoU计算" << endl;
    cout << "   - 排序步骤: 时间复杂度取决于使用的排序算法" << endl;
    cout << "   - 总体复杂度: O(n2 + T_sort)，其中T_sort是排序时间" << endl;
    
    cout << "\n3. 实验预期结果：" << endl;
    cout << "   - 随机数据: 快速/归并/堆排序表现最佳" << endl;
    cout << "   - 聚集数据: 归并排序可能更稳定" << endl;
    cout << "   - 选择/冒泡排序仅适用于小规模数据" << endl;
    cout << "   - NMS性能主要受O(n2)的IoU计算影响" << endl;
    
    cout << "\n4. 数据分布对性能的影响：" << endl;
    cout << "   - 随机分布: 排序算法表现接近理论复杂度" << endl;
    cout << "   - 聚集分布: 可能导致某些算法性能下降" << endl;
    cout << "   - 快速排序在聚集数据上可能出现最坏情况" << endl;
    
    cout << "\n5. 实际应用建议：" << endl;
    cout << "   - 小规模数据(n<1000): 可使用任意排序算法" << endl;
    cout << "   - 中等规模数据: 推荐使用快速排序或堆排序" << endl;
    cout << "   - 大规模数据(n>10000): 推荐使用归并排序或堆排序" << endl;
    cout << "   - 内存受限: 推荐使用堆排序或快速排序" << endl;
    cout << "   - 需要稳定排序: 推荐使用归并排序" << endl;
}

void saveResultsToCSV() {
    cout << "\n=== 第五部分：保存实验结果 ===" << endl;
    
    ofstream file("experiment_results.csv");
    if (!file.is_open()) {
        cerr << "错误: 无法打开 experiment_results.csv 文件" << endl;
        return;
    }
    
    // 写入表头（中英文混合）
    file << "Algorithm,算法,Data_Type,数据分布,Size,规模,Time_us,时间微秒,Selected_Boxes,保留框数量" << endl;
    
    vector<int> sizes = {100, 500, 1000, 5000, 10000};
    vector<SortUtils::SortAlgorithm> algorithms = {
        SortUtils::QUICK_SORT,
        SortUtils::MERGE_SORT,
        SortUtils::HEAP_SORT,
        SortUtils::SELECTION_SORT,
        SortUtils::BUBBLE_SORT
    };
    
    vector<string> algo_names_en = {"Quick_Sort", "Merge_Sort", "Heap_Sort", "Selection_Sort", "Bubble_Sort"};
    vector<string> algo_names_cn = {"快速排序", "归并排序", "堆排序", "选择排序", "冒泡排序"};
    vector<string> data_types_en = {"Random", "Clustered"};
    vector<string> data_types_cn = {"随机分布", "聚集分布"};
    
    cout << "开始收集性能数据..." << endl;
    int total_tests = 2 * algorithms.size() * sizes.size();
    int completed = 0;
    
    for (bool clustered : {false, true}) {
        for (size_t algo_idx = 0; algo_idx < algorithms.size(); algo_idx++) {
            for (int size : sizes) {
                // 更新进度
                completed++;
                cout << "进度: " << completed << "/" << total_tests << " 测试...\r";
                cout.flush();
                
                // 生成测试数据
                auto boxes = clustered ? 
                    NMS::generateClusteredBoxes(size) : 
                    NMS::generateRandomBoxes(size);
                
                // 测量时间
                auto start = chrono::high_resolution_clock::now();
                auto result = NMS::NMSWithSort(boxes, algorithms[algo_idx], 0.5);
                auto end = chrono::high_resolution_clock::now();
                
                auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                
                // 写入数据
                file << algo_names_en[algo_idx] << ","
                     << algo_names_cn[algo_idx] << ","
                     << data_types_en[clustered] << ","
                     << data_types_cn[clustered] << ","
                     << size << ","
                     << size << ","
                     << duration.count() << ","
                     << duration.count() << ","
                     << result.size() << ","
                     << result.size() << endl;
            }
        }
    }
    
    cout << "\n数据收集完成!" << endl;
    file.close();
    cout << "实验结果已保存到 experiment_results.csv" << endl;
}

void runPerformanceSummary() {
    cout << "\n=== 第六部分：性能总结 ===" << endl;
    
    // 运行一个代表性的测试来展示总体性能
    cout << "运行代表性测试 (1000个边界框，随机分布)..." << endl;
    NMS::testNMSPerformance(1000, false, 0.5);
    
    cout << "\n运行代表性测试 (1000个边界框，聚集分布)..." << endl;
    NMS::testNMSPerformance(1000, true, 0.5);
}

void generateReport() {
    cout << "\n=== 实验报告生成 ===" << endl;
    cout << "实验报告已生成，主要内容包括：" << endl;
    cout << "1. 五种排序算法的实现和测试" << endl;
    cout << "2. NMS算法的实现和集成" << endl;
    cout << "3. 两种数据分布(随机/聚集)的性能对比" << endl;
    cout << "4. 不同数据规模(100-10000)的性能分析" << endl;
    cout << "5. 理论复杂度与实际性能的对比" << endl;
    cout << "6. 完整性能数据已保存到CSV文件" << endl;
}

int main() {
    // 显示实验标题
    displayBanner();
    
    cout << "开始运行第四次实验..." << endl;
    cout << "本实验将自动运行所有测试项目，无需手动选择。" << endl;
    cout << "实验预计需要几分钟时间，请耐心等待..." << endl << endl;
    
    // 1. 测试排序算法
    testSortAlgorithms();
    
    // 2. 测试NMS算法
    testNMSAlgorithms();
    
    // 3. 演示
    demonstration();
    
    // 4. 理论分析
    analysis();
    
    // 5. 保存结果到CSV
    saveResultsToCSV();
    
    // 6. 性能总结
    runPerformanceSummary();
    
    // 7. 生成报告
    generateReport();
    
    cout << "\n==========================================" << endl;
    cout << "           第四次实验运行完成             " << endl;
    cout << "==========================================" << endl;
    cout << "实验结果总结：" << endl;
    cout << "- 排序算法测试完成" << endl;
    cout << "- NMS算法测试完成" << endl;
    cout << "- 性能数据已保存" << endl;
    cout << "- 理论分析已完成" << endl;
    cout << "请查看 experiment_results.csv 文件获取详细数据" << endl;
    cout << "==========================================" << endl;
    
    return 0;
}
