/**
 * @file template.cpp
 * @brief Examen Práctico: Algoritmos Avanzados en Grafos y Concurrencia.
 *
 * INSTRUCCIONES:
 * - Completa las funciones marcadas con "TODO".
 * - NO modifiques las firmas de las funciones existentes.
 * - NO modifiques el bloque "main" ni la función "findNode".
 * - Puedes agregar funciones auxiliares (helpers) si lo consideras necesario.
 *
 * REGLAS IMPORTANTES:
 * - EJERCICIO 1: Si el grafo es estrictamente un árbol (o no tiene aristas de reemplazo
 *   que mantengan el grafo conectado), debes retornar -1.0.
 * - EJERCICIO 2 (Segundo Punto): Para evitar deadlocks, adquiere los mutexes en orden global
 *   (por ID de cuenta) y usa std::lock_guard.
 */

#include "lib/graph.hpp"
#include "lib/graph_algorithms.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <map>
#include <fstream>
#include <thread>
#include <stdexcept>
#include <cmath>

using namespace std;
using namespace graph_lib;

// ========================================================================
// FUNCIONES AUXILIARES (NO MODIFICAR)
// ========================================================================

/**
 * @brief Encuentra y devuelve el puntero a un nodo dado su valor string.
 */
template <typename GraphType>
std::shared_ptr<graph_lib::Node<std::string>> findNode(const GraphType& graph, const std::string& val) {
    for (const auto& node : graph.getNodes()) {
        if (node->getValue() == val) return node;
    }
    return nullptr;
}

/**
 * @brief Estructura que representa una cuenta bancaria con control de concurrencia.
 */
struct BankAccount {
    int id;
    long balance;
    std::mutex mtx;   // Mutex para proteger modificaciones al saldo

    BankAccount(int id, long balance) : id(id), balance(balance) {}

    // Deshabilitar copia para evitar problemas con std::mutex
    BankAccount(const BankAccount&) = delete;
    BankAccount& operator=(const BankAccount&) = delete;
};

// ========================================================================
// EJERCICIO 1: El Plan de Contingencia (Second-best MST)
// ========================================================================

/**
 * @brief Encuentra el peso (costo total) del Segundo Mejor Árbol de Recubrimiento Mínimo.
 *
 * @param graph El grafo original no dirigido y ponderado.
 * @return double El peso total del segundo mejor MST.
 *
 * REGLA ESTRICTA: Si el grafo es estrictamente un árbol (o no tiene aristas de reemplazo
 *                 que mantengan el grafo conectado), debes retornar -1.0.
 *
 * HINT 1: Usa Kruskal o Prim para encontrar el MST primario.
 * HINT 2: Itera sobre cada arista del MST primario. Crea un subgrafo temporal excluyendo
 *         esa arista, calcula el MST de ese subgrafo y quédate con el de menor peso total.
 * HINT 3: Para el cálculo del MST en cada iteración, puedes reutilizar el mismo algoritmo.
 */
double getSecondBestMSTWeight(const UndirectedGraph<string, double>& graph) {
    // TODO: Implementar el algoritmo para encontrar el Second-best MST.
    
    using NodePtr = std::shared_ptr<Node<std::string>>;
    using ArcPtr = std::shared_ptr<Arc<std::string, double>>;

    size_t nodes = graph.getNodes().size();
    
    if (nodes <= 1) return -1.0;

    std::vector<ArcPtr> caminos;
    
    for(auto& arc : graph.getArcs())
    {
        caminos.push_back(arc);
    }
    
    std::sort(caminos.begin(), caminos.end(), [](const ArcPtr& a, const ArcPtr& b) 
    {
        return a->getWeight() < b->getWeight();
    });

    graph_lib::DSU<UndirectedGraph<string, double>> dsu1(graph.getNodes());
    std::vector<ArcPtr> mst_edges;
    
    for(auto& edge : caminos)
    {
        if(dsu1.unite(edge->getSrcNode(), edge->getTgtNode()))
        {
            mst_edges.push_back(edge);
        }
    }

    if(mst_edges.size() != nodes - 1)
    {
        return -1.0; 
    }

    double min = std::numeric_limits<double>::max();
    bool found = false;

    for(auto& excluido : mst_edges)
    {
        graph_lib::DSU<UndirectedGraph<string, double>> dsu(graph.getNodes());
        double current_weight = 0;
        size_t camino_añadir = 0;

        for(auto& camino : caminos)
        {
            if (camino == excluido) continue; 

            if(dsu.unite(camino->getSrcNode(), camino->getTgtNode()))
            {
                current_weight += camino->getWeight();
                camino_añadir++;
                
                if (camino_añadir == nodes - 1) break;
            }
        }

        if(camino_añadir == nodes - 1)
        {
            found = true;
            min = std::min(min, current_weight);
        }
    }

    if(!found) return -1.0;

    return min;
    //throw std::logic_error("NOT_IMPLEMENTED");
}

