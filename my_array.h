/* 朱泽凯 2051995 济美 */

#ifndef MY_ARRAY_HEHEPIG
#define MY_ARRAY_HEHEPIG

#include <iostream>

template<typename T>
class my_iterator {

protected:
    T *pointer;

public:
    my_iterator() :pointer(NULL) {};
    my_iterator(T *p) :pointer(p) {};

    T &operator*()
    {
        return *pointer;
    }

    my_iterator &operator++(int)
    {
        pointer++;
        return *this;
    }

    my_iterator operator++()
    {
        my_iterator tmp = *this;
        ++ this->pointer;
        return tmp;
    }

    bool operator!=(const my_iterator X) const
    {
        return this->pointer != X.pointer;
    }

};


template<typename T>
class my_reverse_iterator {

protected:
    T *pointer;

public:
    my_reverse_iterator() :pointer(NULL) {};
    my_reverse_iterator(T *p) :pointer(p) {};

    T &operator*()
    {
        return *pointer;
    }

    my_reverse_iterator &operator++(int)
    {
        pointer--;
        return *this;
    }

    my_reverse_iterator operator++()
    {
        my_reverse_iterator tmp = *this;
        -- this->pointer;
        return tmp;
    }

    bool operator!=(const my_reverse_iterator X) const
    {
        return this->pointer != X.pointer;
    }
};


template<typename T>
class my_const_iterator {

protected:
    const T *pointer;

public:
    my_const_iterator() :pointer(NULL) {};
    my_const_iterator(const T *p) :pointer(p) {};

    const T &operator*() const
    {
        return *pointer;
    }

    my_const_iterator &operator++(int)
    {
        pointer++;
        return *this;
    }

    my_const_iterator operator++()
    {
        my_const_iterator tmp = *this;
        ++ this->pointer;
        return tmp;
    }

    bool operator!=(const my_const_iterator X) const
    {
        return this->pointer != X.pointer;
    }
};


template<typename T>
class my_const_reverse_iterator {

protected:
    const T *pointer;

public:
    my_const_reverse_iterator() :pointer(NULL) {};
    my_const_reverse_iterator(const T *p) :pointer(p) {};

    const T &operator*() const
    {
        return *pointer;
    }

    my_const_reverse_iterator &operator++(int)
    {
        pointer--;
        return *this;
    }

    my_const_reverse_iterator operator++()
    {
        my_const_reverse_iterator tmp = *this;
        -- this->pointer;
        return tmp;
    }

    bool operator!=(const my_const_reverse_iterator X) const
    {
        return this->pointer != X.pointer;
    }
};









#include <string>
using namespace std;







//静态数组
template<typename T, int N>
class my_array {

protected:

#if defined(__linux) || defined(__linux__)
    T D[N];
#else
    T D[N > 0 ? N : 1];     //防止空数组
#endif

    int DEF(int x)
    {
        return 0;
    }
    double DEF(double x)
    {
        return 0.0;
    }
    string DEF(string x)
    {
        return string();
    }

public:

    typedef my_iterator<T>                  iterator;
    typedef my_const_iterator<T>            const_iterator;
    typedef my_reverse_iterator<T>          reverse_iterator;
    typedef my_const_reverse_iterator<T>    const_reverse_iterator;

    my_array() {}
    my_array(std::initializer_list<T> X)
    {

        int i = 0;
        for (auto tmp : X) {
            D[i++] = tmp;
        }

        while (i < N) {
            D[i++] = DEF(T());
        }
    }

    iterator begin()
    {
        return D;
    }

    iterator end()
    {
        return D + N;
    }

    reverse_iterator rbegin()
    {
        return D + N - 1;
    }

    reverse_iterator rend()
    {
        return D - 1;
    }

    const_iterator cbegin() const
    {
        return D;
    }

    const_iterator cend() const
    {
        return D + N;
    }

    const_reverse_iterator crbegin() const
    {
        return D + N - 1;
    }

    const_reverse_iterator crend() const
    {
        return D - 1;
    }

    int size() const
    {
        return N;
    }

    int max_size() const
    {
        return N;
    }

    bool empty() const
    {
        return N == 0;
    }

    T &operator[](int index)
    {
        return D[index];
    }

    T &at(int index)
    {
        return D[index];
    }

    T &front()
    {
        return D[0];
    }

    T &back()
    {
        return D[N - 1];
    }

    T *data()
    {
        return this->D;
    }

    //全部填充
    void fill(const T X)
    {
        for (int i = 0; i < N; i++)
            D[i] = X;
    }

    void swap(my_array<T, N> &_Other)
    {
        for (int i = 0; i < N; i++) {
            std::swap(this->D[i], _Other.D[i]);
        }
    }

    bool operator==(const my_array<T, N> _Other) const
    {
        for (int i = 0; i < N; i++)
            if (this->D[i] != _Other.D[i])
                return false;
        return true;
    }

    bool operator!=(const my_array<T, N> _Other) const
    {
        for (int i = 0; i < N; i++)
            if (this->D[i] != _Other.D[i])
                return true;
        return false;
    }

    bool operator>=(const my_array<T, N> _Other) const
    {
        for (int i = 0; i < N; i++)
            if (this->D[i] < _Other.D[i])
                return false;
        return true;
    }

    bool operator>(const my_array<T, N> _Other) const
    {
        for (int i = 0; i < N; i++)
            if (this->D[i] != _Other.D[i])
                return this->D[i] > _Other.D[i];
        return false;
    }

    bool operator<=(const my_array<T, N> _Other) const
    {
        for (int i = 0; i < N; i++)
            if (this->D[i] > _Other.D[i])
                return false;
        return true;
    }

    bool operator<(const my_array<T, N> _Other) const
    {
        for (int i = 0; i < N; i++)
            if (this->D[i] != _Other.D[i])
                return  this->D[i] < _Other.D[i];;
        return false;
    }
};





#endif