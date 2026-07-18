#include "graph.hpp"
#include "graph_algorithms.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <fstream>
#include <stdexcept>

// ========================================================================
// HELPER FUNCTION (Provided for you)
// Finds a node in the graph by its string value.
// ========================================================================
template <typename GraphType>
std::shared_ptr<graph_lib::Node<std::string>> findNode(const GraphType& graph, const std::string& val)
{
    for (const auto& node : graph.getNodes())
    {
        if (node->getValue() == val) return node;
    }
    return nullptr;
}

// ========================================================================
// EXERCISE 1: The Main Event (Wrestling Rivalries)
// ========================================================================
bool classifyWrestlers(
    const graph_lib::UndirectedGraph<std::string>& roster,
    std::unordered_set<std::shared_ptr<graph_lib::Node<std::string>>>& faces,
    std::unordered_set<std::shared_ptr<graph_lib::Node<std::string>>>& heels)
{
    // TODO: Implement your logic here using BFS or DFS.
    // Return true if it's possible to split them without conflicts.
    // Populate 'faces' and 'heels' sets.

    return false;
}

// ========================================================================
// EXERCISE 2: The Phantom Heist (Strict Waypoints)
// ========================================================================
std::vector<std::shared_ptr<graph_lib::Node<std::string>>> planHeist(
    const graph_lib::UndirectedGraph<std::string>& mansion,
    std::shared_ptr<graph_lib::Node<std::string>> entry,
    std::shared_ptr<graph_lib::Node<std::string>> vault,
    std::shared_ptr<graph_lib::Node<std::string>> escape)
{
    // TODO: Implement your logic here.
    // Return the shortest path: Entry -> Vault -> Escape.
    // If impossible, return an empty vector.

    return {};
}

// ========================================================================
// EXERCISE 3: Network Triage (Prioritization)
// ========================================================================
std::vector<std::vector<std::shared_ptr<graph_lib::Node<std::string>>>> getRepairPriority(
    const graph_lib::UndirectedGraph<std::string>& network)
{
    // TODO: Implement your logic here.
    // Return a list of isolated network segments, sorted by size (descending).

    return {};
}

// ========================================================================
// EXERCISE 4: Parallel Build Orchestrator
// ========================================================================
std::vector<std::vector<std::shared_ptr<graph_lib::Node<std::string>>>> getParallelBuildSchedule(
    const graph_lib::DirectedGraph<std::string>& project_modules)
{
    // TODO: Implement your logic here.
    // Return a list of stages. Each stage must be sorted alphabetically.
    // Use try-catch to handle cycles. If a cycle is caught, return an empty vector.

    return {};
}

// ========================================================================
// I/O BOILERPLATE (DO NOT MODIFY BELOW THIS LINE)
// This section communicates with the Automated Grader.
// ========================================================================
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: No exercise specified." << std::endl;
        return 1;
    }

    std::string exercise = argv[1];
    std::ofstream sol("solution.txt");

    if (exercise == "ex1")
    {
        auto roster = graph_lib::UndirectedGraph<std::string>::loadFromFile("test_graph.txt");
        std::unordered_set<std::shared_ptr<graph_lib::Node<std::string>>> faces, heels;

        bool possible = classifyWrestlers(roster, faces, heels);
        sol << (possible ? "1" : "0") << "\n";
        if (possible)
        {
            for (auto& n : faces) sol << n->getValue() << " ";
            sol << "\n";
            for (auto& n : heels) sol << n->getValue() << " ";
            sol << "\n";
        }
    }
    else if (exercise == "ex2")
    {
        auto mansion = graph_lib::UndirectedGraph<std::string>::loadFromFile("test_graph.txt");

        std::ifstream params("params.txt");
        std::string e_val, v_val, esc_val;
        params >> e_val >> v_val >> esc_val;

        auto entry = findNode(mansion, e_val);
        auto vault = findNode(mansion, v_val);
        auto escape = findNode(mansion, esc_val);

        auto route = planHeist(mansion, entry, vault, escape);
        for (auto& n : route) sol << n->getValue() << " ";
        sol << "\n";
    }
    else if (exercise == "ex3")
    {
        auto network = graph_lib::UndirectedGraph<std::string>::loadFromFile("test_graph.txt");
        auto segments = getRepairPriority(network);

        for (auto& seg : segments)
        {
            for (auto& n : seg) sol << n->getValue() << " ";
            sol << "\n";
        }
    }
    else if (exercise == "ex4")
    {
        auto dag = graph_lib::DirectedGraph<std::string>::loadFromFile("test_graph.txt");
        auto stages = getParallelBuildSchedule(dag);

        if (stages.empty())
        {
            sol << "CYCLE\n";
        }
        else
        {
            for (auto& stage : stages)
            {
                for (auto& n : stage) sol << n->getValue() << " ";
                sol << "\n";
            }
        }
    }

    sol.close();
    return 0;
}