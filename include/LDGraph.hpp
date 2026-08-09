#pragma once

#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>
#include <vector>

using ull = unsigned long long;
using ll = long long;

template <typename T> class LDGNode;
struct Connexion
{
    public: 
        ull a_id;
        ull b_id;
        ll weight;
};

/** @brief LDGNode stands for adjacency {L}ist, {D}irected {G}raph node
 * preserves a map with the k: id and the v: wieght for every neighbor
 */
template <typename T>
class LDGNode
{
    // Map because we don't have repeated conexions, so the key is the neihbor's id and the value the weight to travel there
    using Neighbors = std::unordered_map<ull, ll>; 

    private:
        T value{};
        ull id{0};
        Neighbors neighbors{}; // Id's of all their neighbors 

    public:
        // Creates a LGNode with the value and the indx as the id
        LDGNode(const T& _val, ull _indx) noexcept
        {
            this->value = _val;
            this->id = _indx;
        }

        const Neighbors& get_neighbors() const noexcept { return neighbors; }

        /** @brief it uses a map to store the nieghbors so is an O(1) method        
         *
         * @param _neighbor_id the neighbor's id to add
         * @param _weight      to travel from this node to the new neighbor
         *
         * @returns true if was correctly added, otherwise false
         */
        bool add_neighbor(ull _neighbor_id, ll _weight) noexcept
        {
            if (_neighbor_id == id) return false; // if the node to join is the same as this

            if (!neighbors.contains(_neighbor_id))
            {
                neighbors[_neighbor_id] = _weight;
                return true;
            }
            return false;
        }

        ull get_id() const noexcept { return id; }

        const T& get_value() const noexcept { return value; }

        T& mut_value() noexcept { return value; }
};



/** @brief LDGraph stands for adjacency {L}ist, {D}irected {G}raph 
 * is a graph that holds nodes with no sense of their neighbors more than
 * their ids, so the Graph class is the only one capable of iteratin through
 * the structure
 */
template <typename T>
class LDGraph
{
    using NodePtr = std::shared_ptr<LDGNode<T>>;
    using Neighbors = std::unordered_map<ull, ll>; 

    private:
        ull edges{0};
        std::vector<NodePtr> adj; // The adjacency list

        template <class Functor> 
        void _dfs(NodePtr current, const Functor& _lambda, std::vector<bool>& visited) const
        {
            visited[current->get_id()] = true;
            const auto& value = current->get_value();
            _lambda(value);      // We excute the lambda in the current node

            for (std::pair<ull, ll> neighbor : current->get_neighbors())
            {
                if (neighbor.first >= adj.size()) continue;

                if (!visited[neighbor.first])
                    _dfs(adj[neighbor.first], _lambda, visited);
            }
        }

    public:

        /** @brief receives the list of values of every node and a list of conexions, 
         * each of them representing two nodes that will be conected
         * (a, b, weight) = a -> b with cost weight  : each conexion represents the values 
         * and the order of conection
         * 
         * in this method two different nodes with the same value can be created, just have 
         * to be repeated in the list
         */
        template <template <typename...> class Collection>
        LDGraph(const Collection<T>& _values, const Collection<Connexion>& _conn) noexcept
        {
            ull i = 0;
            for(auto value : _values) // for every node
            {
                // Create the node and add it to the adjacency list
                NodePtr new_node = std::make_shared<LDGNode<T>>(value, i);
                adj.push_back(new_node); 
                ++i;
            }

            for (Connexion con : _conn) // for every connexion
            {
                if (con.a_id >= adj.size() || con.b_id >= adj.size()) continue; // if it's out of bounds
                if (adj[con.a_id]->add_neighbor(con.b_id, con.weight)) ++edges;
            }
        }


        /** @brief Receives a list where the first element is the value for the next node to insert
         * and a vector of pair<ull, ll> that represents all the indx of their neighbors and weights
         */
        template <template <typename...> class Collection>
        void insert_by_indx(const Collection<std::pair<T, std::vector<std::pair<ull, ll>>>>& _pairs) noexcept
        {
            for(auto pair : _pairs)
            {
                T value = pair.first;
                NodePtr new_node = std::make_shared<LDGNode<T>>(value, adj.size());
                adj.push_back(new_node);
                const auto& nieghbors = pair.second;

                for(std::pair<ull, ll> neighbor : nieghbors)
                {
                    if (neighbor.first >= adj.size()) continue;
                    if (new_node->add_neighbor(neighbor.first, neighbor.second)) ++edges;
                }
            }
        }


         /** @brief iterates the graph in Depth-First Search and executes for each element the @param _lambda function
          * It sends to the _lambda function the a const reference to the value of each node.
          */
        template <class Functor>
        void for_eachDFS(const Functor& _lambda) const
        {
            if (adj.size() == 0) return;
            std::vector<bool> visited(adj.size(), false);   // We pre-size all the index that represents the nodes
            _dfs(adj[0], _lambda, visited);
        }


        /** @brief iterates the graph in Breadth-First Search and executes for each element the @param _lambda function.
         * It sends to the _lambda function the a const reference to the value of each node.
         */
        template <class Functor>
        void for_eachBFS(const Functor& _lambda) const 
        {
            if (adj.size() == 0) return;
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
                for(std::pair<ull, ll> neighbor : top->get_neighbors())
                {
                    if (neighbor.first >= adj.size() || visited[neighbor.first]) continue;
                    visited[neighbor.first] = true;
                    to_process.push(adj[neighbor.first]);
                }
            }
        }

        /** @brief iterates trough ALL nodes (even not connected ones) and calls 
         * @param _lamdba on them, it sends the nodes itself and not their values. 
         * So inside _lambda you should call _node.get_value(), in order to access 
         * the value of the node
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
         *  - _weight : the weight of the edge 
         */
        template <class Functor>
        void for_each_edge(const Functor& _lambda) const 
        {
            for (ull i = 0; i < adj.size(); ++i) // for every node
            {
                for (std::pair<ull, ll> neighbor : adj[i]->get_neighbors()) // for every neighbor
                {
                    if (neighbor.first < adj.size()) // here every conn is valid since we have that id
                        _lambda(i, neighbor.first, neighbor.second);         
                }
            }
        }
};

