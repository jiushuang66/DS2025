#ifndef STACK_H
#define STACK_H

#include <cassert>
#include <algorithm>

template <typename T>
class Stack {
private:
    T* _data;       // 存储元素的动态数组
    int _size;      // 当前元素数量
    int _capacity;  // 容量

    // 扩容函数，确保有足够空间存储新元素
    void expand() {
        if (_size < _capacity) return;  // 无需扩容
        _capacity = (_capacity == 0) ? 1 : _capacity * 2;  // 初始容量为1，之后翻倍
        T* newData = new T[_capacity];
        for (int i = 0; i < _size; ++i) {
            newData[i] = _data[i];  // 复制元素
        }
        delete[] _data;  // 释放旧空间
        _data = newData;
    }

public:
    // 构造函数
    Stack() : _data(nullptr), _size(0), _capacity(0) {}

    // 拷贝构造函数
    Stack(const Stack<T>& other) : _size(other._size), _capacity(other._capacity) {
        _data = new T[_capacity];
        for (int i = 0; i < _size; ++i) {
            _data[i] = other._data[i];
        }
    }

    // 析构函数
    ~Stack() {
        delete[] _data;
    }

    // 赋值运算符
    Stack<T>& operator=(const Stack<T>& other) {
        if (this != &other) {  // 避免自赋值
            delete[] _data;    // 释放当前资源
            _size = other._size;
            _capacity = other._capacity;
            _data = new T[_capacity];
            for (int i = 0; i < _size; ++i) {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }

    // 入栈操作
    void push(const T& element) {
        expand();               // 确保容量足够
        _data[_size++] = element;  // 存储元素并更新大小
    }

    // 出栈操作，返回栈顶元素
    T pop() {
        assert(_size > 0 && "Stack is empty!");  // 检查栈非空
        return _data[--_size];  // 更新大小并返回元素
    }

    // 获取栈顶元素（不删除）
    T& top() {
        assert(_size > 0 && "Stack is empty!");
        return _data[_size - 1];
    }

    const T& top() const {
        assert(_size > 0 && "Stack is empty!");
        return _data[_size - 1];
    }

    // 清空栈
    void clear() {
        _size = 0;  // 逻辑清空，不释放内存
    }

    // 判断栈是否为空
    bool empty() const {
        return _size == 0;
    }

    // 获取栈的大小
    int size() const {
        return _size;
    }

    // 交换两个栈的内容
    void swap(Stack<T>& other) {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }

    // 比较两个栈是否相等
    bool operator==(const Stack<T>& other) const {
        if (_size != other._size) return false;
        for (int i = 0; i < _size; ++i) {
            if (_data[i] != other._data[i]) return false;
        }
        return true;
    }

    // 比较两个栈是否不相等
    bool operator!=(const Stack<T>& other) const {
        return !(*this == other);
    }
};

#endif // STACK_H
