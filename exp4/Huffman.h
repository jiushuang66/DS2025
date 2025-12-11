#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "Tree.h"
#include "Bitmap.h"
#include "Queue.h"
#include "Vector.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

// Huffman 树节点
template <typename T>
struct HuffmanNode {
    T data;           // 字符
    int weight;       // 权重
    HuffmanNode<T>* left;
    HuffmanNode<T>* right;
    bool isLeaf;      // 标记是否为叶子节点
    
    HuffmanNode(T d, int w) : data(d), weight(w), left(nullptr), right(nullptr), isLeaf(true) {}
    HuffmanNode(int w) : data(T()), weight(w), left(nullptr), right(nullptr), isLeaf(false) {}
    
    // 用于比较的运算符重载
    bool operator>(const HuffmanNode<T>& other) const {
        return weight > other.weight;
    }
};

// Huffman 编码表项
template <typename T>
struct HuffmanCode {
    T character;
    std::string code;
    int frequency;
    
    HuffmanCode() : character(T()), code(""), frequency(0) {}
    HuffmanCode(T ch, const std::string& cd, int freq) 
        : character(ch), code(cd), frequency(freq) {}
};

// 用于优先队列的比较函数
template <typename T>
struct CompareHuffmanNode {
    bool operator()(const HuffmanNode<T>* a, const HuffmanNode<T>* b) const {
        return a->weight > b->weight; // 最小堆
    }
};

// Huffman 树类
template <typename T>
class HuffmanTree {
private:
    HuffmanNode<T>* root;
    Vector<HuffmanCode<T>> codeTable;
    
    // 递归生成编码
    void generateCodes(HuffmanNode<T>* node, const std::string& currentCode) {
        if (!node) return;
        
        if (node->isLeaf) {
            // 叶子节点，存储编码
            codeTable.push_back(HuffmanCode<T>(node->data, currentCode, node->weight));
            return;
        }
        
        if (node->left) {
            generateCodes(node->left, currentCode + "0");
        }
        if (node->right) {
            generateCodes(node->right, currentCode + "1");
        }
    }
    
    // 递归销毁树
    void destroy(HuffmanNode<T>* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }
    
