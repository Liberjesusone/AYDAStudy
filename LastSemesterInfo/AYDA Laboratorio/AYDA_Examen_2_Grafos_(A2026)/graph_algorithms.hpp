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
 * @brief Implements Dijkstra's algorithm for finding shortest paths in weighted graphs.
 * @note The graph must have non-negative weights.
 */
template <typename GraphType>
class Dijkstra
{
public:
    using WeightType = typename GraphType::ArcWeightType;
    using NodePtr = typename GraphType::NodePtr;
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
    using ArcPtr     = typename GraphType::ArcPtr;
    using WeightType = typename GraphType::ArcWeightType;


    /**
     * @brief Performs a Breadth-First Search traversal starting from a node.
     * @param graph The graph to traverse.
     * @param start The starting node.
     * @param op Unary operation (Visitor pattern) invoked for each visited node.
     * @par Time complexity: O(V + E)
     */
    static void traverseBFS(const GraphType& graph, NodePtr start, std::function<void(NodePtr)> op)
    {
        // TODO: Implement BFS using a Queue. Ensure nodes are only visited once.
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
        // TODO: Implement shortest path finding using BFS and a predecessors map.
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
        // TODO: Implement DFS (You may want to create a private recursive helper function).
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
            // 1. Revisamos las flechas de SALIDA
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                // Seguridad: Si la librería me devolvió mi propio nodo, tomo el otro extremo
                if (vecino == actual) vecino = arco->getSrcNode(); 

                if (visitados.find(vecino) == visitados.end()) 
                {
                    visitados.insert(vecino);
                    arbol.addNode(vecino);
                    arbol.addArc(actual, vecino, arco->getWeight());
                    dfs_recursivo(vecino);
                }
            }

            // 2. Revisamos las flechas de ENTRADA (Clave para grafos No Dirigidos)
            for (const auto& arco : graph.getIncomingArcs(actual)) 
            {
                NodePtr vecino = arco->getSrcNode();
                // Seguridad: Si la librería me devolvió mi propio nodo, tomo el otro extremo
                if (vecino == actual) vecino = arco->getTgtNode();

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
            
            // 1. Revisamos las flechas de SALIDA
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                // Seguridad: Si el destino soy yo mismo, tomo el origen
                if (vecino == actual) vecino = arco->getSrcNode(); 
                
                if (visitados.find(vecino) == visitados.end()) 
                {
                    visitados.insert(vecino);
                    q.push(vecino);
                    
                    arbol.addNode(vecino);
                    arbol.addArc(actual, vecino, arco->getWeight());
                }
            }

            // 2. Revisamos las flechas de ENTRADA (Para soportar UndirectedGraph)
            for (const auto& arco : graph.getIncomingArcs(actual)) 
            {
                NodePtr vecino = arco->getSrcNode();
                // Seguridad: Si el origen soy yo mismo, tomo el destino
                if (vecino == actual) vecino = arco->getTgtNode();

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
    }

    /**
     * @brief Realiza un recorrido DFS visual estilo "Laberinto", imprimiendo los pasos.
     * @param graph El grafo a recorrer.
     * @param start El nodo de inicio.
     */
    static void printMazeTraversal(const GraphType& graph, NodePtr start)
    {
        std::unordered_set<NodePtr> visitados;
        std::unordered_set<ArcPtr> aristas_procesadas; // Para no pasar por la misma puerta 2 veces

        std::cout << "=== INICIANDO RECORRIDO EN EL LABERINTO ===\n";

        std::function<void(NodePtr)> dfs_laberinto = [&](NodePtr u) {
            visitados.insert(u);

            for (const auto& arco : graph.getOutgoingArcs(u)) {
                NodePtr v = arco->getTgtNode();

                // Si no hemos cruzado por esta puerta/arista exacta
                if (aristas_procesadas.find(arco) == aristas_procesadas.end()) {
                    
                    aristas_procesadas.insert(arco);

                    if (visitados.find(v) == visitados.end()) {
                        // Habitación nueva
                        std::cout << "Explorando: " << u->getValue() << " -> " << v->getValue() << "\n";
                        dfs_laberinto(v);
                        std::cout << "Retrocediendo: " << v->getValue() << " -> " << u->getValue() << "\n";
                    } else {
                        // Callejón sin salida (Habitación ya visitada)
                        std::cout << "Ida (callejon/visitado): " << u->getValue() << " -> " << v->getValue() << "\n";
                        std::cout << "Vuelta inmediata: " << v->getValue() << " -> " << u->getValue() << "\n";
                    }
                }
            }
        };

        dfs_laberinto(start);
        std::cout << "=== HAS SALIDO DEL LABERINTO ===\n";
    }

