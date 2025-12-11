#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cassert>
#include <queue>
#include <stack>

// 二叉树节点结构
template <typename T>
struct TreeNode {
    T data;               // 节点数据
    TreeNode* left;       // 左子节点
    TreeNode* right;      // 右子节点
    TreeNode* parent;     // 父节点

    // 构造函数
    TreeNode(const T& val, TreeNode* p = nullptr, 
             TreeNode* l = nullptr, TreeNode* r = nullptr)
        : data(val), parent(p), left(l), right(r) {}
};

// 二叉树类
template <typename T>
class BinaryTree {
protected:
    TreeNode<T>* root;    // 根节点
    int size;             // 节点数量

    // 递归销毁树
    void destroy(TreeNode<T>* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
            size--;
        }
    }

    // 递归复制树
    TreeNode<T>* copy(TreeNode<T>* node, TreeNode<T>* parent) {
        if (!node) return nullptr;
        TreeNode<T>* newNode = new TreeNode<T>(node->data, parent);
        newNode->left = copy(node->left, newNode);
        newNode->right = copy(node->right, newNode);
        return newNode;
    }

    // 递归前序遍历
    void preOrderRecursive(TreeNode<T>* node, void (*visit)(const T&)) const {
        if (node) {
            visit(node->data);
            preOrderRecursive(node->left, visit);
            preOrderRecursive(node->right, visit);
        }
    }

    // 递归中序遍历
    void inOrderRecursive(TreeNode<T>* node, void (*visit)(const T&)) const {
        if (node) {
            inOrderRecursive(node->left, visit);
            visit(node->data);
            inOrderRecursive(node->right, visit);
        }
    }

    // 递归后序遍历
    void postOrderRecursive(TreeNode<T>* node, void (*visit)(const T&)) const {
        if (node) {
            postOrderRecursive(node->left, visit);
            postOrderRecursive(node->right, visit);
            visit(node->data);
        }
    }

public:
    // 构造函数
    BinaryTree() : root(nullptr), size(0) {}

    // 拷贝构造函数
    BinaryTree(const BinaryTree<T>& other) {
        root = copy(other.root, nullptr);
        size = other.size;
    }

    // 析构函数
    ~BinaryTree() {
        clear();
    }

    // 赋值运算符
    BinaryTree<T>& operator=(const BinaryTree<T>& other) {
        if (this != &other) {
            clear();
            root = copy(other.root, nullptr);
            size = other.size;
        }
        return *this;
    }

    // 判断树是否为空
    bool empty() const { return size == 0; }

    // 获取树的大小
    int getSize() const { return size; }

    // 清空树
    void clear() {
        destroy(root);
        root = nullptr;
    }

    // 获取根节点
    TreeNode<T>* getRoot() const { return root; }

    // 前序遍历（递归）
    void preOrder(void (*visit)(const T&)) const {
        preOrderRecursive(root, visit);
    }

    // 中序遍历（递归）
    void inOrder(void (*visit)(const T&)) const {
        inOrderRecursive(root, visit);
    }

    // 后序遍历（递归）
    void postOrder(void (*visit)(const T&)) const {
        postOrderRecursive(root, visit);
    }

    // 层次遍历
    void levelOrder(void (*visit)(const T&)) const {
        if (!root) return;
        
        std::queue<TreeNode<T>*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode<T>* current = q.front();
            q.pop();
            visit(current->data);
            
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    // 前序遍历（迭代）
    void preOrderIterative(void (*visit)(const T&)) const {
        if (!root) return;
        
        std::stack<TreeNode<T>*> s;
        s.push(root);
        
        while (!s.empty()) {
            TreeNode<T>* current = s.top();
            s.pop();
            visit(current->data);
            
            if (current->right) s.push(current->right);
            if (current->left) s.push(current->left);
        }
    }

    // 中序遍历（迭代）
    void inOrderIterative(void (*visit)(const T&)) const {
        std::stack<TreeNode<T>*> s;
        TreeNode<T>* current = root;
        
        while (current || !s.empty()) {
            while (current) {
                s.push(current);
                current = current->left;
            }
            
            current = s.top();
            s.pop();
            visit(current->data);
            current = current->right;
        }
    }

    // 后序遍历（迭代）
    void postOrderIterative(void (*visit)(const T&)) const {
        if (!root) return;
        
        std::stack<TreeNode<T>*> s1, s2;
        s1.push(root);
        
        while (!s1.empty()) {
            TreeNode<T>* current = s1.top();
            s1.pop();
            s2.push(current);
            
            if (current->left) s1.push(current->left);
            if (current->right) s1.push(current->right);
        }
        
        while (!s2.empty()) {
            TreeNode<T>* current = s2.top();
            s2.pop();
            visit(current->data);
        }
    }

    // 获取节点的深度
    int depth(TreeNode<T>* node) const {
        if (!node) return -1;
        return 1 + std::max(depth(node->left), depth(node->right));
    }

    // 获取树的深度
    int depth() const {
        return depth(root);
    }
};

// 二叉搜索树类（继承自二叉树）
template <typename T>
class BST : public BinaryTree<T> {
public:
    // 查找元素
    TreeNode<T>* search(const T& val) const {
        TreeNode<T>* current = this->root;
        while (current) {
            if (val == current->data) return current;
            else if (val < current->data) current = current->left;
            else current = current->right;
        }
        return nullptr;
    }

    // 插入元素
    TreeNode<T>* insert(const T& val) {
        TreeNode<T>* parent = nullptr;
        TreeNode<T>* current = this->root;
        
        // 查找插入位置
        while (current) {
            parent = current;
            if (val == current->data) return nullptr;  // 不允许重复元素
            else if (val < current->data) current = current->left;
            else current = current->right;
        }
        
        // 创建新节点
        TreeNode<T>* newNode = new TreeNode<T>(val, parent);
        if (!parent) this->root = newNode;  // 树为空
        else if (val < parent->data) parent->left = newNode;
        else parent->right = newNode;
        
        this->size++;
        return newNode;
    }

    // 删除元素
    bool remove(const T& val) {
        TreeNode<T>* node = search(val);
        if (!node) return false;
        
        removeNode(node);
        return true;
    }

private:
    // 删除指定节点
    void removeNode(TreeNode<T>* node) {
        if (!node) return;
        
        // 情况1：节点有两个子节点
        if (node->left && node->right) {
            // 找到中序后继（右子树的最左节点）
            TreeNode<T>* successor = node->right;
            while (successor->left) {
                successor = successor->left;
            }
            
            // 复制后继节点的数据
            node->data = successor->data;
            
            // 转化为删除后继节点（最多有一个子节点）
            node = successor;
        }
        
        // 情况2：节点最多有一个子节点
        TreeNode<T>* child = (node->left) ? node->left : node->right;
        
        if (child) {
            child->parent = node->parent;  // 更新子节点的父指针
        }
        
        // 更新父节点的子指针
        if (node->parent) {
            if (node == node->parent->left) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
        } else {
            // 如果是根节点
            this->root = child;
        }
        
        // 删除节点
        delete node;
        this->size--;
    }
};

#endif // TREE_H
    