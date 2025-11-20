#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <cstdlib>
using namespace std;

template <typename T>
struct ListNode {
    T data;
    ListNode<T>* prev;
    ListNode<T>* next;
    
    // 构造函数
    ListNode() : prev(nullptr), next(nullptr) {}
    ListNode(T e, ListNode<T>* p = nullptr, ListNode<T>* n = nullptr)
        : data(e), prev(p), next(n) {}
    
    // 插入节点
    ListNode<T>* insertAsPrev(const T& e) {
        ListNode<T>* node = new ListNode<T>(e, prev, this);
        prev->next = node;
        prev = node;
        return node;
    }
    
    ListNode<T>* insertAsNext(const T& e) {
        ListNode<T>* node = new ListNode<T>(e, this, next);
        next->prev = node;
        next = node;
        return node;
    }
};

template <typename T>
class List {
private:
    int _size;           // 规模
    ListNode<T>* header; // 头哨兵
    ListNode<T>* trailer;// 尾哨兵

protected:
    // 清除所有节点
    void clear();
    // 复制列表L中自位置p起的n个元素
    void copyNodes(ListNode<T>* p, int n);
    // 选择排序
    void selectionSort(ListNode<T>* p, int n);
    // 插入排序
    void insertionSort(ListNode<T>* p, int n);

public:
    // 构造函数
    List();
    List(const List<T>& L);
    List(const List<T>& L, int r, int n);
    List(ListNode<T>* p, int n);
    
    // 析构函数
    ~List();
    
    // 只读访问接口
    int size() const { return _size; }
    bool empty() const { return _size == 0; }
    T& operator[](int r) const;
    ListNode<T>* first() const { return header->next; }
    ListNode<T>* last() const { return trailer->prev; }
    bool valid(ListNode<T>* p) {
        return p && (trailer != p) && (header != p);
    }
    
    // 查找
    ListNode<T>* find(const T& e) const {
        return find(e, _size, trailer);
    }
    ListNode<T>* find(const T& e, int n, ListNode<T>* p) const;
    
    // 插入
    ListNode<T>* insertAsFirst(const T& e);
    ListNode<T>* insertAsLast(const T& e);
    ListNode<T>* insertBefore(ListNode<T>* p, const T& e);
    ListNode<T>* insertAfter(ListNode<T>* p, const T& e);
    
    // 删除
    T remove(ListNode<T>* p);
    
    // 唯一化
    int deduplicate();
    int uniquify();
    
    // 遍历
    void traverse(void (*visit)(T&));
    template <typename VST> void traverse(VST& visit);
    
    // 排序
    void sort(ListNode<T>* p, int n);
    void sort() { sort(first(), _size); }
};

// 构造函数实现
template <typename T>
List<T>::List() {
    _size = 0;
    header = new ListNode<T>();
    trailer = new ListNode<T>();
    header->next = trailer;
    trailer->prev = header;
}

template <typename T>
List<T>::List(const List<T>& L) {
    copyNodes(L.first(), L._size);
}

template <typename T>
List<T>::List(const List<T>& L, int r, int n) {
    ListNode<T>* p = L.first();
    for (int i = 0; i < r; i++) p = p->next;
    copyNodes(p, n);
}

template <typename T>
List<T>::List(ListNode<T>* p, int n) {
    copyNodes(p, n);
}

// 析构函数
template <typename T>
List<T>::~List() {
    clear();
    delete header;
    delete trailer;
}

// 复制节点
template <typename T>
void List<T>::copyNodes(ListNode<T>* p, int n) {
    header = new ListNode<T>();
    trailer = new ListNode<T>();
    header->next = trailer;
    trailer->prev = header;
    _size = 0;
    
    for (int i = 0; i < n; i++) {
        insertAsLast(p->data);
        p = p->next;
    }
}

