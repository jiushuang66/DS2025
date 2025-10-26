#ifndef STACK_H
#define STACK_H

#include <cassert>
#include <algorithm>

template <typename T>
class Stack {
private:
    T* _data;       // 瀛樺偍鍏冪礌鐨勫姩鎬佹暟缁?
    int _size;      // 褰撳墠鍏冪礌鏁伴噺
    int _capacity;  // 瀹归噺

    // 鎵╁鍑芥暟锛岀‘淇濇湁瓒冲绌洪棿瀛樺偍鏂板厓绱?
    void expand() {
        if (_size < _capacity) return;  // 鏃犻渶鎵╁
        _capacity = (_capacity == 0) ? 1 : _capacity * 2;  // 鍒濆瀹归噺涓?锛屼箣鍚庣炕鍊?
        T* newData = new T[_capacity];
        for (int i = 0; i < _size; ++i) {
            newData[i] = _data[i];  // 澶嶅埗鍏冪礌
        }
        delete[] _data;  // 閲婃斁鏃х┖闂?
        _data = newData;
    }

public:
    // 鏋勯€犲嚱鏁?
    Stack() : _data(nullptr), _size(0), _capacity(0) {}

    // 鎷疯礉鏋勯€犲嚱鏁?
    Stack(const Stack<T>& other) : _size(other._size), _capacity(other._capacity) {
        _data = new T[_capacity];
        for (int i = 0; i < _size; ++i) {
            _data[i] = other._data[i];
        }
    }

    // 鏋愭瀯鍑芥暟
    ~Stack() {
        delete[] _data;
    }

    // 璧嬪€艰繍绠楃
    Stack<T>& operator=(const Stack<T>& other) {
        if (this != &other) {  // 閬垮厤鑷祴鍊?
            delete[] _data;    // 閲婃斁褰撳墠璧勬簮
            _size = other._size;
            _capacity = other._capacity;
            _data = new T[_capacity];
            for (int i = 0; i < _size; ++i) {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }

    // 鍏ユ爤鎿嶄綔
    void push(const T& element) {
        expand();               // 纭繚瀹归噺瓒冲
        _data[_size++] = element;  // 瀛樺偍鍏冪礌骞舵洿鏂板ぇ灏?
    }

    // 鍑烘爤鎿嶄綔锛岃繑鍥炴爤椤跺厓绱?
    T pop() {
        assert(_size > 0 && "Stack is empty!");  // 妫€鏌ユ爤闈炵┖
        return _data[--_size];  // 鏇存柊澶у皬骞惰繑鍥炲厓绱?
    }

    // 鑾峰彇鏍堥《鍏冪礌锛堜笉鍒犻櫎锛?
    T& top() {
        assert(_size > 0 && "Stack is empty!");
        return _data[_size - 1];
    }

    const T& top() const {
        assert(_size > 0 && "Stack is empty!");
        return _data[_size - 1];
    }

    // 娓呯┖鏍?
    void clear() {
        _size = 0;  // 閫昏緫娓呯┖锛屼笉閲婃斁鍐呭瓨
    }

    // 鍒ゆ柇鏍堟槸鍚︿负绌?
    bool empty() const {
        return _size == 0;
    }

    // 鑾峰彇鏍堢殑澶у皬
    int size() const {
        return _size;
    }

    // 浜ゆ崲涓や釜鏍堢殑鍐呭
    void swap(Stack<T>& other) {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }

    // 姣旇緝涓や釜鏍堟槸鍚︾浉绛?
    bool operator==(const Stack<T>& other) const {
        if (_size != other._size) return false;
        for (int i = 0; i < _size; ++i) {
            if (_data[i] != other._data[i]) return false;
        }
        return true;
    }

    // 姣旇緝涓や釜鏍堟槸鍚︿笉鐩哥瓑
    bool operator!=(const Stack<T>& other) const {
        return !(*this == other);
    }
    
    // 原位构造元素
    template<typename... Args>
    void emplace(Args&&... args) {
        expand();
        _data[_size++] = T(std::forward<Args>(args)...);  // 原位构造
    }
};

#endif // STACK_H
