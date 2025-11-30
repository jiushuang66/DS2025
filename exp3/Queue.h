#ifndef QUEUE_H
#define QUEUE_H

#include <cassert>
#include <algorithm>

template <typename T>
class Queue {
private:
    T* _data;       // 存储元素的数组
    int _front;     // 队头索引
    int _rear;      // 队尾索引
    int _size;      // 当前元素数量
    int _capacity;  // 队列容量

    // 扩容函数
    void expand() {
        if (_size < _capacity) return;
        
        int newCapacity = _capacity * 2;
        T* newData = new T[newCapacity];
        
        // 复制元素
        for (int i = 0; i < _size; ++i) {
            newData[i] = _data[(_front + i) % _capacity];
        }
        
        // 更新成员变量
        delete[] _data;
        _data = newData;
        _front = 0;
        _rear = _size;
        _capacity = newCapacity;
    }

public:
    // 构造函数
    Queue(int capacity = 4) : _capacity(capacity > 0 ? capacity : 4) {
        _data = new T[_capacity];
        _front = 0;
        _rear = 0;
        _size = 0;
    }

    // 拷贝构造函数
    Queue(const Queue& other) {
        _capacity = other._capacity;
        _size = other._size;
        _data = new T[_capacity];
        
        for (int i = 0; i < _size; ++i) {
            _data[i] = other._data[(other._front + i) % other._capacity];
        }
        
        _front = 0;
        _rear = _size;
    }

    // 赋值运算符
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            delete[] _data;
            
            _capacity = other._capacity;
            _size = other._size;
            _data = new T[_capacity];
            
            for (int i = 0; i < _size; ++i) {
                _data[i] = other._data[(other._front + i) % other._capacity];
            }
            
            _front = 0;
            _rear = _size;
        }
        return *this;
    }

    // 析构函数
    ~Queue() {
        delete[] _data;
    }

    // 入队操作
    void enqueue(const T& e) {
        expand();  // 自动扩容
        _data[_rear] = e;
        _rear = (_rear + 1) % _capacity;
        _size++;
    }

    // 出队操作
    T dequeue() {
        assert(!empty() && "Queue is empty!");
        
        T e = _data[_front];
        _front = (_front + 1) % _capacity;
        _size--;
        return e;
    }

    // 获取队头元素
    T& front() {
        assert(!empty() && "Queue is empty!");
        return _data[_front];
    }

    const T& front() const {
        assert(!empty() && "Queue is empty!");
        return _data[_front];
    }

    // 获取队尾元素
    T& back() {
        assert(!empty() && "Queue is empty!");
        int backIndex = (_rear - 1 + _capacity) % _capacity;
        return _data[backIndex];
    }

    const T& back() const {
        assert(!empty() && "Queue is empty!");
        int backIndex = (_rear - 1 + _capacity) % _capacity;
        return _data[backIndex];
    }

    // 判断队列是否为空
    bool empty() const {
        return _size == 0;
    }

    // 获取队列大小
    int size() const {
        return _size;
    }

    // 清空队列
    void clear() {
        _front = 0;
        _rear = 0;
        _size = 0;
    }

    // 交换两个队列的内容
    void swap(Queue& other) {
        std::swap(_data, other._data);
        std::swap(_front, other._front);
        std::swap(_rear, other._rear);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }

    // 比较运算符
    bool operator==(const Queue& other) const {
        if (_size != other._size) return false;
        
        for (int i = 0; i < _size; ++i) {
            int thisIndex = (_front + i) % _capacity;
            int otherIndex = (other._front + i) % other._capacity;
            if (_data[thisIndex] != other._data[otherIndex]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Queue& other) const {
        return !(*this == other);
    }
};

#endif // QUEUE_H
    