    /**
     * @brief Algoritmo de Floyd-Warshall. Calcula las distancias minimas entre TODOS los pares de nodos.
     * @return Un mapa de mapas: distancias[origen][destino] = distancia_minima
     */
    static std::unordered_map<NodePtr, std::unordered_map<NodePtr, WeightType>> getAllPairsShortestPaths(const GraphType& graph)
    {
        std::unordered_map<NodePtr, std::unordered_map<NodePtr, WeightType>> dist;
        WeightType INFINITO = std::numeric_limits<WeightType>::max() / 2;

        // 1. Inicializar la matriz de distancias
        for (const auto& u : graph.getNodes()) {
            for (const auto& v : graph.getNodes()) {
                dist[u][v] = (u == v) ? WeightType{} : INFINITO;
            }
            // Si hay arista directa, ponemos su peso
            for (const auto& arco : graph.getOutgoingArcs(u)) {
                dist[u][arco->getTgtNode()] = arco->getWeight();
            }
            for (const auto& arco : graph.getIncomingArcs(u)) {
                dist[u][arco->getSrcNode()] = arco->getWeight();
            }
        }

        // 2. El núcleo de Floyd-Warshall (3 ciclos for anidados)
        for (const auto& k : graph.getNodes()) {          // Nodo intermedio
            for (const auto& i : graph.getNodes()) {      // Nodo origen
                for (const auto& j : graph.getNodes()) {  // Nodo destino
                    if (dist[i][k] != INFINITO && dist[k][j] != INFINITO) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                        }
                    }
                }
            }
        }
        return dist;
    }
    
    /**
     * @brief Algoritmo Voraz (Greedy) para coloreado de grafos.
     * Asigna colores (numeros enteros) a los nodos de forma que ningun vecino tenga el mismo color.
     * Util para problemas de planificacion (ej. evitar choques de horarios).
     */
    static std::unordered_map<NodePtr, int> greedyGraphColoring(const GraphType& graph)
    {
        std::unordered_map<NodePtr, int> color_asignado;
        
        // Inicializamos todos los nodos sin color (-1)
        for (const auto& nodo : graph.getNodes()) {
            color_asignado[nodo] = -1;
        }

        for (const auto& actual : graph.getNodes()) {
            std::unordered_set<int> colores_vecinos;

            // 1. Recolectamos TODOS los vecinos reales (Soporte total para No Dirigidos)
            std::unordered_set<NodePtr> vecinos;
            for (const auto& arco : graph.getOutgoingArcs(actual)) vecinos.insert(arco->getTgtNode());
            for (const auto& arco : graph.getIncomingArcs(actual)) vecinos.insert(arco->getSrcNode());

            // 2. Revisamos qué colores ya están usando mis vecinos
            for (NodePtr vecino : vecinos) {
                if (vecino != actual && color_asignado[vecino] != -1) {
                    colores_vecinos.insert(color_asignado[vecino]);
                }
            }

            // 3. Buscamos el color (número) más bajo que esté disponible
            int nuevo_color = 0;
            while (colores_vecinos.find(nuevo_color) != colores_vecinos.end()) {
                nuevo_color++;
            }

            // 4. Se lo asignamos al nodo actual
            color_asignado[actual] = nuevo_color;
        }

        return color_asignado;
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
        // TODO: Implement cycle detection using Kahn's Algorithm concepts.
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



        // HINT: You can easily get in-degrees using graph.getInDegree(node).
        //throw std::logic_error("hasCycle is not implemented yet!");
    }

    /**
     * @brief Busca los nodos que componen un ciclo en un grafo digido.
     * @param graph The graph to analyze.
     * @return Devuelve un vector de nodos ordenados que componen el ciclo
     */
    static std::vector<NodePtr> getCycleNodes(const GraphType& graph)
    {
        std::unordered_map<NodePtr, size_t> in_degree;
        
        //  Inicializamos los grados de entrada
        for (const auto& nodo : graph.getNodes()) {
            in_degree[nodo] = graph.getInDegree(nodo);
        }
        
        std::queue<NodePtr> q;
        
        //  Metemos los nodos libres (in_degree == 0)
        for (const auto& par : in_degree) {
            if (par.second == 0) {
                q.push(par.first);
            }
        }
        
        //  Procesamos el grafo 
        while (!q.empty()) 
        {
            NodePtr actual = q.front();
            q.pop();
            
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                in_degree[vecino]--;
                
                if (in_degree[vecino] == 0) {
                    q.push(vecino);
                }
            }
        }

    
        // Cualquier nodo que se haya quedado con in_degree > 0 está atrapado en un ciclo
        std::vector<NodePtr> nodos_en_ciclo;
        for (const auto& par : in_degree) {
            if (par.second > 0) {
                nodos_en_ciclo.push_back(par.first);
            }
        }
        return nodos_en_ciclo;
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
                    
                    // REVISAR SALIDAS
                    for (const auto& arco : graph.getOutgoingArcs(actual)) {
                        NodePtr vecino = arco->getTgtNode();
                        if (visitados.find(vecino) == visitados.end()) {
                            visitados.insert(vecino);
                            q.push(vecino);
                        }
                    }
                    // REVISAR ENTRADAS (Para soportar UndirectedGraph)
                    for (const auto& arco : graph.getIncomingArcs(actual)) {
                        NodePtr vecino = arco->getSrcNode();
                        if (visitados.find(vecino) == visitados.end()) {
                            visitados.insert(vecino);
                            q.push(vecino);
                        }
                    }
                }
                componentes.push_back(componente_actual);
            }
        }
        return componentes;
    }

    /**
     * @brief Computes Strongly Connected Components (SCC) using Kosaraju's Algorithm.
     * @param graph The directed graph to analyze.
     * @return std::vector<std::vector<NodePtr>> A list of strongly connected components.
     */
    static std::vector<std::vector<NodePtr>> computeStronglyConnectedComponents(const GraphType& graph)
    {
        // TODO: Implement Kosaraju's Algorithm.
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

        //throw std::logic_error("computeStronglyConnectedComponents is not implemented yet!");
    }


    /**
     * @brief Devuelve una lista con la cantidad de nodos de cada "Isla" (Componente Conexa).
     * La lista viene ordenada de mayor a menor (Ideal para "Encontrar la isla más grande").
     */
    static std::vector<int> getIslandsSizes(const GraphType& graph)
    {
        // Usamos tu función de componentes conexas
        auto componentes = GraphProperties<GraphType>::computeConnectedComponents(graph);
        std::vector<int> tamanos;

        for (const auto& isla : componentes) {
            tamanos.push_back(isla.size());
        }

        // Ordenamos de mayor a menor
        std::sort(tamanos.rbegin(), tamanos.rend());
        
        return tamanos;
    }


    /**
     * @brief Detecta y devuelve las fuentes (in=0) y sumideros (out=0).
     */
    static void printSourcesAndSinks(const GraphType& graph)
    {
        bool hay_fuentes = false;
        bool hay_sumideros = false;

        std::cout << "--- Analisis de Fuentes y Sumideros ---\n";
        for (const auto& nodo : graph.getNodes()) {
            size_t in = graph.getInDegree(nodo);
            size_t out = graph.getOutDegree(nodo);

            if (in == 0 && out > 0) {
                std::cout << "Fuente detectada: " << nodo->getValue() << "\n";
                hay_fuentes = true;
            }
            if (out == 0 && in > 0) {
                std::cout << "Sumidero detectado: " << nodo->getValue() << "\n";
                hay_sumideros = true;
            }
        }
        if (!hay_fuentes) std::cout << "No hay fuentes.\n";
        if (!hay_sumideros) std::cout << "No hay sumideros.\n";
    }

    /**
     * @brief Verifica si el grafo es Bipartito (Problema de los Luchadores).
     * @return true si se puede dividir en dos bandos sin conflictos.
     */
    static bool isBipartite(const GraphType& graph)
    {
       // 0: Sin color, 1: Color A, 2: Color B
    std::unordered_map<NodePtr, int> color;
    for (const auto& n : graph.getNodes()) color[n] = 0;

    for (const auto& start_node : graph.getNodes()) {
        if (color[start_node] == 0) {
            std::queue<NodePtr> q;
            q.push(start_node);
            color[start_node] = 1;

            while (!q.empty()) {
                NodePtr u = q.front();
                q.pop();

                // Recolectamos vecinos únicos para evitar procesar doble en grafos no dirigidos
                std::unordered_set<NodePtr> vecinos;
                for (auto& a : graph.getOutgoingArcs(u)) vecinos.insert(a->getTgtNode());
                for (auto& a : graph.getIncomingArcs(u)) vecinos.insert(a->getSrcNode());

                for (NodePtr v : vecinos) {
                    if (v == u) continue; // Ignorar self-loops si existen

                    if (color[v] == 0) {
                        // Asignar color opuesto: (1 -> 2) o (2 -> 1)
                        color[v] = (color[u] == 1) ? 2 : 1;
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        // ¡Conflicto! Dos nodos adyacentes tienen el mismo color
                        return false;
                    }
                }
            }
        }
    }
    return true;
    }

    /**
     * @brief Cuenta los caminos simples entre dos nodos en un DAG usando Memoization.
     */
    static int countSimplePaths(const GraphType& graph, NodePtr origen, NodePtr destino)
    {
        std::unordered_map<NodePtr, int> memo;

        std::function<int(NodePtr)> dfs_contar = [&](NodePtr u) -> int {
            if (u == destino) return 1;
            if (memo.find(u) != memo.end()) return memo[u];

            int caminos = 0;
            for (const auto& arco : graph.getOutgoingArcs(u)) {
                caminos += dfs_contar(arco->getTgtNode());
            }
            memo[u] = caminos;
            return caminos;
        };

        return dfs_contar(origen);
    }


    /**
     * @brief Verifica si un grafo dirigido es Simplemente Conexo.
     * Es decir, si existe a lo sumo UN solo camino simple entre cualquier par de nodos.
     */
    static bool isSimplyConnected(const GraphType& graph)
    {
        for (const auto& start_node : graph.getNodes()) {
            std::unordered_map<NodePtr, int> color; // 0: Blanco, 1: Gris, 2: Negro
            for (const auto& n : graph.getNodes()) color[n] = 0;

            // Función DFS recursiva con lambda
            std::function<bool(NodePtr)> dfs = [&](NodePtr u) -> bool {
                color[u] = 1; // Visitando (Gris)
                
                for (const auto& arco : graph.getOutgoingArcs(u)) {
                    NodePtr v = arco->getTgtNode();
                    if (color[v] == 0) {
                        if (!dfs(v)) return false;
                    } 
                    else if (color[v] == 2) {
                        // Si tocamos un nodo Negro, significa que llegamos a él por otro camino distinto.
                        // Por lo tanto, no es simplemente conexo.
                        return false; 
                    }
                }
                color[u] = 2; // Terminado (Negro)
                return true;
            };

            if (!dfs(start_node)) return false;
        }
        return true;
    }

    /**
     * @brief Calcula el Diámetro de un Árbol (La mayor distancia posible entre dos nodos).
     * @note Se asume que el grafo ingresado no tiene ciclos (es un árbol).
     */
    static int getTreeDiameter(const GraphType& graph)
    {
        if (graph.getNodes().empty()) return 0;

        // Sub-función: Un BFS que devuelve {Nodo_Mas_Lejano, Distancia}
        auto bfsFurthestNode = [&](NodePtr start) -> std::pair<NodePtr, int> {
            std::unordered_map<NodePtr, int> dist;
            std::queue<NodePtr> q;
            
            q.push(start);
            dist[start] = 0;
            
            NodePtr furthest = start;
            int max_d = 0;
            
            while (!q.empty()) {
                NodePtr u = q.front();
                q.pop();
                
                for (const auto& arco : graph.getOutgoingArcs(u)) {
                    NodePtr v = arco->getTgtNode();
                    if (dist.find(v) == dist.end()) {
                        dist[v] = dist[u] + 1; // Asumimos distancia +1 por arista
                        q.push(v);
                        if (dist[v] > max_d) {
                            max_d = dist[v];
                            furthest = v;
                        }
                    }
                }
                for (const auto& arco : graph.getIncomingArcs(u)) {
                    NodePtr v = arco->getSrcNode();
                    if (dist.find(v) == dist.end()) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                        if (dist[v] > max_d) {
                            max_d = dist[v];
                            furthest = v;
                        }
                    }
                }
            }
            return {furthest, max_d};
        };

        // El truco del doble BFS:
        // 1. Buscamos el extremo A desde cualquier nodo
        NodePtr root = *graph.getNodes().begin();
        auto [extremo_A, dist_1] = bfsFurthestNode(root);
        
        // 2. Buscamos el extremo B saliendo desde el extremo A
        auto [extremo_B, diametro] = bfsFurthestNode(extremo_A);

        return diametro;
    }

    
    /**
     * @brief Calcula el Diámetro de un garfo (La mayor distancia posible entre dos nodos).
     */
    static int getGraphDiameter(const GraphType& graph)
    {
        if (graph.getNodes().empty()) return 0;

        // Sub-función: BFS que devuelve la distancia máxima alcanzable desde UN nodo 'start'
        auto bfsMaxDistance = [&](NodePtr start) -> int {
            std::unordered_map<NodePtr, int> dist;
            std::queue<NodePtr> q;
            
            q.push(start);
            dist[start] = 0;
            
            int max_d = 0; // La distancia máxima encontrada para este inicio específico
            
            while (!q.empty()) {
                NodePtr u = q.front();
                q.pop();
                
                // Revisamos salidas
                for (const auto& arco : graph.getOutgoingArcs(u)) {
                    NodePtr v = arco->getTgtNode();
                    if (dist.find(v) == dist.end()) {
                        dist[v] = dist[u] + 1; // Un salto más
                        q.push(v);
                        max_d = std::max(max_d, dist[v]); // Actualizamos el récord local
                    }
                }
                // Revisamos entradas (Soporte para grafos No Dirigidos)
                for (const auto& arco : graph.getIncomingArcs(u)) {
                    NodePtr v = arco->getSrcNode();
                    if (dist.find(v) == dist.end()) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                        max_d = std::max(max_d, dist[v]); 
                    }
                }
            }
            return max_d;
        };

        int diametro_global = 0;

        // LA FUERZA BRUTA: Lanzamos el BFS desde TODOS los nodos del mapa
        for (const auto& nodo : graph.getNodes()) {
            int diametro_local = bfsMaxDistance(nodo);
            
            // Si este nodo logró llegar más lejos que el campeón anterior, tenemos nuevo récord
            diametro_global = std::max(diametro_global, diametro_local);
        }

        return diametro_global;
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
    static std::vector<NodePtr> getSortDFS(const GraphType& graph)
    {
        // TODO: Implement Topological Sort using Kahn's Algorithm. Throw an exception on cycle.
        if (GraphProperties<GraphType>::hasCycle(graph)) {
            throw std::runtime_error("Error: El grafo tiene un ciclo. No se puede ordenar topologicamente.");
        }

        std::vector<NodePtr> orden_topologico;
        std::unordered_set<NodePtr> visitados;
        std::function<void(NodePtr)> dfs_topo = [&](NodePtr actual) 
        {
            visitados.insert(actual); 
            for (const auto& arco : graph.getOutgoingArcs(actual)) 
            {
                NodePtr vecino = arco->getTgtNode();
                if (visitados.find(vecino) == visitados.end()) 
                {
                    dfs_topo(vecino); 
                }
            }
            orden_topologico.push_back(actual);
        };
        for (const auto& nodo : graph.getNodes()) 
        {
            if (visitados.find(nodo) == visitados.end()) 
            {
                dfs_topo(nodo);
            }
        }
        std::reverse(orden_topologico.begin(), orden_topologico.end());
        return orden_topologico;

        //throw std::logic_error("getSort is not implemented yet!");
    }

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
    }


    /**
     * @brief Obtiene los nodos que pertenecen a un rango especifico de niveles topologicos.
     * Ejemplo: getNodesInRankRange(grafo, 1, 3) devuelve las materias del semestre 1, 2 y 3.
     */
    static std::vector<NodePtr> getNodesInRankRange(const GraphType& graph, int min_rank, int max_rank)
    {
        auto ranks = GraphTopological<GraphType>::getRanks(graph);
        std::vector<NodePtr> filtrados;

        for (const auto& [nodo, rank] : ranks) {
            if (rank >= min_rank && rank <= max_rank) {
                filtrados.push_back(nodo);
            }
        }

        // ¡EL ARREGLO MÁGICO! Ordenamos el vector basándonos en su rango
        std::sort(filtrados.begin(), filtrados.end(), [&](NodePtr a, NodePtr b) {
            return ranks[a] < ranks[b]; 
        });

        return filtrados;
    }

    /**
     * @brief Encuentra TODOS los caminos simples posibles desde un origen hasta un destino.
     */
    static std::vector<std::vector<NodePtr>> getAllPaths(const GraphType& graph, NodePtr start, NodePtr end)
    {
        std::vector<std::vector<NodePtr>> todos_los_caminos;
        std::vector<NodePtr> camino_actual;
        std::unordered_set<NodePtr> visitados;

        std::function<void(NodePtr)> dfs = [&](NodePtr u) {
            camino_actual.push_back(u);

            if (u == end) {
                // Si llegamos al destino, guardamos la foto del camino
                todos_los_caminos.push_back(camino_actual);
            } else {
                visitados.insert(u); // Lo bloqueamos para no hacer ciclos
                for (const auto& arco : graph.getOutgoingArcs(u)) {
                    NodePtr v = arco->getTgtNode();
                    if (visitados.find(v) == visitados.end()) {
                        dfs(v);
                    }
                }
                for (const auto& arco : graph.getIncomingArcs(u)) {
                    NodePtr v = arco->getSrcNode();
                    if (visitados.find(v) == visitados.end()) dfs(v);
                }
                visitados.erase(u); // Backtracking: lo liberamos al retroceder
            }
            camino_actual.pop_back(); // Quitamos el nodo actual de la mochila
        };

        dfs(start);
        return todos_los_caminos;
    }


    /**
     * @brief BFS Multi-Origen. Calcula la distancia minima desde un GRUPO de nodos iniciales
     * a todos los demas nodos del grafo.
     */
    static std::unordered_map<NodePtr, int> multiSourceBFS(const GraphType& graph, const std::vector<NodePtr>& sources)
    {
        std::unordered_map<NodePtr, int> distancias;
        std::queue<NodePtr> q;

        // Inicializamos los origenes con distancia 0 y los metemos todos a la cola
        for (const auto& src : sources) {
            distancias[src] = 0;
            q.push(src);
        }

        while (!q.empty()) {
            NodePtr actual = q.front();
            q.pop();

            for (const auto& arco : graph.getOutgoingArcs(actual)) {
                NodePtr vecino = arco->getTgtNode();
                
                // Si no lo hemos visitado
                if (distancias.find(vecino) == distancias.end()) {
                    distancias[vecino] = distancias[actual] + 1;
                    q.push(vecino);
                }
            }
            for (const auto& arco : graph.getIncomingArcs(actual)) {
                NodePtr vecino = arco->getSrcNode();
                if (distancias.find(vecino) == distancias.end()) {
                    distancias[vecino] = distancias[actual] + 1;
                    q.push(vecino);
                }
            }
        }
        return distancias;
    }

    /**
     * @brief Encuentra "Puentes" (Aristas criticas que si se eliminan desconectan el grafo).
     * Devuelve una lista de pares de nodos que representan esas aristas.
     */
    static std::vector<std::pair<NodePtr, NodePtr>> findBridges(const GraphType& graph)
    {
        std::vector<std::pair<NodePtr, NodePtr>> puentes;
        std::unordered_map<NodePtr, int> discovery_time;
        std::unordered_map<NodePtr, int> lowest_reachable;
        int time = 0;

        std::function<void(NodePtr, NodePtr)> dfs_tarjan = [&](NodePtr u, NodePtr padre) {
            discovery_time[u] = lowest_reachable[u] = ++time;

            // Recolectar vecinos (Soporte total para UndirectedGraph)
            std::unordered_set<NodePtr> vecinos;
            for (const auto& arco : graph.getOutgoingArcs(u)) vecinos.insert(arco->getTgtNode());
            for (const auto& arco : graph.getIncomingArcs(u)) vecinos.insert(arco->getSrcNode());

            for (NodePtr v : vecinos) {
                if (v == padre) continue; // No mirar por donde vinimos

                if (discovery_time.find(v) != discovery_time.end()) {
                    // Back-edge encontrada: actualizar low de 'u'
                    lowest_reachable[u] = std::min(lowest_reachable[u], discovery_time[v]);
                } else {
                    // Nodo no visitado
                    dfs_tarjan(v, u);
                    lowest_reachable[u] = std::min(lowest_reachable[u], lowest_reachable[v]);

                    // Condicion de puente: Si el vecino no puede alcanzar un nodo anterior a 'u'
                    if (lowest_reachable[v] > discovery_time[u]) {
                        puentes.push_back({u, v});
                    }
                }
            }
        };

        for (const auto& nodo : graph.getNodes()) {
            if (discovery_time.find(nodo) == discovery_time.end()) {
                dfs_tarjan(nodo, nullptr);
            }
        }
        return puentes;
    }



    /**
     * @brief Computes topological ranks (depth levels) for each node.
     * @param graph The directed acyclic graph to analyze.
     * @return std::unordered_map<NodePtr, int> A map from nodes to their ranks.
     * @throws std::runtime_error If the graph contains a cycle.
     */
    static std::unordered_map<NodePtr, int> getRanks(const GraphType& graph)
    {
        // TODO: Compute ranks. Nodes with no dependencies have rank 0, their successors have rank 1...
        
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
        //throw std::logic_error("getRanks is not implemented yet!");
    }
};


