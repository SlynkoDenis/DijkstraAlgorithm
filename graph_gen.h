//
// Created by denis on 18.03.2020.
//
#pragma once

#ifndef DIJKSTRAALGORITHM_GRAPH_GEN_H
#define DIJKSTRAALGORITHM_GRAPH_GEN_H

#endif //DIJKSTRAALGORITHM_GRAPH_GEN_H

#include <algorithm>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>

struct Node {
    size_t number;
    size_t weight;

    Node() : number(0), weight(0) {};
    Node(const size_t& new_number_, const size_t& new_weight_) : number(new_number_), weight(new_weight_) {};
};

std::ostream& operator <<(std::ostream& os, const Node& node) {
    os << "(" << node.number << ", " << node.weight << ")";

    return os;
}

using g_list = std::vector<std::vector<Node>>;

struct Graph {
    g_list gList;

    explicit Graph(const size_t& sz) : gList(sz) {};
    void reserve(const size_t& cap) {
        gList.reserve(cap);
    }
    [[nodiscard]] size_t size() const { return gList.size(); };
    void addNode(size_t number, Node *begin, size_t sz);
    auto begin() {
        return gList.begin();
    }
    auto end() {
        return gList.end();
    }
    [[nodiscard]] auto begin() const {
        return gList.begin();
    }
    [[nodiscard]] auto end() const {
        return gList.end();
    }
    std::vector<Node>& operator [](const size_t& index) {
        return gList[index];
    }
    const std::vector<Node>& operator [](const size_t& index) const {
        return gList[index];
    }
};

void Graph::addNode(size_t number, Node *begin, size_t sz) {
    if (!begin)
        throw std::invalid_argument("list of nodes");

    if (number < size()) {
        auto& iter = gList[number];
        for (size_t i = 0; i < sz; ++i) {
            iter.push_back(*(begin + i));
        }
    } else if (number == size()) {
        std::vector<Node> tmp;
        tmp.resize(sz);
        for (size_t i = 0; i < sz; ++i) {
            tmp.push_back(*(begin + i));
        }
        gList.push_back(tmp);
    }
}

size_t generate_optimal_number(const size_t& number_of_vertices, const size_t& number_of_edges) {
    size_t tmp = number_of_edges / number_of_vertices;
    if (tmp < number_of_vertices)
        return tmp + 2;
    else
        return number_of_vertices / 2;
}

Graph generate_graph(const size_t& number_of_vertices, const size_t& number_of_edges, const size_t& max_weight) {
    if (number_of_edges > number_of_vertices * (number_of_vertices - 1) / 2)
        throw std::invalid_argument("cannot create graph with such number of edges");

    Graph result(number_of_vertices);

    size_t already_generated = 0;
    size_t opt_number = generate_optimal_number(number_of_vertices, number_of_edges);

    std::random_device rd0;
    std::mt19937 gen0(rd0());
    std::uniform_int_distribution<> weight_dis(1, max_weight);

    std::random_device rd1;
    std::mt19937 gen1(rd1());
    std::uniform_int_distribution<> vert_dis(0, number_of_vertices - 1);

    std::random_device rd2;
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> edge_dis(0, opt_number);

    std::vector<bool> vertices(number_of_vertices, false);

    while (already_generated != number_of_edges) {
        size_t current_index = vert_dis(gen1);
        while (vertices[current_index])
            current_index = vert_dis(gen1);
        vertices[current_index] = true;

        size_t current_edges = edge_dis(gen2);
        while (already_generated + current_edges > number_of_edges || current_edges == 0) {
            current_edges = edge_dis(gen2);
        }
        already_generated += current_edges;

        std::set<size_t> tmp;
        std::vector<Node> tmp_res;
        tmp_res.reserve(current_edges);
        for (; tmp.size() < current_edges;) {
            size_t tmp_sz = tmp.size();
            size_t tmp_index = 0;
            while (tmp.size() == tmp_sz) {
                tmp_index = vert_dis(gen1);
                if (tmp_index != current_index)
                    tmp.insert(tmp_index);
            }

            tmp_res.emplace_back(tmp_index, weight_dis(gen0));
        }

        result.gList[current_index] = tmp_res;
    }

    return result;
}
