#include <iostream>
#include <chrono>

#include "data_manager.hpp"
#include "adjacency_list_graph.hpp"
#include "adjacency_matrix_graph.hpp"
#include "dijkstra_algorithm.hpp"

int main()
{
    const int N[5] { 10, 50, 100, 500, 1000 };
    const double D[4] { .25, .5, .75, 1 };

    AdjacencyListGraph<int,int> ALgraph;
    AdjacencyMatrixGraph<int,int> AMgraph;
    
    auto ALtab = DataManager::createTimesTable(&ALgraph, N, 5, D, 4);
    DataManager::saveToCSV("../AL_data.csv", ALtab, 5, 6, ';');

    auto AMtab = DataManager::createTimesTable(&AMgraph, N, 5, D, 4);
    DataManager::saveToCSV("../AM_data.csv", AMtab, 5, 6, ';');

    return 0;
}