#ifndef DIJKSTRA_ALGORITHM_HPP
#define DIJKSTRA_ALGORITHM_HPP

#include <vector>
#include <queue>
#include <map>

#include <chrono>

#include "graph.hpp"

#ifndef TIME_POINT
#define TIME_POINT std::chrono::high_resolution_clock::now()
#endif

/// @brief Klasa implementujaca algorytm Dijkstry
class DijkstraAlgorithm
{
    public:
        DijkstraAlgorithm() = delete;

        /// @brief Driver algorytmu Dijkstry. Prezentuje dzialanie algorytmu dla grafu z mala iloscia wierzcholkow.
        /// @tparam T typ elementow na wierzcholkach
        /// @param graph referencja do nieskierowanego grafu prostego z nieujemnymi wagami krawedzi
        /// @param v referencja do wierzcholka wejsciowego
        /// @return Zwraca std::vector<int> zawierajacy dlugosci sciezek do kazdego wektora w grafie.
        template <typename T>
        static std::vector<int> driver(const GraphADT<T,int>* graph, const Vertex<T,int>* v)
        {
            std::vector<int> D(graph->sizeV(), INT_MAX);
            D[v->i] = 0;

            if (graph->sizeV() > 10) return D;

            std::cout << "======= Najkrotsza sciezka od " << v->element << " =======\nPoczatek: { ";
            for (int i = 0; i < D.size(); i++)
                std::cout << static_cast<char>('a' + i) << "=" << D[i] << ' ';
            std::cout << '}' << std::endl;

            auto compare = [D](const Vertex<T,int>* v, const Vertex<T,int>* u) { return D[v->i] > D[u->i]; };
            
            std::priority_queue<const Vertex<T,int>*, std::vector<const Vertex<T,int>*>, decltype(compare)> Q(compare);
            for (auto & i : graph->vertices())
                Q.push(i);

            int step = 1;

            while (!Q.empty())
            {
                const Vertex<T,int>* u = Q.top();
                Q.pop();

                for (auto & i : graph->incidentEdges(u))
                {
                    auto z = graph->opposite(u, i);
                    if (D[u->i] + i->element < D[z->i])
                    {
                        D[z->i] = D[u->i] + i->element;
                        Q.push(u);
                        std::cout << "       " << step++ <<": { ";
                        for (int i = 0; i < D.size(); i++)
                            std::cout << static_cast<char>('a' + i) << "=" << D[i] << ' ';
                        std::cout << '}' << std::endl;
                    }
                }
            }

            return D;
        }

        /// @brief Algorytm Dijkstry
        /// @tparam T typ elementow na wierzcholkach
        /// @param graph referencja do nieskierowanego grafu prostego z nieujemnymi wagami krawedzi
        /// @param v referencja do wierzcholka wejsciowego
        /// @return Zwraca std::vector<int> zawierajacy dlugosci sciezek do kazdego wektora w grafie.
        template <typename T>
        static std::vector<int> shortestPath(const GraphADT<T,int>* graph, const Vertex<T,int>* v)
        {
            std::vector<int> D(graph->sizeV(), INT_MAX);
            D[v->i] = 0;

            auto compare = [D](const Vertex<T,int>* v, const Vertex<T,int>* u) { return D[v->i] > D[u->i]; };
            
            std::priority_queue<const Vertex<T,int>*, std::vector<const Vertex<T,int>*>, decltype(compare)> Q(compare);
            for (auto & i : graph->vertices())
                Q.push(i);

            while (!Q.empty())
            {
                const Vertex<T,int>* u = Q.top();
                Q.pop();

                for (auto & i : graph->incidentEdges(u))
                {
                    auto z = graph->opposite(u, i);
                    if (D[u->i] + i->element < D[z->i])
                    {
                        D[z->i] = D[u->i] + i->element;
                        Q.push(u);
                    }
                }
            }

            return D;
        }
};

#endif