// ========================================================================
// EJERCICIO 2: Núcleo Transaccional "GraphBank"
// ========================================================================

/**
 * @brief Liquidación de Deudas (Netting)
 *
 * @param debt_graph Grafo dirigido donde el peso es la deuda de Origen hacia Destino.
 * @return std::string Lista de transferencias mínimas en formato "Origen->Destino:Monto "
 *
 * HINT: Calcula el balance neto de cada nodo (suma de entradas - suma de salidas).
 *       Luego, empareja a los nodos con balance negativo (deudores) con los de balance positivo (acreedores).
 *       El resultado debe ser una cadena con todas las transferencias necesarias, donde cada transferencia
 *       tiene el formato "Origen->Destino:Monto" y las transferencias se separan con un espacio.
 *       No debe haber espacio al final de la cadena. Por ejemplo: "EmpA->EmpC:150 EmpB->EmpC:75".
 */
std::string executeNetting(const DirectedGraph<string, double>& debt_graph) {
    // TODO: Implementar el algoritmo de compensación de deudas (Netting).
    std::map<std::string, double> net_balances; 
    
    int infinito = 1e-9;

    for(auto& node : debt_graph.getNodes())
    {
        net_balances[node->getValue()] = 0.0;
    }

    for(auto& arc : debt_graph.getArcs())
    {
        std::string src = arc->getSrcNode()->getValue();
        std::string tgt = arc->getTgtNode()->getValue();
        double amount = arc->getWeight();
        net_balances[src] -= amount;
        net_balances[tgt] += amount;
    }

    std::vector<std::pair<std::string, double>> debtors;
    std::vector<std::pair<std::string, double>> creditors;

    for(const auto& pair : net_balances)
    {
        if(pair.second < -infinito) 
        {
        	debtors.push_back({pair.first, -pair.second});
        } else 
        if(pair.second > infinito)
        {
        	creditors.push_back({pair.first, pair.second});
        }
    }

    std::string result = "";
    size_t d = 0, c = 0;
    
    while(d < debtors.size() && c < creditors.size())
    {
        double amount = std::min(debtors[d].second, creditors[c].second);
        
        std::string formatted_amount;
        
        if(std::abs(amount - std::round(amount)) < 1e-6)
        {
            formatted_amount = std::to_string((long long)std::round(amount));
        } else {
        
            formatted_amount = std::to_string(amount);
            formatted_amount.erase(formatted_amount.find_last_not_of('0') + 1, std::string::npos);
            if (formatted_amount.back() == '.') formatted_amount.pop_back();
        }

        result += debtors[d].first + "->" + creditors[c].first + ":" + formatted_amount + " ";
        debtors[d].second -= amount;
        creditors[c].second -= amount;
        
        if (debtors[d].second < 1e-9) d++;
        if (creditors[c].second < 1e-9) c++;
    }

    if(!result.empty()) result.pop_back();
    
    return result;
    //throw std::logic_error("NOT_IMPLEMENTED");
}

/**
 * @brief Motor de Transferencias Concurrentes Seguras
 *
 * @param accounts Mapa de IDs de cuenta a punteros de BankAccount.
 * @param from_id ID de la cuenta origen.
 * @param to_id ID de la cuenta destino.
 * @param amount Cantidad a transferir.
 *
 * CRÍTICO: Múltiples hilos llamarán a esta función al mismo tiempo.
 *
 * HINT: Usa std::lock_guard. Para evitar Interbloqueos (Deadlocks) cuando ocurren
 *       transferencias cruzadas (A->B y B->A simultáneas), debes establecer un
 *       "Orden Global" al adquirir los locks. ¿Qué pasa si siempre bloqueas primero el ID menor?
 *       Utiliza std::lock o bloquea en orden creciente de id.
 *       Recuerda verificar que haya saldo suficiente antes de realizar la transferencia.
 */
 

void safeTransfer(std::map<int, std::unique_ptr<BankAccount>>& accounts, int from_id, int to_id, long amount) {
    // TODO: Implementar una transferencia atómica y Thread-Safe.
    // Elimina este return cuando implementes tu lógica.
    
    if (from_id == to_id) return;

    int first_id = std::min(from_id, to_id);
    int second_id = std::max(from_id, to_id);

    std::lock_guard<std::mutex> lock1(accounts[first_id]->mtx);
    std::lock_guard<std::mutex> lock2(accounts[second_id]->mtx);
}

// ========================================================================
// EXERCISE 3: Arbitrage
// ========================================================================

/**
 * @brief Detectar ciclos de arbitraje que generen ganacias al multiplicar las tasas de cambio a lo largo del ciclo.
 * @param graph Grafo dirigido donde el peso de cada arco representa la tasa de cambio de la moneda origen a la moneda destino.
 * @return std::vector<shared_ptr<Node<string>>> Vector con los nodos del ciclo de arbitraje.
 */