// 清除所有节点
template <typename T>
void List<T>::clear() {
    ListNode<T>* p = header->next;
    while (p != trailer) {
        ListNode<T>* temp = p;
        p = p->next;
        delete temp;
    }
    header->next = trailer;
    trailer->prev = header;
    _size = 0;
}

// 重载[]操作符
template <typename T>
T& List<T>::operator[](int r) const {
    ListNode<T>* p = first();
    for (int i = 0; i < r; i++) p = p->next;
    return p->data;
}

// 查找
template <typename T>
ListNode<T>* List<T>::find(const T& e, int n, ListNode<T>* p) const {
    while (n-- > 0 && p != header) {
        p = p->prev;
        if (p->data == e) return p;
    }
    return nullptr;
}

// 插入操作
template <typename T>
ListNode<T>* List<T>::insertAsFirst(const T& e) {
    _size++;
    return header->insertAsNext(e);
}

template <typename T>
ListNode<T>* List<T>::insertAsLast(const T& e) {
    _size++;
    return trailer->insertAsPrev(e);
}

template <typename T>
ListNode<T>* List<T>::insertBefore(ListNode<T>* p, const T& e) {
    _size++;
    return p->insertAsPrev(e);
}

template <typename T>
ListNode<T>* List<T>::insertAfter(ListNode<T>* p, const T& e) {
    _size++;
    return p->insertAsNext(e);
}

// 删除操作
template <typename T>
T List<T>::remove(ListNode<T>* p) {
    T e = p->data;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;
    _size--;
    return e;
}

// 去重
template <typename T>
int List<T>::deduplicate() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = first();
    int r = 0;
    
    while (p != trailer) {
        ListNode<T>* q = find(p->data, r, p);
        if (q) remove(q);
        else r++;
        p = p->next;
    }
    return oldSize - _size;
}

// 有序列表去重
template <typename T>
int List<T>::uniquify() {
    if (_size < 2) return 0;
    int oldSize = _size;
    ListNode<T>* p = first();
    ListNode<T>* q;
    
    while ((q = p->next) != trailer) {
        if (p->data != q->data)
            p = q;
        else
            remove(q);
    }
    return oldSize - _size;
}

// 遍历
template <typename T>
void List<T>::traverse(void (*visit)(T&)) {
    for (ListNode<T>* p = first(); p != trailer; p = p->next)
        visit(p->data);
}

template <typename T>
template <typename VST>
void List<T>::traverse(VST& visit) {
    for (ListNode<T>* p = first(); p != trailer; p = p->next)
        visit(p->data);
}

// 选择排序
template <typename T>
void List<T>::selectionSort(ListNode<T>* p, int n) {
    ListNode<T>* head = p->prev;
    ListNode<T>* tail = p;
    
    // 移动tail到待排序区间的末尾
    for (int i = 0; i < n; i++) tail = tail->next;
    
    while (n > 1) {
        // 找出最大元素
        ListNode<T>* max = head->next;
        ListNode<T>* curr = head->next;
        
        for (int i = 0; i < n; i++) {
            if (curr->data > max->data)
                max = curr;
            curr = curr->next;
        }
        
        // 将最大元素移到tail之前
        ListNode<T>* prev = max->prev;
        ListNode<T>* next = max->next;
        
        prev->next = next;
        next->prev = prev;
        
        max->prev = tail->prev;
        max->next = tail;
        tail->prev->next = max;
        tail->prev = max;
        
        tail = max;
        n--;
    }
}

// 插入排序
template <typename T>
void List<T>::insertionSort(ListNode<T>* p, int n) {
    for (int i = 0; i < n; i++) {
        insertBefore(search(p->data, i, p), p->data);
        p = p->next;
        remove(p->prev);
    }
}

// 排序接口
template <typename T>
void List<T>::sort(ListNode<T>* p, int n) {
    // 使用选择排序替代归并排序，避免潜在问题
    selectionSort(p, n);
}

#endif // LIST_H