/**
 * @brief Provee métodos para transformar grafos en nuevas estructuras.
 */
template <typename GraphType>
class GraphTransformations
{
public:
    using NodePtr = typename GraphType::NodePtr;

    /**
     * @brief Calcula la transpuesta de un grafo dirigido (invierte todas las aristas).
     */
    static GraphType getTransposedGraph(const GraphType& graph)
    {
        GraphType transpuesto;
        
        // 1. Copiamos todos los nodos
        for (const auto& nodo : graph.getNodes()) {
            transpuesto.addNode(nodo->getValue());
        }

        // 2. Mapeamos los nodos originales con los nuevos para reconectar
        std::unordered_map<NodePtr, NodePtr> mapa_nodos;
        for (const auto& nodo_orig : graph.getNodes()) {
            for (const auto& nodo_nuevo : transpuesto.getNodes()) {
                if (nodo_orig->getValue() == nodo_nuevo->getValue()) {
                    mapa_nodos[nodo_orig] = nodo_nuevo;
                    break;
                }
            }
        }

        // 3. Invertimos las aristas
        for (const auto& nodo_orig : graph.getNodes()) {
            for (const auto& arco : graph.getOutgoingArcs(nodo_orig)) {
                NodePtr destino_orig = arco->getTgtNode();
                
                NodePtr nuevo_origen = mapa_nodos[destino_orig];
                NodePtr nuevo_destino = mapa_nodos[nodo_orig];
                
                transpuesto.addArc(nuevo_origen, nuevo_destino, arco->getWeight());
            }
        }
        return transpuesto;
    }