std::vector<shared_ptr<Node<string>>> detectArbitrage(const DirectedGraph<string, double>& graph) {
    // TODO: Implementar algoritmo
    
using NodePtr = std::shared_ptr<Node<std::string>>;
    
    std::unordered_map<NodePtr, double> distancia;
    std::unordered_map<NodePtr, NodePtr> padre;
    
    int infinito = 1e-9;
    
    for (const auto& node : graph.getNodes()) 
    {
        distancia[node] = 0.0;
        padre[node] = nullptr;
    }
    
    size_t tam = graph.getNodes().size();
    
    if(tam == 0) return {};

    NodePtr actualizado = nullptr;
    
    for(size_t i = 0; i < tam; ++i) 
    {
        actualizado = nullptr;
        
        for(const auto& arc : graph.getArcs()) 
        {
            NodePtr u = arc->getSrcNode();
            NodePtr v = arc->getTgtNode();
            double weight = -std::log(arc->getWeight());
            
            if(distancia[u] + weight < distancia[v] - infinito) 
            { 
                distancia[v] = distancia[u] + weight;
                padre[v] = u;
                actualizado = v;
            }
        }
    }
    
    if(actualizado == nullptr) return {}; 
    
    NodePtr aux = actualizado;
    
    for(size_t i = 0; i < tam; ++i) 
    {
        aux = padre[aux];
    }
    
    std::vector<NodePtr> cycle;
    NodePtr current = aux;
    
    while(true)
    {
        cycle.push_back(current);
        current = padre[current];
        
        if (current == aux) break;
    }
    
    cycle.push_back(aux);
    
    std::reverse(cycle.begin(), cycle.end());
    
    return cycle;
    
    //throw std::logic_error("NOT_IMPLEMENTED");
}

// ========================================================================
// I/O BOILERPLATE (DO NOT MODIFY)
// This section handles the communication between the Grader and your code.
// ========================================================================

int main(int argc, char* argv[]) {
    // Ensure an exercise identifier is provided
    if (argc < 2) {
        cerr << "Error: No exercise specified." << endl;
        return 1;
    }

    string exercise = argv[1];
    ofstream sol("solution.txt");

    try {
        // --- Exercise 1: Second-best MST ---
        if (exercise == "ex1") {
            auto g = UndirectedGraph<string, double>::loadFromFile("test_graph.txt");
            double res = getSecondBestMSTWeight(g);
            sol << res << "\n";
        }
        // --- Exercise 2: GraphBank (Netting & Concurrency) ---
        else if (exercise == "ex2") {
            ifstream params("params.txt");
            string mode;
            params >> mode;

            if (mode == "netting") {
                auto g = DirectedGraph<string, double>::loadFromFile("test_graph.txt");
                sol << executeNetting(g) << "\n";
            }
            else if (mode == "stress") {
                int num_threads, iterations;
                params >> num_threads >> iterations;

                // Initialize accounts with high balance to prevent negative results
                std::map<int, std::unique_ptr<BankAccount>> accounts;
                accounts[1] = std::make_unique<BankAccount>(1, 1000000);
                accounts[2] = std::make_unique<BankAccount>(2, 1000000);

                // Launch ASYMMETRIC concurrent transfers to detect empty implementations
                vector<thread> workers;
                for (int i = 0; i < num_threads; i++) {
                    workers.push_back(thread([&, i]() {
                        for(int j = 0; j < iterations; j++) {
                            // Even-indexed threads move $2, odd-indexed threads move $1 back
                            if (i % 2 == 0) safeTransfer(accounts, 1, 2, 2);
                            else safeTransfer(accounts, 2, 1, 1);
                        }
                    }));
                }

                for (auto& w : workers) w.join();

                // Calculate mathematically expected balances
                long half_threads = num_threads / 2;
                long expected_1 = 1000000 - (half_threads * iterations * 2) + (half_threads * iterations * 1);
                long expected_2 = 1000000 + (half_threads * iterations * 2) - (half_threads * iterations * 1);

                // Verify both consistency (money conservation) and movement accuracy
                if (accounts[1]->balance == expected_1 && accounts[2]->balance == expected_2) {
                    sol << "SUCCESS\n";
                } else {
                    sol << "CONSISTENCY_ERROR\n";
                }
            }
        }
        // --- Exercise 3: Arbitrage Detection ---
        else if (exercise == "ex3") {
            // ex3 is now at the same level as ex1 and ex2
            auto g = DirectedGraph<string, double>::loadFromFile("test_graph.txt");
            auto cycle = detectArbitrage(g);

            for (const auto& node : cycle) {
                sol << node->getValue() << " ";
            }
            sol << "\n";
        }
    }
    // Handle the specific logic_error used to indicate pending work
    catch (const std::logic_error& e) {
        sol << "NOT_IMPLEMENTED\n";
    }
    // Handle any unexpected runtime exceptions
    catch (const std::exception& e) {
        sol << "ERROR\n";
    }

    sol.close();
    return 0;
}