    // 递归复制树
    HuffmanNode<T>* copy(HuffmanNode<T>* node) {
        if (!node) return nullptr;
        HuffmanNode<T>* newNode = nullptr;
        if (node->isLeaf) {
            newNode = new HuffmanNode<T>(node->data, node->weight);
        } else {
            newNode = new HuffmanNode<T>(node->weight);
        }
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

    // 清空编码表
    void clearCodeTable() {
        Vector<HuffmanCode<T>> newTable;
        codeTable = newTable;
    }

    // 构建最小堆（使用简单排序实现）
    void buildMinHeap(Vector<HuffmanNode<T>*>& nodes) {
        for (int i = 0; i < nodes.size() - 1; i++) {
            for (int j = i + 1; j < nodes.size(); j++) {
                if (nodes[i]->weight > nodes[j]->weight) {
                    HuffmanNode<T>* temp = nodes[i];
                    nodes[i] = nodes[j];
                    nodes[j] = temp;
                }
            }
        }
    }

    // 从堆中提取最小节点
    HuffmanNode<T>* extractMin(Vector<HuffmanNode<T>*>& nodes) {
        if (nodes.empty()) return nullptr;
        HuffmanNode<T>* minNode = nodes[0];
        for (int i = 0; i < nodes.size() - 1; i++) {
            nodes[i] = nodes[i + 1];
        }
        // 使用 erase 方法删除最后一个元素
        Vector<HuffmanNode<T>*> newNodes;
        for (int i = 0; i < nodes.size() - 1; i++) {
            newNodes.push_back(nodes[i]);
        }
        nodes = newNodes;
        return minNode;
    }

    // 插入节点到堆中并保持堆性质
    void insertToHeap(Vector<HuffmanNode<T>*>& nodes, HuffmanNode<T>* node) {
        nodes.push_back(node);
        // 重新构建堆
        buildMinHeap(nodes);
    }

public:
    HuffmanTree() : root(nullptr) {}
    
    HuffmanTree(const HuffmanTree& other) {
        root = copy(other.root);
        codeTable = other.codeTable;
    }
    
    ~HuffmanTree() {
        destroy(root);
    }
    
    HuffmanTree& operator=(const HuffmanTree& other) {
        if (this != &other) {
            destroy(root);
            root = copy(other.root);
            codeTable = other.codeTable;
        }
        return *this;
    }
    
    // 构建 Huffman 树
    void build(const Vector<T>& characters, const Vector<int>& frequencies) {
        if (characters.size() != frequencies.size() || characters.empty()) {
            std::cout << "Error: Invalid input for Huffman tree building" << std::endl;
            return;
        }
        
        // 如果已有树，先销毁
        if (root) {
            destroy(root);
            root = nullptr;
        }
        
        // 清空编码表
        clearCodeTable();
        
        // 创建初始节点列表
        Vector<HuffmanNode<T>*> nodes;
        for (int i = 0; i < characters.size(); i++) {
            nodes.push_back(new HuffmanNode<T>(characters[i], frequencies[i]));
        }
        
        // 显示初始节点信息
        std::cout << "Building Huffman tree with " << nodes.size() << " characters" << std::endl;
        
        // 构建最小堆
        buildMinHeap(nodes);
        
        // 构建 Huffman 树
        while (nodes.size() > 1) {
            // 提取两个最小权重的节点
            HuffmanNode<T>* left = extractMin(nodes);
            HuffmanNode<T>* right = extractMin(nodes);
            
            if (!left || !right) break;
            
            // 创建新节点
            HuffmanNode<T>* parent = new HuffmanNode<T>(left->weight + right->weight);
            parent->left = left;
            parent->right = right;
            parent->isLeaf = false;
            
            // 将新节点插入堆中
            insertToHeap(nodes, parent);
        }
        
        if (!nodes.empty()) {
            root = nodes[0];
            std::cout << "Huffman tree built successfully!" << std::endl;
        } else {
            std::cout << "Error: Failed to build Huffman tree" << std::endl;
        }
        
        // 生成编码表
        generateCodes(root, "");
        std::cout << "Generated " << codeTable.size() << " codes" << std::endl;
    }
    
    // 获取编码表
    const Vector<HuffmanCode<T>>& getCodeTable() const {
        return codeTable;
    }
    
    // 获取字符的编码
    std::string getCode(T character) const {
        for (int i = 0; i < codeTable.size(); i++) {
            if (codeTable[i].character == character) {
                return codeTable[i].code;
            }
        }
        std::cout << "Warning: No code found for character '" << character << "'" << std::endl;
        return "";
    }
    
    // 编码字符串
    std::string encode(const std::string& text) const {
        if (codeTable.size() == 0) {
            std::cout << "Error: Code table is empty" << std::endl;
            return "";
        }
        
        std::string result;
        for (char c : text) {
            // 只处理字母字符，转换为小写
            if (std::isalpha(c)) {
                T character = std::tolower(c);
                std::string code = getCode(character);
                if (!code.empty()) {
                    result += code;
                } else {
                    std::cout << "Warning: Skipping character '" << c << "' with no code" << std::endl;
                }
            }
        }
        return result;
    }
    
    // 解码
    std::string decode(const std::string& encoded) const {
        if (!root) {
            std::cout << "Error: Huffman tree is empty" << std::endl;
            return "";
        }
        
        std::string result;
        HuffmanNode<T>* current = root;
        
        for (char bit : encoded) {
            if (bit == '0') {
                current = current->left;
            } else if (bit == '1') {
                current = current->right;
            } else {
                // 跳过非0/1字符
                continue;
            }
            
            if (!current) {
                std::cout << "Error: Invalid code sequence" << std::endl;
                break;
            }
            
            if (current->isLeaf) {
                result += current->data;
                current = root;
            }
        }
        
        // 检查是否在中间节点结束
        if (current != root && !current->isLeaf) {
            std::cout << "Warning: Incomplete code at the end" << std::endl;
        }
        
        return result;
    }
    
    // 显示编码表
    void displayCodeTable() const {
        std::cout << "Huffman Code Table:" << std::endl;
        std::cout << "Char\tFreq\tCode" << std::endl;
        std::cout << "----\t----\t----" << std::endl;
        for (int i = 0; i < codeTable.size(); i++) {
            std::cout << codeTable[i].character << "\t" 
                      << codeTable[i].frequency << "\t" 
                      << codeTable[i].code << std::endl;
        }
    }
    
    // 获取树的高度
    int getHeight() const {
        return calculateHeight(root);
    }
    
private:
    int calculateHeight(HuffmanNode<T>* node) const {
        if (!node) return -1;
        return 1 + std::max(calculateHeight(node->left), calculateHeight(node->right));
    }
};

// 文本分析函数
class TextAnalyzer {
public:
    static void analyzeText(const std::string& text, Vector<char>& characters, Vector<int>& frequencies) {
        int count[26] = {0};
        
        // 统计字母频率（不分大小写）
        for (char c : text) {
            if (std::isalpha(c)) {
                char lowerChar = std::tolower(c);
                count[lowerChar - 'a']++;
            }
        }
        
        // 构建字符和频率向量
        for (int i = 0; i < 26; i++) {
            if (count[i] > 0) {
                characters.push_back('a' + i);
                frequencies.push_back(count[i]);
            }
        }
    }
    
    // 显示文本统计信息
    static void displayTextStats(const std::string& text) {
        int totalChars = 0;
        int letterCount[26] = {0};
        
        for (char c : text) {
            if (std::isalpha(c)) {
                char lowerChar = std::tolower(c);
                letterCount[lowerChar - 'a']++;
                totalChars++;
            }
        }
        
        std::cout << "Text Statistics:" << std::endl;
        std::cout << "Total letters: " << totalChars << std::endl;
        std::cout << "Unique letters: ";
        int uniqueCount = 0;
        for (int i = 0; i < 26; i++) {
            if (letterCount[i] > 0) {
                uniqueCount++;
            }
        }
        std::cout << uniqueCount << std::endl;
    }
};

#endif // HUFFMAN_H