    /**
     * @brief Genera el grafo al cuadrado G^2 (Conecta nodos con caminos de longitud 2).
     */
    static GraphType getSquaredGraph(const GraphType& graph)
    {
        // 1. Extraemos el tipo de peso del grafo
        using WeightType = typename GraphType::ArcWeightType; 
        
        GraphType cuadrado;
        std::unordered_map<NodePtr, NodePtr> mapa_nodos;

        // 2. Clonamos todos los nodos en el nuevo grafo
        for (const auto& nodo : graph.getNodes()) {
            NodePtr nuevo = cuadrado.addNode(nodo->getValue());
            mapa_nodos[nodo] = nuevo;
        }

        // --- LA MEJORA: Nuestro Radar 360° en una pequeña función ---
        // Nos devuelve una lista de parejas: {Vecino_Real, Peso_Del_Arco}
        auto obtener_vecinos_reales = [&](NodePtr actual) {
            std::vector<std::pair<NodePtr, WeightType>> vecinos;
            
            // Revisamos Salidas
            for (const auto& arco : graph.getOutgoingArcs(actual)) {
                NodePtr vecino = arco->getTgtNode();
                if (vecino == actual) vecino = arco->getSrcNode(); // Protección de memoria
                vecinos.push_back({vecino, arco->getWeight()});
            }
            // Revisamos Entradas
            for (const auto& arco : graph.getIncomingArcs(actual)) {
                NodePtr vecino = arco->getSrcNode();
                if (vecino == actual) vecino = arco->getTgtNode(); // Protección de memoria
                vecinos.push_back({vecino, arco->getWeight()});
            }
            return vecinos;
        };
        // -------------------------------------------------------------

        // 3. Construimos el Cuadrado usando nuestro radar
        for (const auto& u : graph.getNodes()) {
            
            // Miramos a los amigos de 1er nivel
            auto amigos_directos = obtener_vecinos_reales(u);
            
            for (const auto& [v, peso_uv] : amigos_directos) {
                
                // Mantenemos la calle original
                cuadrado.addArc(mapa_nodos[u], mapa_nodos[v], peso_uv);

                // Miramos a los amigos del amigo (2do nivel)
                auto amigos_de_mi_amigo = obtener_vecinos_reales(v);

                for (const auto& [w, peso_vw] : amigos_de_mi_amigo) {
                    
                    // Si el amigo de mi amigo NO soy yo mismo...
                    if (u != w) { 
                        
                        // SOLUCIÓN MÁGICA: Creamos el atajo directo
                        if constexpr (!std::is_same_v<WeightType, NoWeight>) {
                            cuadrado.addArc(mapa_nodos[u], mapa_nodos[w], peso_uv + peso_vw);
                        } else {
                            cuadrado.addArc(mapa_nodos[u], mapa_nodos[w], NoWeight{});
                        }
                    }
                }
            }
        }
        
        return cuadrado;
    }
};

