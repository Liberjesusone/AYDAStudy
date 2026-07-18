#pragma once

#include <graph.hpp>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <stdexcept>

namespace graph_lib
{

/**
 * @brief Implements the A* (A-Star) search algorithm.
 *
 * @tparam GraphType The graph type.
 */
template <typename GraphType>
class AStar
{
public:
    using NodeType = typename GraphType::NodeValueType;
    using WeightType = typename GraphType::ArcWeightType;
    using NodePtr = std::shared_ptr<Node<NodeType>>;

    /**
     * @brief Finds the shortest path using a heuristic function.
     *
     * @param graph The graph to search.
     * @param start The source node.
     * @param goal The target node.
     * @param heuristic A function evaluating estimated cost from a node to the goal.
     * @return std::vector<NodePtr> The path from start to goal. Empty if no path exists.
     */
    static std::vector<NodePtr> findPath(
        const GraphType& graph,
        NodePtr start,
        NodePtr goal,
        std::function<WeightType(NodePtr)> heuristic)
    {
        static_assert(!std::is_same_v<WeightType, void>, "A* requires a weighted graph.");

        // TODO: Implement A* search algorithm using a priority queue (Min-Heap).
        // HINT: Maintain a g_score (actual cost) and f_score (estimated total cost) for each node.
        throw std::logic_error("findPath is not implemented yet!");
    }
};

/**
 * @brief Implements the Bellman-Ford algorithm for shortest paths with negative weights.
 */
template <typename GraphType>
class BellmanFord
{
public:
    using NodeType = typename GraphType::NodeValueType;
    using WeightType = typename GraphType::ArcWeightType;
    using NodePtr = std::shared_ptr<Node<NodeType>>;

    /**
     * @brief Computes the Shortest Paths Tree, detecting negative cycles.
     *
     * @param graph The graph to analyze.
     * @param source The starting node.
     * @return GraphType The shortest paths tree.
     * @throws std::runtime_error If a negative weight cycle is detected.
     */
    static GraphType getMinimumPathsTree(const GraphType& graph, NodePtr source)
    {
        static_assert(!std::is_same_v<WeightType, void>, "Bellman-Ford requires a weighted graph.");

        // TODO: Implement Bellman-Ford to find shortest paths and detect negative cycles.
        // HINT: Relax all edges |V| - 1 times. A final relaxation pass should detect cycles.
        throw std::logic_error("getMinimumPathsTree is not implemented yet!");
    }
};

/**
 * @brief Implements Prim's Algorithm for Minimum Spanning Trees (MST).
 */
template <typename GraphType>
class Prim
{
public:
    using NodeType = typename GraphType::NodeValueType;
    using WeightType = typename GraphType::ArcWeightType;
    using NodePtr = std::shared_ptr<Node<NodeType>>;

    /**
     * @brief Builds an MST starting from an arbitrary node.
     */
    static GraphType getMinimumSpanningTree(const GraphType& graph, NodePtr start)
    {
        // TODO: Implement Prim's algorithm using a priority queue.
        // HINT: Keep track of nodes already included in the MST to avoid cycles.
        throw std::logic_error("getMinimumSpanningTree (Prim) is not implemented yet!");
    }
};

/**
 * @brief Implements Kruskal's Algorithm for Minimum Spanning Trees (MST).
 */
template <typename GraphType>
class Kruskal
{
public:
    using NodeType = typename GraphType::NodeValueType;
    using WeightType = typename GraphType::ArcWeightType;
    using NodePtr = std::shared_ptr<Node<NodeType>>;

    static GraphType getMinimumSpanningTree(const GraphType& graph)
    {
        // TODO: Implement Kruskal's algorithm.
        // HINT: Sort all edges by weight. You will need a Disjoint Set (Union-Find) structure.
        throw std::logic_error("getMinimumSpanningTree (Kruskal) is not implemented yet!");
    }
};

/**
 * @brief Implements Dijkstra's algorithm for finding shortest paths in weighted graphs.
 * @note The graph must have non-negative weights.
 */
template <typename GraphType>
class Dijkstra
{
public:
    using WeightType = typename GraphType::ArcWeightType;
    using NodePtr    = typename GraphType::NodePtr;
    using ArcPtr     = typename GraphType::ArcPtr;

