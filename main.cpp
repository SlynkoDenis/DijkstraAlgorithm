#include <iostream>
#include <queue>
#include <vector>

#include "bheap.h"
#include "graph_gen.h"

struct Size {
    long int value;

    Size() : value(-1) {};
    Size(const Size& other) = default;
    explicit Size(size_t new_value_) : value(new_value_) {};
    Size& operator =(size_t new_value_) {
        value = new_value_;
        return *this;
    }
    explicit operator int() {
        return static_cast<int>(value);
    }
};

Size operator +(const Size& lhs, const Size& rhs) {
    Size result;

    if (lhs.value == -1 || rhs.value == -1)
        result.value = -1;
    else
        result.value = lhs.value + rhs.value;
    return result;
}
Size operator +(const Size& lhs, const size_t& rhs) {
    Size result;

    if (lhs.value != -1)
        result.value = rhs + lhs.value;
    return result;
}

std::ostream& operator <<(std::ostream& os, const Size& obj) {
    os << obj.value;

    return os;
}

bool operator <(const Size& lhs, const Size& rhs) {
    if (lhs.value != -1 && rhs.value != -1)
        return lhs.value < rhs.value;
    else if (rhs.value != -1)
        return false;
    else return lhs.value != -1;
}

bool operator ==(const Size& lhs, const Size& rhs) {
    return lhs.value == rhs.value;
}

template <typename T>
bool operator ==(const T& lhs, const Size& rhs) {
    return lhs == rhs.value;
}

template <typename T>
bool operator ==(const Size& lhs, const T& rhs) {
    return lhs.value == rhs;
}

bool operator !=(const Size& lhs, const Size& rhs) {
    return !(lhs == rhs);
}

bool operator >(const Size& lhs, const Size& rhs) {
    return !(lhs < rhs) && (lhs != rhs);
}

namespace {
    template <typename T>
    std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
        for (const auto& it : v) {
            os << it << " ";
        }

        return os;
    }

    std::vector<Size> Dijkstra(const Graph &g, const size_t &start) {
        if (start >= g.size())
            throw std::invalid_argument("start position");

        std::vector<Size> result(g.size());
        result[start] = 0;

        std::vector<bool> used(g.size(), true);

        b_heap<Size, size_t> heap;

        for (size_t i = 0, end_index = g.size(); i < end_index; ++i)
            if (i != start) {
                heap.b_insert(Size(), i);
            } else {
                heap.b_insert(Size(0), i);
            }

        while (!heap.empty()) {
            auto node = heap.b_extract_min();

            used[node.get_value()] = false;
            for (const auto &it : g[node.get_value()]) {
                if (used[it.number]) {
                    if (result[node.get_value()] + it.weight < result[it.number]) {
                        result[it.number] = result[node.get_value()] + it.weight;
                    }
                }
            }

            heap.clear();
            for (size_t i = 0, end_index = g.size(); i < end_index; ++i)
                if (used[i]) {
                    heap.b_insert(Size(result[i]), i);
                }
        }

        return result;
    }

    bool BFS(const Graph& g, const size_t& start, const size_t& goal) {
         if (start >= g.size() || goal >= g.size())
             throw std::invalid_argument("nodes");

         std::vector<bool> visited(g.size(), false);
         visited[start] = true;

         std::queue<size_t> unvisited_vertices;
         unvisited_vertices.push(start);

         while (!unvisited_vertices.empty()) {
             auto tmp = unvisited_vertices.front();
             unvisited_vertices.pop();

             if (tmp == goal)
                 return true;

             for (auto it : g[tmp]) {
                 if (!visited[it.number]) {
                     unvisited_vertices.push(it.number);
                     visited[it.number] = true;
                 }
             }
         }

         return false;
    }

    std::ostream& operator <<(std::ostream& os, const Graph& g) {
        for (size_t i = 0, end_index = g.size(); i < end_index; ++i) {
            os << "Node number " << i << ": ";

            for (const auto& it : g[i]) {
                os << it << " ";
            }

            if (i != end_index - 1)
                os << std::endl;
        }

        return os;
    }
}

int main() {
    Graph g = generate_graph(10, 15, 20);
    std::cout << g << std::endl << std::endl;

    std::cout << Dijkstra(g, 0) << std::endl;

    return 0;
}