/**
 * @brief Herramientas para Administracion de Proyectos (Metodo PERT/CPM).
 */
template <typename GraphType>
class ProjectManagementCPM
{
public:
    using NodePtr = typename GraphType::NodePtr;
    using WeightType = typename GraphType::ArcWeightType;

    /**
     * @brief Calcula los Tiempos Tempranos (tn) usando Orden Topologico.
     */
    static std::unordered_map<NodePtr, WeightType> calculateEarlyTimes(const GraphType& graph)
    {
        std::unordered_map<NodePtr, WeightType> tn;
        std::unordered_map<NodePtr, size_t> in_degree;
        std::queue<NodePtr> cola;

        for (const auto& nodo : graph.getNodes()) {
            tn[nodo] = WeightType{}; // Inicializar en 0
            in_degree[nodo] = graph.getInDegree(nodo);
            if (in_degree[nodo] == 0) cola.push(nodo);
        }

        size_t procesados = 0;
        while (!cola.empty()) {
            NodePtr actual = cola.front();
            cola.pop();
            procesados++;

            for (const auto& arco : graph.getOutgoingArcs(actual)) {
                NodePtr vecino = arco->getTgtNode();
                WeightType peso = arco->getWeight();

                // tn(v) es el máximo de (tn(u) + peso)
                if (tn[actual] + peso > tn[vecino]) {
                    tn[vecino] = tn[actual] + peso;
                }

                in_degree[vecino]--;
                if (in_degree[vecino] == 0) cola.push(vecino);
            }
        }

        if (procesados != graph.getNodes().size()) {
            throw std::runtime_error("El proyecto tiene un ciclo. No se pueden calcular los tiempos.");
        }
        return tn;
    }