    struct NodeInfo
    {
        WeightType distance;    ///< Current shortest distance from source.
        NodePtr predecessor;    ///< Previous node in the shortest path.
        ArcPtr incoming_arc;    ///< Arc used to reach this node.
    };

    /**
     * @brief Computes the Shortest Paths Tree (SPT) from a source node.
     * @param graph The input weighted graph.
     * @param source The source node for path computation.
     * @return GraphType A new graph containing only the arcs that form the shortest paths tree.
     * @par Time complexity: O((V + E) log V)
     */
    static GraphType getMinimumPathsTree(const GraphType& graph, NodePtr source)
    {
        static_assert(!std::is_same_v<WeightType, void>, "Error: Dijkstra requires a weighted graph.");

        std::unordered_map<NodePtr, NodeInfo> tabla;
        using Par = std::pair<WeightType, NodePtr>;
        std::priority_queue<Par, std::vector<Par>, std::greater<Par>> pq;
        tabla[source] = {WeightType{}, nullptr, nullptr}; 
        pq.push({WeightType{}, source});


        while (!pq.empty())
        {
            auto [distancia_actual, actual] = pq.top();
            pq.pop();
            if (distancia_actual > tabla[actual].distance) {
                continue;
            }
            for (const auto& arco : graph.getOutgoingArcs(actual))
            {
                NodePtr vecino = arco->getTgtNode();
                WeightType peso = arco->getWeight();
                WeightType nueva_distancia = distancia_actual + peso;
                
                if (tabla.find(vecino) == tabla.end() || nueva_distancia < tabla[vecino].distance)
                {
                    tabla[vecino] = {nueva_distancia, actual, arco};
                    pq.push({nueva_distancia, vecino});
                }
            }
        }
        GraphType arbol;
        
        for (const auto& par : tabla) {
            arbol.addNode(par.first);
        }
        
        for (const auto& par : tabla) {
            NodePtr nodo = par.first;
            NodeInfo info = par.second;
            if (info.incoming_arc != nullptr) {
                arbol.addArc(info.predecessor, nodo, info.incoming_arc->getWeight());
            }
        }

        return arbol;

        // TODO: Implement Dijkstra's Algorithm using a Min-Heap (Priority Queue).
        //throw std::logic_error("getMinimumPathsTree is not implemented yet!");
    }
};

/**
 * @brief Provides graph traversal algorithms (BFS, DFS) and related operations.
 */
template <typename GraphType>
class GraphTraversals
{
public:
    using NodePtr = typename GraphType::NodePtr;

    /**
     * @brief Performs a Breadth-First Search traversal starting from a node.
     * @param graph The graph to traverse.
     * @param start The starting node.
     * @param op Unary operation (Visitor pattern) invoked for each visited node.
     * @par Time complexity: O(V + E)
     */
    static void traverseBFS(const GraphType& graph, NodePtr start, std::function<void(NodePtr)> op)
    {
        std::unordered_set<NodePtr> visitados;
        std::queue<NodePtr> q;
        
        q.push(start);
        visitados.insert(start);

       
        while (!q.empty()) 
        {
            NodePtr actual = q.front();
            q.pop();
            op(actual);
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                
                if (visitados.find(vecino) == visitados.end()) 
                {
                    visitados.insert(vecino); 
                    q.push(vecino);           
                }
            }
        }

        // TODO: Implement BFS using a Queue. Ensure nodes are only visited once.
        // HINT: Use graph.getOutgoingArcs() and call op(current_node) when a node is processed.
        //throw std::logic_error("traverseBFS is not implemented yet!");
    }

