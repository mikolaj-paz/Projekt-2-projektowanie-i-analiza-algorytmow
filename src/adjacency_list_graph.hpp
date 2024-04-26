#include <vector>

#include "graph.hpp"

template <typename T>
class AdjacencyListGraph : public GraphADT
{
    private:
        class AdjacencyListVertex : public Edge
        {
            public:
                std::vector<Edge> incidentEdges() const
                {
                    
                }
            private:
                T element;
                sizeType index;

                std::vector<Edge*> I;
        };

        class AdjacencyListEdge : public Edge
        {
            private:
                T element;
                AdjacencyListVertex* v;
                AdjacencyListVertex* w;
                sizeType index;

                std::vector<Edge*>* Iv;
                std::vector<Edge*>* Iw;
        };

        std::vector<AdjacencyListVertex> V;
        std::vector<AdjacencyListEdge> E;
};