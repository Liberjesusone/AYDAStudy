#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <memory>
#include <vector>
#include <queue>

using ull = unsigned long long;
using ll = long long;

template <typename T> class LGNode;

// ====================         ====================
// ==================== HASHES: ====================
template <typename T>
struct NodeEqual
{
    bool operator() (const std::weak_ptr<LGNode<T>>& a, const std::weak_ptr<LGNode<T>>& b) const
    {
        auto la = a.lock(), lb = b.lock();
        if (!la && !lb) return true;
        if (!la || !lb) return false;
        return la->get_id() == lb->get_id();
    }
};

template <typename T>
struct NodeHash
{
    ull operator() (const std::weak_ptr<LGNode<T>>& wp) const
    {
        auto live = wp.lock();
        return live ? std::hash<ull>{}(live->get_id()) : 0;
    }
};

// ==================== /HASHES: ====================
// ====================          ====================


/** @brief LGNode is a node of the graph, it has a value and a list of neighbors, 
 * the neighbors are stored as weak_ptr to avoid circular references 
 */
template <typename T>
class LGNode
{
    using NodePtr = std::shared_ptr<LGNode<T>>;
    using WeakNodePtr = std::weak_ptr<LGNode<T>>;
    using Neighbors = std::unordered_set<std::weak_ptr<LGNode<T>>, NodeHash<T>, NodeEqual<T>>; // Set because we don't have repeated conexions
    private:
        T value{};
        ull id{0};
        Neighbors neighbors{}; // References to all their neighbors without owning .lock() to obtain the nullptr/node

    public:
        // Creates a LGNode with the value and the indx as the id
        LGNode(const T& _val, ull _indx) noexcept
        {
            this->value = _val;
            this->id = _indx;
        }

        const Neighbors& get_neighbors() const noexcept { return neighbors; }

        /** @brief it uses a set to store the nieghbors so is an O(1) method
         */
        void add_neighbor(const NodePtr& _node_ptr) noexcept
        {
            if (_node_ptr->get_id() == id) return; // if the node to join is the same as this

            WeakNodePtr weak_node = _node_ptr;
            if (!neighbors.contains(weak_node))
                neighbors.insert(weak_node);
        }

        ull get_id() const noexcept { return id; }

        const T& get_value() const noexcept { return value; }

        T& mut_value() noexcept { return value; }
};


/** @brief LGraph is a graph data structure implemented using an adjacency list
 * LGraph stands for "List Graph", it is a template class that can store any type 
 * of value in the nodes using smart pointers to manage memory automatically. It 
 * provides methods for depth-first search (DFS) and breadth-first search (BFS) 
 * traversal, as well as methods for inserting nodes and edges. Also allows repeated 
 * values in the nodes, but they have to be inserted using the insert_by_indx method, 
 * because the constructor will not allow repeated values
 */
template <typename T>
class LGraph
{
    using NodePtr = std::shared_ptr<LGNode<T>>;
    using WeakNodePtr = std::weak_ptr<LGNode<T>>;
    using Neighbors = std::unordered_set<std::weak_ptr<LGNode<T>>, NodeHash<T>, NodeEqual<T>>; // Set because we don't have repeated conexions

    private:
        ull edges{0};
        std::vector<NodePtr> adj; // The adjacency list

        template <class Functor>
        void _dfs(NodePtr current, const Functor& _lambda, std::vector<bool>& visited) const
        {
            visited[current->get_id()] = true;
            const auto& value = current->get_value();
            _lambda(value);      // We excute the lambda in the current node

            for (WeakNodePtr neighbor : current->get_neighbors())
            {
                auto live = neighbor.lock();
                if (live == nullptr) continue;

                if (!visited[live->get_id()])
                    _dfs(live, _lambda, visited);
            }
        }

    public:

        /** @brief receives a list of pairs, for two new nodes that will be created, if the value is repeated
         * it will take from the existing ones, so in this method two different nodes with the same value cannot be created
         */
        template <template <typename...> class Collection>
        LGraph(const Collection<std::pair<T, T>>& _pairs) noexcept
        {
            std::unordered_map<T, ull> visited;
            ull i = 0;
            for(auto pair : _pairs)
            {
                NodePtr new_node, sec_node;

                // Create the nodes if doesn't exits
                if (!visited.contains(pair.first))
                {
                    new_node = std::make_shared<LGNode<T>>(pair.first, i);

                    adj.push_back(new_node); // Add them to the adjacency list
                    visited[pair.first] = i;  ++i;
                }
                else // If it already exits
                    new_node = adj[visited[pair.first]];

                if (!visited.contains(pair.second))
                {
                    sec_node = std::make_shared<LGNode<T>>(pair.second, i);

                    adj.push_back(sec_node);
                    visited[pair.second] = i;  ++i;
                }
                else
                    sec_node = adj[visited[pair.second]];

                // Add the conexions
                if (new_node != nullptr && sec_node != nullptr)
                {
                    new_node->add_neighbor(sec_node);
                    sec_node->add_neighbor(new_node);
                    ++edges;
                }
            }
        }


        /** @brief Receives a list where the first element is the value for the next node to insert
         * and a vector of ull that represents all the indx of
         */
        template <template <typename...> class Collection>
        void insert_by_indx(const Collection<std::pair<T, std::vector<ull>>>& _pairs) noexcept
        {
            for(auto pair : _pairs)
            {
                T value = pair.first;
                NodePtr new_node = std::make_shared<LGNode<T>>(value, adj.size());
                adj.push_back(new_node);
                const auto& nieghbors = pair.second;

                for(ull ind : nieghbors)
                {
                    if (ind >= adj.size()) continue;
                    new_node->add_neighbor(adj[ind]);
                    adj[ind]->add_neighbor(new_node);
                    ++edges;
                }
            }
        }


         /** @brief iterates the graph in Depth-First Search and executes for each element the @param _lambda function
          * It sends to the _lambda function the a const reference to the value of each node.
          */
        template <class Functor>
        void for_eachDFS(const Functor& _lambda) const
        {
            std::vector<bool> visited(adj.size(), false);   // We pre-size all the index that represents the nodes
            _dfs(adj[0], _lambda, visited);
        }


        /** @brief iterates the graph in Breadth-First Search and executes for each element the @param _lambda function.
         * It sends to the _lambda function the a const reference to the value of each node.
         */
        template <class Functor>
        void for_eachBFS(const Functor& _lambda) const 
        {
            std::vector<bool> visited(adj.size(), false);
            std::queue<NodePtr> to_process;
            to_process.push(adj[0]);
            visited[0] = true; // mark when pushing, not when popping

            while (!to_process.empty())
            {
                // 1. copy front then pop (reference would dangle after pop)
                NodePtr top = to_process.front();
                to_process.pop();

                // 2. process current node
                const auto& value = top->get_value();
                _lambda(value);

                // 3. enqueue unvisited neighbors
                for(WeakNodePtr neighbor : top->get_neighbors())
                {
                    NodePtr live = neighbor.lock();
                    if (live != nullptr && !visited[live->get_id()])
                    {
                        visited[live->get_id()] = true;
                        to_process.push(live);
                    }
                }
            }
        }

        /** @brief iterates trough all nodes and calls @param _lamdba on them it 
         * sends the nodes itself and not their values. So inside _lambda you 
         * should call _node.get_value(), in order to access the value of the node
         */
        template <class Functor>
        void for_each(const Functor& _lambda) const 
        {
            for (ull i = 0; i < adj.size(); ++i)
            {
                const auto& node = *adj[i];
                _lambda(node);
            }
        }

        /** @brief iterates trough all edges and calls @param _lamdba on them it 
         * sends 3 parametters
         *  - _from   : the id of the first node
         *  - _to     : the id of the second node to connect
         *  - _weight : the weight of the edge that is 0 for this construction
         */
        template <class Functor>
        void for_each_edge(const Functor& _lambda) const 
        {
            for (ull i = 0; i < adj.size(); ++i) // for every node
            {
                for (WeakNodePtr weak_neighbor : adj[i]->get_neighbors()) // for every neighbor
                {
                    NodePtr live = weak_neighbor.lock();
                    if (live != nullptr && live->get_id() > i) // if the id is smaller than i, it means that we have already
                        _lambda(i, live->get_id(), 0);         // passed by it 
                }
            }
        }
};