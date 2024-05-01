#include <iostream>

#include "adjacency_list_graph.hpp"
#include "adjacency_matrix_graph.hpp"

template <typename C>
void printContainer(const C& container)
{
    for (auto & i : container)
        std::cout << i << ' ';
    std::cout << std::endl;
}

int main()
{
    AdjacencyMatrixGraph<int> graph;

    auto v = graph.insertVertex(3);
    auto w = graph.insertVertex(4);

    graph.insertEdge(*v, *w, 5);

    auto vertices = graph.vertices();
    printContainer(vertices);

    auto edges = graph.edges();
    printContainer(edges);

    auto endVert = graph.endVertices(edges[0]);
    printContainer(endVert);

    auto incidEdg = graph.incidentEdges(vertices[0]);
    printContainer(incidEdg);

    std::cout << graph.areAdjacent(*v, *w) << std::endl;
    std::cout << graph.opposite(*w, edges[0]) << std::endl;

    graph.removeVertex(*v);

    std::cout << graph.vertices().size() << ' ' << graph.edges().size() << std::endl;

    return 0;
}