    /**
     * @brief Finds the shortest path between two nodes in an unweighted graph using BFS.
     * @param graph The graph to search.
     * @param start The source node.
     * @param end The destination node.
     * @return std::vector<NodePtr> A vector containing the nodes in the path, or empty if no path exists.
     */
    static std::vector<NodePtr> findShortestPathBFS(const GraphType& graph, NodePtr start, NodePtr end)
    {
        std::vector<NodePtr> path;
        
        // 1. Si ya estamos en el destino, el camino es el nodo mismo
        if (start == end) {
            path.push_back(start);
            return path;
        }

        // 2. Diccionario de migas de pan y cola de exploración
        std::unordered_map<NodePtr, NodePtr> predecesor;
        std::queue<NodePtr> q;
        
        q.push(start);
        predecesor[start] = nullptr; 
        bool encontrado = false;
        while (!q.empty()) 
        {
            NodePtr actual = q.front();
            q.pop();
            if (actual == end) {
                encontrado = true;
                break;
            }
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                if (predecesor.find(vecino) == predecesor.end()) 
                {
                    predecesor[vecino] = actual; 
                    q.push(vecino);              
                }
            }
        }
        if (encontrado) 
        {
            NodePtr paso = end;
            while (paso != nullptr) 
            {
                path.push_back(paso);
                paso = predecesor[paso]; 
            }
            std::reverse(path.begin(), path.end());
        }

        return path;

        // TODO: Implement shortest path finding using BFS and a predecessors map.
        //throw std::logic_error("findShortestPathBFS is not implemented yet!");
    }

    /**
     * @brief Performs a Depth-First Search traversal starting from a node.
     * @param graph The graph to traverse.
     * @param start The starting node.
     * @param op Unary operation (Visitor pattern) invoked for each visited node.
     * @par Time complexity: O(V + E)
     */
    static void traverseDFS(const GraphType& graph, NodePtr start, std::function<void(NodePtr)> op)
    {
        std::unordered_set<NodePtr> visitados;

        std::function<void(NodePtr)> dfs_recursivo = [&](NodePtr actual) 
        {
            visitados.insert(actual);
            op(actual);
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                
                if (visitados.find(vecino) == visitados.end()) 
                {
                    dfs_recursivo(vecino); 
                }
            }
        };
        dfs_recursivo(start);

        // TODO: Implement DFS (You may want to create a private recursive helper function).
        // HINT: Use graph.getOutgoingArcs() and call op(current_node) when a node is processed.
        //throw std::logic_error("traverseDFS is not implemented yet!");
    }

    /**
     * @brief Builds a spanning tree using Depth-First Search.
     * @param graph The original graph.
     * @param start The root node for the spanning tree.
     * @return GraphType A new graph representing the spanning tree.
     */
    static GraphType buildSpanningTreeDFS(const GraphType& graph, NodePtr start)
    {
        GraphType arbol; 
        std::unordered_set<NodePtr> visitados;
        
        arbol.addNode(start);
        visitados.insert(start);
        
        std::function<void(NodePtr)> dfs_recursivo = [&](NodePtr actual) 
        {
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                
                if (visitados.find(vecino) == visitados.end()) 
                {
                    visitados.insert(vecino);
                    arbol.addNode(vecino);
                    arbol.addArc(actual, vecino, arco->getWeight());
                    dfs_recursivo(vecino);
                }
            }
        };

        dfs_recursivo(start);
        return arbol;

        // TODO: Traverse the graph using DFS and add the discovered edges to a new GraphType.
        //throw std::logic_error("buildSpanningTreeDFS is not implemented yet!");
    }

    /**
     * @brief Builds a spanning tree using Breadth-First Search.
     * @param graph The original graph.
     * @param start The root node for the spanning tree.
     * @return GraphType A new graph representing the spanning tree.
     */
    static GraphType buildSpanningTreeBFS(const GraphType& graph, NodePtr start)
    {
        GraphType arbol; 
        std::unordered_set<NodePtr> visitados;
        std::queue<NodePtr> q;     
        arbol.addNode(start);
        visitados.insert(start);
        q.push(start);
        
        while (!q.empty()) 
        {
            NodePtr actual = q.front();
            q.pop();
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                
                if (visitados.find(vecino) == visitados.end()) 
                {
                    visitados.insert(vecino);
                    q.push(vecino);
                    
                    arbol.addNode(vecino);
                    arbol.addArc(actual, vecino, arco->getWeight());
                }
            }
        }

        return arbol;

        // TODO: Traverse the graph using BFS and add the discovered edges to a new GraphType.
        //throw std::logic_error("buildSpanningTreeBFS is not implemented yet!");
    }
};

