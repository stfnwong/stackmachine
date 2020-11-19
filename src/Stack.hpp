/*
 * STACK
 * A templated stack
 *
 * Stefan Wong 2020
 */

#ifndef __SM_STACK_HPP
#define __SM_STACK_HPP

#include <iomanip>
#include <iostream>     // debug only - remove
#include <sstream>
#include <string>
#include <vector>

constexpr int DEFAULT_STACK_SIZE = 32;

template <typename T> class Stack
{
    std::vector<T> data;
    unsigned int ptr;
    unsigned int max_size;

    public:
        Stack() : data(DEFAULT_STACK_SIZE), ptr(0), max_size(DEFAULT_STACK_SIZE) {} 
        Stack(unsigned int stack_size) : data(stack_size), ptr(0), max_size(stack_size) {} 
        Stack(const Stack& that);

        // copy assign
        Stack& operator=(const Stack& that);

        // comparisons
        bool operator==(const Stack& that) const;
        bool operator!=(const Stack& that) const;

        void         push(const T& x);
        T            pop(void);
        T            peek(void);
        bool         empty(void) const;
        bool         full(void) const;
        unsigned int size(void) const;
        unsigned int capacity(void) const;

        std::string toString(void) const;
};


// copy ctor
template <typename T> Stack<T>::Stack(const Stack& that)
{
    this->ptr = that.ptr;
    this->max_size = that.max_size;
    this->data.reserve(that.data.size());
    for(unsigned int i = 0; i < that.data.size(); ++i)
        this->data[i] = that.data[i];
}
// copy assignment
template <typename T> Stack<T>& Stack<T>::operator=(const Stack& that)
{
    if(this != &that)
    {
        if(this->max_size < that.max_size)
            this->data.reserve(that.max_size);
        this->ptr = that.ptr;
        this->max_size = that.max_size;
        for(unsigned int i = 0; i < that.data.size(); ++i)
            this->data[i] = that.data[i];
    }

    return *this;
}

// comparisons 
template <typename T> bool Stack<T>::operator==(const Stack& that) const
{
    if(this->ptr != that.ptr)
        return false;
    if(this->max_size != that.max_size)
        return false;
    for(unsigned int i = 0; i < this->max_size; ++i)
    {
        if(this->data[i] != that.data[i])
            return false;
    }

    return true;
}

template <typename T> bool Stack<T>::operator!=(const Stack& that) const
{
    return !(*this == that);
}

template <typename T> void Stack<T>::push(const T& x)
{
    //std::cout << "[" << __func__ << "] this->ptr = " << this->ptr << std::endl;
    if(this->ptr < this->max_size)
    {
        this->data[this->ptr] = x;
        this->ptr++;
    }
}

template <typename T> T Stack<T>::pop(void)
{
    auto x = this->data[this->ptr-1];
    if(this->ptr > 0)
        this->ptr--;
    return x;
}

// TODO : this is a little bit shit...
template <typename T> T Stack<T>::peek(void)
{
    if(this->ptr > 0)
        return this->data[this->ptr-1];
    return this->data[0];
}

template <typename T> bool Stack<T>::empty(void) const
{
    return (this->ptr == 0) ? true : false;
}

template <typename T> bool Stack<T>::full(void) const
{
    return this->ptr == this->max_size;     // since ptr increments over the end
}

template <typename T> unsigned int Stack<T>::size(void) const
{
    return this->ptr;
}

template <typename T> unsigned int Stack<T>::capacity(void) const
{
    return this->max_size;
}

template <typename T> std::string Stack<T>::toString(void) const
{
    std::ostringstream oss;

    oss << "Stack <" << this->max_size << ">, " << this->ptr << " items" << std::endl;
    for(unsigned int i = 0; i < this->ptr; ++i)
    {
        oss << "   [" << std::setw(4) << std::setfill(' ') << i << "] : " 
            << this->data[i] << std::endl;
    }

    return oss.str();
}

#endif /*__SM_STACK_HPP*/
