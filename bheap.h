//
// Created by denis on 18.03.2020.
//
#pragma once

#ifndef DIJKSTRAALGORITHM_BHEAP_H
#define DIJKSTRAALGORITHM_BHEAP_H

#endif //DIJKSTRAALGORITHM_BHEAP_H

#include <vector>
#include <cstring>
#include <cmath>
#include <stdexcept>

template <typename K, typename T> class t_node
{
    K key;
    T data;

public:
    t_node() : key(K()), data(nullptr) {};
    t_node(K num, T str) : key(num), data(str) {};
    t_node& operator =(const t_node& other) {
        key = other.key;
        data = other.data;

        return *this;
    };
    void set_key(K num) {
        key = num;
    };
    K get_key() {
        return key;
    };
    T get_value() {
        return data;
    };
};

template <typename K, typename T> class b_heap
{
    std::vector<t_node<K, T>> heap_v;
    size_t last_iter;

    int node_min(int first, int second);

public:
    b_heap() : last_iter(0) {};
    template <typename FIt> b_heap(FIt first, FIt last);
    [[nodiscard]] size_t size() const { return heap_v.size(); };
    [[nodiscard]] bool empty() const { return heap_v.empty(); };
    size_t b_insert(K num, T str);
    void b_remove(size_t it);
    inline K b_get_key(unsigned it) {
        if (it < heap_v.size ())
            return heap_v[it].get_key();
        else
            throw std::invalid_argument("position");
    };
    t_node<K, T> b_extract_min();
    size_t b_decrease_key(size_t it, K num);
    size_t sift_up(size_t num);
    size_t sift_down(size_t num);
    void clear() {
        heap_v.clear();
        last_iter = 0;
    };
};

template <typename K, typename T> template <typename FIt> b_heap<K, T>::b_heap(FIt first, FIt last) : heap_v(first, last), last_iter(0) {           // wasn't testes
    size_t sz = heap_v.size();
    for (auto i = (sz - 2) / 2; i >= 0; --i) {
        if (2 * i + 1 < sz && 2 * i + 2 < sz) {
            if (heap_v[i] > std::min(heap_v[2 * i + 1], heap_v[2 * i + 2]))
                sift_down(i);
        } else if (2 * i + 1 < sz) {
            if (heap_v[i] > heap_v[2 * i + 1])
                sift_down(i);
        }
    }
}

template <typename K, typename T> int b_heap<K, T>::node_min(int first, int second) {
    if ((first < heap_v.size()) && (second < heap_v.size()))
    {
        int num_min = std::min(static_cast<int>(heap_v[first].get_key()), static_cast<int>(heap_v[second].get_key()));

        return num_min == heap_v[first].get_key() ? first : second;
    }
    else if (first < heap_v.size())
        return first;
    else if (second < heap_v.size())
        return second;
    else
        return heap_v.size() + 1;
}

template <typename K, typename T> size_t b_heap<K, T>::sift_up(size_t num) {
    if (num >= size())
        throw std::invalid_argument("overflow");
    if (!num)
        return num;

    K check_num = heap_v[num].get_key();

    while ((num >= 0) && (heap_v[(num - 1) / 2].get_key() > check_num))
    {
        //heap_v[num].t_swap(heap_v[(num - 1) / 2]);
        std::swap(heap_v[num], heap_v[(num - 1) / 2]);
        num = (num - 1) / 2;
    }

    return num;
}

template <typename K, typename T> size_t b_heap<K, T>::sift_down(size_t num) {
    int lt = heap_v.size() - 1;

    if (lt == -1)
        throw std::invalid_argument("heap is empty");

    if (num == lt)
        return num;

    int num_tmp = node_min(2 * num + 1, 2 * num + 2);
    while ((num_tmp <= lt) && (heap_v[num_tmp].get_key() < heap_v[num].get_key()))
    {
        std::swap(heap_v[num_tmp], heap_v[num]);
        num = num_tmp;
        num_tmp = node_min(2 * num + 1, 2 * num + 2);
    }

    return num;
}

template <typename K, typename T> size_t b_heap<K, T>::b_insert(K num, T str) {
    t_node<K, T> buf(num, str);
    heap_v.push_back(buf);

    return last_iter = sift_up(heap_v.size() - 1);
}

template <typename K, typename T> t_node<K, T> b_heap<K, T>::b_extract_min() {
    int lt = heap_v.size() - 1;

    if (lt == -1)
        throw std::logic_error("heap is empty");

    if (lt == 0)
    {
        auto res = heap_v[0];
        heap_v.pop_back();

        return res;
    }

    auto result = heap_v[0];
    std::swap(heap_v[0], heap_v[lt]);
    heap_v.pop_back();

    last_iter = sift_down(0);
    return result;
}

template <typename K, typename T> void b_heap<K, T>::b_remove(size_t it) {
    int lt = heap_v.size() - 1;

    if (it > lt)
        throw std::invalid_argument("incoming position");

    if (it == lt)
    {
        heap_v.pop_back();
        return ;
    }

    std::swap(heap_v[it], heap_v[lt]);
    heap_v.pop_back();

    last_iter = sift_down(it);
}

template <typename K, typename T> size_t b_heap<K, T>::b_decrease_key(size_t it, K num) {
    int lt = heap_v.size() - 1;

    if (it > lt)
        throw std::invalid_argument("incoming position");

    if (num > heap_v[it].give_key())
        throw std::invalid_argument("incoming new value");

    heap_v[it].set_key(num);
    return last_iter = sift_up(it);
}