/**
 * @brief Analyzes graph properties such as cycles and connectivity.
 */
template <typename GraphType>
class GraphProperties
{
public:
    using NodePtr = typename GraphType::NodePtr;

    /**
     * @brief Detects cycles in a directed graph using Kahn's algorithm.
     * @param graph The graph to analyze.
     * @return true If the graph contains at least one cycle.
     */
    static bool hasCycle(const GraphType& graph)
    {
        std::unordered_map<NodePtr, size_t> in_degree;
        for (const auto& nodo : graph.getNodes()) {
            in_degree[nodo] = graph.getInDegree(nodo);
        }
        std::queue<NodePtr> q;
        for (const auto& par : in_degree) {
            if (par.second == 0) {
                q.push(par.first);
            }
        }
        
        size_t nodos_procesados = 0;
        while (!q.empty()) 
        {
            NodePtr actual = q.front();
            q.pop();
            
            nodos_procesados++; 
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                in_degree[vecino]--;
                
                if (in_degree[vecino] == 0) {
                    q.push(vecino);
                }
            }
        }
        return nodos_procesados != graph.getNodes().size();

        // TODO: Implement cycle detection using Kahn's Algorithm concepts.
        // HINT: You can easily get in-degrees using graph.getInDegree(node).
        //throw std::logic_error("hasCycle is not implemented yet!");
    }

    /**
     * @brief Groups nodes into connected components (Primarily for undirected graphs).
     * @param graph The graph to analyze.
     * @return std::vector<std::vector<NodePtr>> A list containing the connected components.
     */
    static std::vector<std::vector<NodePtr>> computeConnectedComponents(const GraphType& graph)
    {
        std::vector<std::vector<NodePtr>> componentes;
        std::unordered_set<NodePtr> visitados;

        for (const auto& nodo : graph.getNodes()) 
        {
            if (visitados.find(nodo) == visitados.end()) 
            {
                std::vector<NodePtr> componente_actual; 
                std::queue<NodePtr> q; 
                
                q.push(nodo);
                visitados.insert(nodo);

                while (!q.empty()) 
                {
                    NodePtr actual = q.front();
                    q.pop();
                    
                    componente_actual.push_back(actual);
                    for (const auto& arco : graph.getOutgoingArcs(actual)) 
                    {
                        NodePtr vecino = arco->getTgtNode();
                        
                        if (visitados.find(vecino) == visitados.end()) 
                        {
                            visitados.insert(vecino);
                            q.push(vecino);
                        }
                    }
                }
                componentes.push_back(componente_actual);
            }
        }

        return componentes;


        // TODO: Iterate over all nodes, launching a BFS/DFS for unvisited ones to form components.
        //throw std::logic_error("computeConnectedComponents is not implemented yet!");
    }

    /**
     * @brief Computes Strongly Connected Components (SCC) using Kosaraju's Algorithm.
     * @param graph The directed graph to analyze.
     * @return std::vector<std::vector<NodePtr>> A list of strongly connected components.
     */
    static std::vector<std::vector<NodePtr>> computeStronglyConnectedComponents(const GraphType& graph)
    {
        std::vector<std::vector<NodePtr>> sccs; 
        std::unordered_set<NodePtr> visitados;
        std::vector<NodePtr> orden_finalizacion;
        std::function<void(NodePtr)> dfs1 = [&](NodePtr actual) {
            visitados.insert(actual);
            for (const auto& arco : graph.getOutgoingArcs(actual)) {
                NodePtr vecino = arco->getTgtNode(); // Vamos hacia el destino
                if (visitados.find(vecino) == visitados.end()) {
                    dfs1(vecino);
                }
            }
            orden_finalizacion.push_back(actual); 
        };
        
        for (const auto& nodo : graph.getNodes()) {
            if (visitados.find(nodo) == visitados.end()) {
                dfs1(nodo);
            }
        }
        visitados.clear(); 
    
        std::function<void(NodePtr, std::vector<NodePtr>&)> dfs2 = [&](NodePtr actual, std::vector<NodePtr>& componente) {
            visitados.insert(actual);
            componente.push_back(actual); 
            for (const auto& arco : graph.getIncomingArcs(actual)) {
                NodePtr vecino = arco->getSrcNode(); 
                
                if (visitados.find(vecino) == visitados.end()) {
                    dfs2(vecino, componente);
                }
            }
        };
        for (auto it = orden_finalizacion.rbegin(); it != orden_finalizacion.rend(); ++it) {
            NodePtr nodo = *it;
            if (visitados.find(nodo) == visitados.end()) {
                std::vector<NodePtr> nuevo_grupo;
                dfs2(nodo, nuevo_grupo); 
                sccs.push_back(nuevo_grupo);
            }
        }

        return sccs;

        // TODO: Implement Kosaraju's Algorithm.
        //throw std::logic_error("computeStronglyConnectedComponents is not implemented yet!");
    }
};

