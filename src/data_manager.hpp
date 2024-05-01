#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <vector>
#include <chrono>
#include <random>
#include <limits.h>

#include "graph.hpp"

class DataManager
{
    public:
        DataManager() = delete;

        template <typename C>
        static void printContainer(const C& container)
        {
            std::cout << "{ ";
            for (auto & i : container)
                std::cout << i << ' ';
            std::cout << '}' << std::endl;
        }

        template <typename C>
        static void printPointerContainer(const C& container)
        {
            std::cout << "{ ";
            for (auto & i : container)
                std::cout << *i << ' ';
            std::cout << '}' << std::endl;
        }

        static void createRandomSimpleUndirectedGraph(GraphADT<int,int>* graph, const int& verticesNumber, const double& density)
        {
            std::vector<Vertex<int,int>*> vertices;

            for (int i = 1; i <= verticesNumber; i++)
                vertices.push_back(graph->insertVertex(i));

            const int targetEdgesNumber = density * verticesNumber * (verticesNumber - 1) / 2;

            for (int i = 0; i < targetEdgesNumber; i++)
            {
                int v;
                do
                    v = getRandomInt(0, verticesNumber - 1);
                while (graph->incidentEdges(vertices[v]).size() == verticesNumber - 1);
                int w;
                do
                    w = getRandomInt(0, verticesNumber - 1);
                while (w == v || graph->areAdjacent(vertices[v], vertices[w]));
                graph->insertEdge(vertices[v], vertices[w], getRandomInt(0, INT_MAX - 1));
            }
        }

        static int getRandomInt(const int& min, const int& max)
        {
            auto seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 generator(seed);
            std::uniform_int_distribution<int> distribution(min, max);
            return distribution(generator);
        }
};

#endif