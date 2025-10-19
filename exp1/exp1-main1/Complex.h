#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

template <typename T>
class Complex {
private:
    T real;  // 实部
    T imag;  // 虚部

public:
    // 构造函数
    Complex(T r = 0, T i = 0) : real(r), imag(i) {}

    // 拷贝构造函数
    Complex(const Complex<T>& other) : real(other.real), imag(other.imag) {}

    // 获取实部和虚部
    T getReal() const { return real; }
    T getImag() const { return imag; }

    // 设置实部和虚部
    void setReal(T r) { real = r; }
    void setImag(T i) { imag = i; }

    // 计算模长
    double mod() const {
        return std::sqrt(static_cast<double>(real * real + imag * imag));
    }

    // 计算幅角（弧度制）
    double arg() const {
        return std::atan2(static_cast<double>(imag), static_cast<double>(real));
    }

    // 重载赋值运算符
    Complex<T>& operator=(const Complex<T>& other) {
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    // 算术运算符重载
    Complex<T> operator+(const Complex<T>& other) const {
        return Complex<T>(real + other.real, imag + other.imag);
    }

    Complex<T> operator-(const Complex<T>& other) const {
        return Complex<T>(real - other.real, imag - other.imag);
    }

    Complex<T> operator*(const Complex<T>& other) const {
        return Complex<T>(
            real * other.real - imag * other.imag,
            imag * other.real + real * other.imag
        );
    }

    Complex<T> operator/(const Complex<T>& other) const {
        T denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0) {
            return Complex<T>(0, 0);
        }
        return Complex<T>(
            (real * other.real + imag * other.imag) / denominator,
            (imag * other.real - real * other.imag) / denominator
        );
    }

    // 复合赋值运算符
    Complex<T>& operator+=(const Complex<T>& other) {
        *this = *this + other;
        return *this;
    }

    Complex<T>& operator-=(const Complex<T>& other) {
        *this = *this - other;
        return *this;
    }

    Complex<T>& operator*=(const Complex<T>& other) {
        *this = *this * other;
        return *this;
    }

    Complex<T>& operator/=(const Complex<T>& other) {
        *this = *this / other;
        return *this;
    }

    // 比较运算符
    bool operator==(const Complex<T>& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    bool operator!=(const Complex<T>& other) const {
        return !(*this == other);
    }

    bool operator<(const Complex<T>& other) const {
        double modThis = mod();
        double modOther = other.mod();
        if (modThis != modOther) {
            return modThis < modOther;
        }
        return real < other.real;
    }

    bool operator>(const Complex<T>& other) const {
        return other < *this;
    }

    bool operator<=(const Complex<T>& other) const {
        return !(*this > other);
    }

    bool operator>=(const Complex<T>& other) const {
        return !(*this < other);
    }

    // 友元声明
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Complex<U>& c);
    
    template <typename U>
    friend std::istream& operator>>(std::istream& is, Complex<U>& c);
};

// 输出运算符重载
template <typename T>
std::ostream& operator<<(std::ostream& os, const Complex<T>& c) {
    os << "(" << c.real << ", " << c.imag << ")";
    return os;
}

// 输入运算符重载
template <typename T>
std::istream& operator>>(std::istream& is, Complex<T>& c) {
    T r, i;
    char ch;
    
    // 处理输入格式，允许空格和不同括号格式
    is >> ch;  // 读取'('
    if (ch != '(') is.setstate(std::ios::failbit);
    
    is >> r;   // 读取实部
    is >> ch;  // 读取','
    if (ch != ',') is.setstate(std::ios::failbit);
    
    is >> i;   // 读取虚部
    is >> ch;  // 读取')'
    if (ch != ')') is.setstate(std::ios::failbit);
    
    if (is) {  // 输入成功才更新值
        c.real = r;
        c.imag = i;
    }
    return is;
}

#endif // COMPLEX_H