/**
 * @brief Provides topological sorting and ranking for directed acyclic graphs (DAGs).
 */
template <typename GraphType>
class GraphTopological
{
public:
    using NodePtr = typename GraphType::NodePtr;

    /**
     * @brief Computes a topological ordering of the graph's nodes.
     * @param graph The directed acyclic graph to sort.
     * @return std::vector<NodePtr> A vector containing nodes in topological order.
     * @throws std::runtime_error If the graph contains a cycle.
     */
    static std::vector<NodePtr> getSort(const GraphType& graph)
    {
        std::vector<NodePtr> orden_topologico;
        std::queue<NodePtr> cola;
    
    
        std::unordered_map<NodePtr, int> in_degree;

    
        for (const auto& nodo : graph.getNodes()) {
            in_degree[nodo] = 0;
        }

    
        for (const auto& nodo : graph.getNodes()) {
            for (const auto& arco : graph.getOutgoingArcs(nodo)) {
                NodePtr vecino = arco->getTgtNode();
                in_degree[vecino]++;
            }
        }
        for (const auto& par : in_degree) {
            if (par.second == 0) {
                cola.push(par.first);
            }
        }

    
        while (!cola.empty()) {
            NodePtr actual = cola.front();
            cola.pop();
            orden_topologico.push_back(actual);
            for (const auto& arco : graph.getOutgoingArcs(actual)) {
                NodePtr vecino = arco->getTgtNode();
                in_degree[vecino]--;
                if (in_degree[vecino] == 0) {
                    cola.push(vecino);
                }
            }
        }

        if (orden_topologico.size() != graph.getNodes().size()) {
            throw std::runtime_error("Error: El grafo tiene un ciclo. No se puede ordenar topologicamente usando Kahn.");
        }

        return orden_topologico;

        // TODO: Implement Topological Sort using Kahn's Algorithm. Throw an exception on cycle.
        //throw std::logic_error("getSort is not implemented yet!");
    }

    /**
     * @brief Computes topological ranks (depth levels) for each node.
     * @param graph The directed acyclic graph to analyze.
     * @return std::unordered_map<NodePtr, int> A map from nodes to their ranks.
     * @throws std::runtime_error If the graph contains a cycle.
     */
    static std::unordered_map<NodePtr, int> getRanks(const GraphType& graph)
    {
        std::unordered_map<NodePtr, size_t> in_degree;
        std::unordered_map<NodePtr, int> rangos; 
        for (const auto& nodo : graph.getNodes()) {
            in_degree[nodo] = graph.getInDegree(nodo);
            rangos[nodo] = 0; 
        }
        std::queue<NodePtr> q;
        for (const auto& par : in_degree) {
            if (par.second == 0) {
                q.push(par.first);
            }
        }
        size_t procesados = 0;
        while (!q.empty()) 
        {
            NodePtr actual = q.front();
            q.pop();
            procesados++;
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                in_degree[vecino]--;
                rangos[vecino] = std::max(rangos[vecino], rangos[actual] + 1);
                
                if (in_degree[vecino] == 0) {
                    q.push(vecino);
                }
            }
        }
        if (procesados != graph.getNodes().size()) {
            throw std::runtime_error("Error: El grafo contiene un ciclo. No se pueden calcular los rangos.");
        }
        
        return rangos;

        // TODO: Compute ranks. Nodes with no dependencies have rank 0, their successors have rank 1...
        //throw std::logic_error("getRanks is not implemented yet!");
    }
};

} // namespace graph_lib
