#include "Graph.h"
#include <iostream>
using namespace std;

// 任务1：构建图1的邻接矩阵（顶点A~H，编号0=A,1=B,...,7=H）
void task1_BuildGraph1(Graph& g1) {
    // 根据图1的边关系构建图（C++98兼容写法）
    g1.addEdge(0, 1, 4);   // A-B，权重4
    g1.addEdge(0, 3, 7);   // A-D，权重7
    g1.addEdge(1, 2, 12);  // B-C，权重12
    g1.addEdge(1, 4, 9);   // B-E，权重9
    g1.addEdge(2, 4, 1);   // C-E，权重1
    g1.addEdge(2, 7, 3);   // C-H，权重3
    g1.addEdge(3, 4, 13);  // D-E，权重13
    g1.addEdge(3, 6, 2);   // D-G，权重2
    g1.addEdge(4, 5, 5);   // E-F，权重5
    g1.addEdge(4, 6, 11);  // E-G，权重11
    g1.addEdge(4, 7, 8);   // E-H，权重8
    g1.addEdge(5, 7, 10);  // F-H，权重10
    g1.addEdge(6, 4, 14);  // G-E，权重14（注意图是有向图）

    // 输出图1邻接矩阵（对应任务1）
    cout << "==================== 任务1：图1邻接矩阵 ====================" << endl;
    g1.printAdjMatrix();
    cout << endl;
}

// 任务2：图1的A点（编号0）BFS和DFS（对应任务2）
void task2_BFS_DFS(Graph& g1) {
    cout << "==================== 任务2：图1 A点遍历(BFS/DFS) ====================" << endl;
    int startA = 0;  // A点对应编号0
    g1.BFS(startA);                  // BFS遍历
    g1.DFS_Recursive(startA);        // 递归DFS遍历
    g1.DFS_Iterative(startA);        // 迭代DFS遍历
    cout << endl;
}

// 任务3：图1的A点（编号0）最短路径和最小支撑树（对应任务3）
void task3_ShortestPath_MST(Graph& g1) {
    cout << "==================== 任务3：图1 A点(最短路径/MST) ====================" << endl;
    int startA = 0;  // A点对应编号0
    g1.Dijkstra(startA);  // Dijkstra最短路径
    g1.Prim(startA);      // Prim最小支撑树
    cout << endl;
}

// 任务4：图2计算双连通分量和关节点（顶点A~L，编号0=A,...,11=L，对应任务4）
void task4_Graph2_Biconnected() {
    // 创建图2（12个顶点，无向图）
    Graph g2(12, false);
    // 构建图2的边（注意寻找关节点的用例）
    g2.addEdge(0, 1);   // A-B
    g2.addEdge(1, 2);   // B-C
    g2.addEdge(2, 3);   // C-D
    g2.addEdge(3, 4);   // D-E
    g2.addEdge(4, 2);   // E-C（C是潜在关节点）
    g2.addEdge(4, 5);   // E-F
    g2.addEdge(5, 6);   // F-G
    g2.addEdge(6, 7);   // G-H
    g2.addEdge(7, 5);   // H-F（F是潜在关节点）
    g2.addEdge(7, 8);   // H-I
    g2.addEdge(8, 9);   // I-J
    g2.addEdge(9, 10);  // J-K
    g2.addEdge(10, 11); // K-L
    g2.addEdge(11, 8);  // L-I（I是潜在关节点）

    // 不同起点计算关节点（C++98兼容写法，实际Tarjan算法起点不影响结果）
    cout << "==================== 任务4：图2双连通分量和关节点（不同起点） ====================" << endl;
    
    // 起点1：C（编号2）
    cout << "图2 - 起点C（编号2）的关节点：" << endl;
    g2.Tarjan();

    // 起点2：F（编号5）- 重新构建图2
    Graph g2_2(12, false);
    g2_2.addEdge(0, 1);g2_2.addEdge(1, 2);g2_2.addEdge(2, 3);g2_2.addEdge(3, 4);
    g2_2.addEdge(4, 2);g2_2.addEdge(4, 5);g2_2.addEdge(5, 6);g2_2.addEdge(6, 7);
    g2_2.addEdge(7, 5);g2_2.addEdge(7, 8);g2_2.addEdge(8, 9);g2_2.addEdge(9, 10);
    g2_2.addEdge(10, 11);g2_2.addEdge(11, 8);
    cout << "图2 - 起点F（编号5）的关节点：" << endl;
    g2_2.Tarjan();

    // 起点3：I（编号8）- 第三次构建图2
    Graph g2_3(12, false);
    g2_3.addEdge(0, 1);g2_3.addEdge(1, 2);g2_3.addEdge(2, 3);g2_3.addEdge(3, 4);
    g2_3.addEdge(4, 2);g2_3.addEdge(4, 5);g2_3.addEdge(5, 6);g2_3.addEdge(6, 7);
    g2_3.addEdge(7, 5);g2_3.addEdge(7, 8);g2_3.addEdge(8, 9);g2_3.addEdge(9, 10);
    g2_3.addEdge(10, 11);g2_3.addEdge(11, 8);
    cout << "图2 - 起点I（编号8）的关节点：" << endl;
    g2_3.Tarjan();

    cout << "结论：图2不同起点找到的关节点应该一致，因为关节点是图的全局属性，不依赖于遍历起点" << endl;
}

int main() {
    cout << "========== 图算法实验 ==========" << endl << endl;
    
    // 任务1-3：处理图1（8个顶点：A-H）
    Graph graph1(8, true);  // 有向图，8个顶点
    task1_BuildGraph1(graph1);
    task2_BFS_DFS(graph1);
    task3_ShortestPath_MST(graph1);

    // 任务4：处理图2（12个顶点：A-L）
    task4_Graph2_Biconnected();

    return 0;
}