    /**
     * @brief Calcula los Tiempos Limites/Tardios (tl) recorriendo en reversa.
     */
    static std::unordered_map<NodePtr, WeightType> calculateLateTimes(const GraphType& graph, const std::unordered_map<NodePtr, WeightType>& tn)
    {
        std::unordered_map<NodePtr, WeightType> tl;
        // Obtenemos el orden topológico y lo invertimos
        auto orden = GraphTopological<GraphType>::getSort(graph);
        std::reverse(orden.begin(), orden.end());

        // Un "Infinito" seguro para el tipo de dato
        const WeightType INFINITO = std::numeric_limits<WeightType>::max() / 2;
        
        for (const auto& nodo : graph.getNodes()) tl[nodo] = INFINITO;

        for (const auto& actual : orden) {
            auto salidas = graph.getOutgoingArcs(actual);
            
            if (salidas.empty()) {
                tl[actual] = tn.at(actual); // Nodo final
            } else {
                for (const auto& arco : salidas) {
                    NodePtr sucesor = arco->getTgtNode();
                    WeightType tiempo_posible = tl[sucesor] - arco->getWeight();
                    if (tiempo_posible < tl[actual]) {
                        tl[actual] = tiempo_posible;
                    }
                }
            }
        }
        return tl;
    }

    /**
     * @brief Encuentra todas las Rutas Criticas (holgura cero) de un proyecto.
     * @param graph El grafo del proyecto.
     * @param tn Diccionario de tiempos tempranos.
     * @param tl Diccionario de tiempos tardios.
     * @return std::vector<std::vector<NodePtr>> Lista de todas las rutas criticas.
     */
    static std::vector<std::vector<NodePtr>> getCriticalPaths(
        const GraphType& graph,
        const std::unordered_map<NodePtr, WeightType>& tn,
        const std::unordered_map<NodePtr, WeightType>& tl)
    {
        std::vector<std::vector<NodePtr>> rutas_criticas;
        std::vector<NodePtr> ruta_actual;

        // Función recursiva (DFS + Backtracking)
        std::function<void(NodePtr)> dfs_critico = [&](NodePtr u) {
            ruta_actual.push_back(u);
            auto salidas = graph.getOutgoingArcs(u);

            if (salidas.empty()) {
                // Llegamos al fin del proyecto, guardamos la ruta
                rutas_criticas.push_back(ruta_actual);
            } else {
                for (const auto& arco : salidas) {
                    NodePtr v = arco->getTgtNode();
                    WeightType peso = arco->getWeight();

                    // Filtro Crítico: 
                    // 1. El sucesor debe ser crítico (tn == tl)
                    // 2. La suma del tiempo debe encajar perfectamente
                    if (tn.at(v) == tl.at(v) && (tn.at(u) + peso == tn.at(v))) {
                        dfs_critico(v);
                    }
                }
            }
            ruta_actual.pop_back(); // Damos un paso atrás
        };

        // Disparamos el DFS solo desde los nodos iniciales que sean críticos
        for (const auto& nodo : graph.getNodes()) {
            if (graph.getInDegree(nodo) == 0 && tn.at(nodo) == tl.at(nodo)) {
                dfs_critico(nodo);
            }
        }

        return rutas_criticas;
    }



};

/**
 * @brief Algoritmos para caminos complejos y problemas específicos (Euler, Hamilton, Bosques).
 */
template <typename GraphType>
class AdvancedPaths
{
public:
    using NodePtr = typename GraphType::NodePtr;

