#ifndef DIJKSTRA_ALGORITHM_HPP
#define DIJKSTRA_ALGORITHM_HPP

#include <vector>
#include <queue>
#include <map>

#include "graph.hpp"
#include "data_manager.hpp"

class DijkstraAlgorithm
{
    private:
        // template <typename T>
        // class Greater
        // {
        //     public:
        //         bool operator()(const std::pair<Vertex<T,int>*,int>& l, const std::pair<Vertex<T,int>*,int>& r) const 
        //             { return l.second > r.second; }
        // };

    public:
        DijkstraAlgorithm() = delete;

        // template <typename T>
        // static std::map<Vertex<T,int>*,int> shortestPath(const GraphADT<T,int>* graph, const Vertex<T,int>* v)
        // {
        //     std::vector<Vertex<T,int>*> vertices = graph->vertices();

        //     static std::map<Vertex<T,int>*,int> D;
        //     for (auto & i : vertices)
        //     {
        //         if (i == v) D[i] = 0;
        //         else D[i] = INT_MAX;
        //     }

        //     std::priority_queue<std::pair<Vertex<T,int>*,int>, std::vector<std::pair<Vertex<T,int>*,int>>, Greater<T>> Q;
        //     for (auto & i : D)
        //         Q.push(std::pair<Vertex<T,int>*,int>(i.first,i.second));

        //     while (!Q.empty())
        //     {
        //         std::pair<Vertex<T,int>*,int> u = Q.top();
        //         Q.pop();

        //         for (auto & i : graph->incidentEdges(u.first))
        //         {
        //             auto z = graph->opposite(u.first, i);
        //             if (D[u.first] + i->element < D[z])
        //             {
        //                 D[z] = D[u.first] + i->element;
        //                 u.second = D[z];
        //                 Q.push(u);
        //             }
        //         }
        //     }

        //     return D;
        // }

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