    /**
     * @brief Encuentra un Circuito de Euler usando el Algoritmo de Hierholzer.
     * @return std::vector<NodePtr> El camino exacto del circuito.
     * @throws std::runtime_error si el grafo no cumple las condiciones de Euler.
     */
    static std::vector<NodePtr> getEulerianCircuit(const GraphType& graph)
    {
        NodePtr start = nullptr;

        // 1. Validar que In-Degree == Out-Degree para todo nodo
        for (const auto& nodo : graph.getNodes()) {
            if (graph.getInDegree(nodo) != graph.getOutDegree(nodo)) {
                throw std::runtime_error("No existe Circuito de Euler: Grados de entrada y salida no coinciden.");
            }
            if (graph.getOutDegree(nodo) > 0 && start == nullptr) {
                start = nodo; // Tomamos el primer nodo con conexiones como punto de partida
            }
        }

        if (start == nullptr) return {}; // Grafo sin aristas

        // 2. Ejecutar Hierholzer
        std::unordered_map<NodePtr, size_t> edge_idx; // Indice de la arista actual por nodo
        for(const auto& n : graph.getNodes()) edge_idx[n] = 0;

        std::vector<NodePtr> circuito;
        
        std::function<void(NodePtr)> dfs_euler = [&](NodePtr u) {
            const auto& aristas = graph.getOutgoingArcs(u);
            
            while (edge_idx[u] < aristas.size()) {
                NodePtr v = aristas[edge_idx[u]]->getTgtNode();
                edge_idx[u]++; // Avanzamos el índice (Tachamos la arista)
                dfs_euler(v);
            }
            circuito.push_back(u); // Backtracking
        };

        dfs_euler(start);
        std::reverse(circuito.begin(), circuito.end());

        // Verificar conexidad fuerte (que hayamos pisado todas las aristas)
        if (circuito.size() != graph.getArcs().size() + 1) {
            throw std::runtime_error("El grafo no es fuertemente conexo (aristas inalcanzables).");
        }

        return circuito;
    }

    /**
     * @brief Encuentra TODOS los caminos Hamiltonianos del grafo usando Backtracking.
     */
    static std::vector<std::vector<NodePtr>> getAllHamiltonianPaths(const GraphType& graph)
    {
        std::vector<std::vector<NodePtr>> todos_los_caminos;
        std::vector<NodePtr> camino_actual;
        std::unordered_set<NodePtr> visitados;
        size_t total_nodos = graph.getNodes().size();

        std::function<void(NodePtr)> dfs_hamilton = [&](NodePtr u) {
            visitados.insert(u);
            camino_actual.push_back(u);

            if (camino_actual.size() == total_nodos) {
                todos_los_caminos.push_back(camino_actual);
            } else {
                for (const auto& arco : graph.getOutgoingArcs(u)) {
                    NodePtr v = arco->getTgtNode();
                    if (visitados.find(v) == visitados.end()) {
                        dfs_hamilton(v);
                    }
                }
                for (const auto& arco : graph.getIncomingArcs(u)) {
                    NodePtr v = arco->getSrcNode();
                    if (visitados.find(v) == visitados.end()) {
                        dfs_hamilton(v);
                    }
                }

            }

            // Backtracking: liberamos el nodo para probar otras rutas
            visitados.erase(u);
            camino_actual.pop_back();
        };

        // Lanzamos la búsqueda desde cada nodo como punto de inicio potencial
        for (const auto& nodo : graph.getNodes()) {
            dfs_hamilton(nodo);
        }

        return todos_los_caminos;
    }

    /**
     * @brief Resuelve el problema "Even Tree / Even Forest" (HackerRank).
     * Calcula cuántos cortes de aristas se pueden hacer en un árbol para obtener un bosque
     * donde cada árbol resultante tenga un número par de nodos.
     * @param graph El árbol no dirigido.
     * @param root El nodo desde el cual "colgar" el árbol para el DFS.
     */
    static int calculateEvenForestCuts(const GraphType& graph, NodePtr root)
    {
        int cortes_totales = 0;
        std::unordered_set<NodePtr> visitados;

        std::function<int(NodePtr)> dfs_cortes = [&](NodePtr u) -> int {
            visitados.insert(u);
            int nodos_en_subarbol = 1; // Me cuento a mí mismo

            for (const auto& arco : graph.getOutgoingArcs(u)) {
                NodePtr vecino = arco->getTgtNode();
                
                if (visitados.find(vecino) == visitados.end()) {
                    int nodos_rama_hija = dfs_cortes(vecino);
                    
                    if (nodos_rama_hija % 2 == 0) {
                        cortes_totales++; // Cortamos esta rama porque ya es par por sí misma
                    } else {
                        nodos_en_subarbol += nodos_rama_hija; // No podemos cortar, nos sumamos a la rama padre
                    }
                }
            }
            for (const auto& arco : graph.getIncomingArcs(u)) {
                NodePtr vecino = arco->getSrcNode();
                
                if (visitados.find(vecino) == visitados.end()) {
                    int nodos_rama_hija = dfs_cortes(vecino);
                    
                    if (nodos_rama_hija % 2 == 0) {
                        cortes_totales++; // Cortamos esta rama
                    } else {
                        nodos_en_subarbol += nodos_rama_hija; // Nos sumamos
                    }
                }
            }

            return nodos_en_subarbol;
        };

        dfs_cortes(root);
        return cortes_totales;
    }
};


/**
 * @brief Estructura Union-Find (DSU) con Path Compression y Union by Rank.
 * Es la forma mas rapida de agrupar nodos y detectar ciclos en grafos no dirigidos.
 */

template <typename GraphType>

class DSU {
private:
    using NodePtr = typename GraphType::NodePtr;    
    std::unordered_map<NodePtr, NodePtr> padre;
    std::unordered_map<NodePtr, int> rango;
    int conjuntos_activos;

public:
    
    DSU(const std::unordered_set<NodePtr>& nodos) {
        conjuntos_activos = nodos.size();
        for (const auto& nodo : nodos) {
            padre[nodo] = nodo; // Al principio, cada nodo es su propio jefe
            rango[nodo] = 0;
        }
    }

    // Encuentra al "Jefe Supremo" de un nodo (con compresion de caminos)
    NodePtr find(NodePtr i) {
        if (padre[i] == i) return i;
        return padre[i] = find(padre[i]); 
    }

    // Une dos grupos. Devuelve FALSE si ya estaban en el mismo grupo (¡CICLO DETECTADO!)
    bool unite(NodePtr i, NodePtr j) {
        NodePtr raiz_i = find(i);
        NodePtr raiz_j = find(j);

        if (raiz_i != raiz_j) {
            // El grupo mas pequeño se une al mas grande (Union by Rank)
            if (rango[raiz_i] < rango[raiz_j]) {
                padre[raiz_i] = raiz_j;
            } else if (rango[raiz_i] > rango[raiz_j]) {
                padre[raiz_j] = raiz_i;
            } else {
                padre[raiz_j] = raiz_i;
                rango[raiz_i]++;
            }
            conjuntos_activos--;
            return true;
        }
        return false; // Ya estaban unidos
    }

    int getNumConjuntos() const { return conjuntos_activos; }


    /**
     * @brief Algoritmo de Kruskal para encontrar el Arbol de Expansion Minima (MST).
     * Conecta todos los nodos del grafo asegurando que la suma de los pesos sea la menor posible.
     */
    static GraphType getMinimumSpanningTreeKruskal(const GraphType& graph)
    {
        using ArcPtr = typename GraphType::ArcPtr;
        GraphType mst;
        
        // 1. Copiamos todos los nodos al nuevo grafo
        for (const auto& nodo : graph.getNodes()) {
            mst.addNode(nodo->getValue());
        }

        // 2. Extraemos TODAS las aristas y las ordenamos por peso (de menor a mayor)
        std::vector<ArcPtr> todas_las_aristas;
        for (const auto& arco : graph.getArcs()) {
            todas_las_aristas.push_back(arco);
        }

        std::sort(todas_las_aristas.begin(), todas_las_aristas.end(), 
            [](const ArcPtr& a, const ArcPtr& b) {
                return a->getWeight() < b->getWeight();
            });

        // 3. Usamos nuestro DSU magico para evitar hacer ciclos
        DSU<GraphType> dsu(graph.getNodes());
        int aristas_agregadas = 0;
        int nodos_necesarios = graph.getNodes().size() - 1;

        for (const auto& arco : todas_las_aristas) {
            NodePtr u = arco->getSrcNode();
            NodePtr v = arco->getTgtNode();

            // Si unirlos NO forma un ciclo, agregamos la arista al MST
            if (dsu.unite(u, v)) {
                
                // Buscamos los nodos equivalentes en el nuevo grafo MST para conectarlos
                NodePtr mst_u, mst_v;
                for(auto n : mst.getNodes()) {
                    if (n->getValue() == u->getValue()) mst_u = n;
                    if (n->getValue() == v->getValue()) mst_v = n;
                }

                mst.addArc(mst_u, mst_v, arco->getWeight());
                aristas_agregadas++;

                // Un MST siempre tiene exactamente (Vertices - 1) aristas
                if (aristas_agregadas == nodos_necesarios) break;
            }
        }

        return mst;
    }


};



enum Color { BLANCO, GRIS, NEGRO };
template <typename GraphType>
class CycleDetector {
    public:
    using NodePtr = typename GraphType::NodePtr;
    private:
        std::unordered_map<NodePtr, Color> colores;
        std::unordered_map<NodePtr, NodePtr> padre;
        std::vector<NodePtr> ciclo_encontrado;

        // Función recursiva DFS
        bool explorarDFS(NodePtr actual, const GraphType& graph) {
            
            // 1. Marcamos el nodo actual como GRIS (lo estamos visitando)
            colores[actual] = GRIS;

            // 2. Exploramos a los vecinos
            for (const auto& arco : graph.getOutgoingArcs(actual)) {
                NodePtr vecino = arco->getTgtNode();

                // Si el vecino es BLANCO, lo visitamos
                if (colores[vecino] == BLANCO) {
                    padre[vecino] = actual; // Dejamos la migaja de pan
                    
                    // Si el hijo encuentra un ciclo, detenemos todo y retornamos true
                    if (explorarDFS(vecino, graph)) {
                        return true; 
                    }
                } 
                // 3. ¡EL CHOQUE MÁGICO! Si el vecino es GRIS, encontramos el ciclo
                else if (colores[vecino] == GRIS) {
                    
                    // Empezamos a reconstruir el ciclo hacia atrás usando los padres
                    ciclo_encontrado.push_back(vecino); // Nodo donde cerró el ciclo
                    
                    NodePtr rastreador = actual;
                    while (rastreador != vecino) {
                        ciclo_encontrado.push_back(rastreador);
                        rastreador = padre[rastreador];
                    }
                    ciclo_encontrado.push_back(vecino); // Lo repetimos al final para cerrar el círculo (Ej: A->B->C->A)
                    
                    // Como lo reconstruimos de atrás hacia adelante, le damos la vuelta
                    std::reverse(ciclo_encontrado.begin(), ciclo_encontrado.end());
                    
                    return true; // Avisamos que ya terminamos
                }
            }

            // 4. Si exploramos todos los vecinos y no hubo ciclos, pintamos de NEGRO
            colores[actual] = NEGRO;
            return false;
        }

    public:
        
        std::vector<NodePtr> getExactCyclePath(const GraphType& graph) {
            
            // Inicializamos todos los nodos en BLANCO
            for (const auto& nodo : graph.getNodes()) {
                colores[nodo] = BLANCO;
            }

            // Lanzamos el DFS desde cada nodo (por si el grafo tiene partes desconectadas)
            for (const auto& nodo : graph.getNodes()) {
                if (colores[nodo] == BLANCO) {
                    if (explorarDFS(nodo, graph)) {
                        // Si explorarDFS devuelve true, 'ciclo_encontrado' ya tiene nuestra respuesta
                        return ciclo_encontrado; 
                    }
                }
            }

            // Si termina el for y no retornó, devolvemos un vector vacío (no hay ciclos)
            return {}; 
    }
};


























} // namespace graph_lib