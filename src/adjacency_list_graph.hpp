#ifndef ADJACENCY_LIST_GRAPH_HPP
#define ADJACENCY_LIST_GRAPH_HPP

#include <vector>
#include <list>
#include <array>
#include <iterator>
#include <memory>

#include "graph.hpp"

template <typename T, typename W>
struct AdjacencyListEdge;

template <typename T, typename W>
struct AdjacencyListVertex : public Vertex<T,W>
{
    AdjacencyListVertex(const T& element):
        Vertex<T,W>(element) {}

    AdjacencyListVertex(const AdjacencyListVertex<T,W>& other):
        Vertex<T,W>(other), I{other.I} {}

    AdjacencyListVertex<T, W>& operator=(const AdjacencyListVertex<T, W>& other)
    {
        if (this == &other)
            return *this;
        Vertex<T,W>::operator=(other);
        I = other.I;
        return *this;
    }

    std::list<AdjacencyListEdge<T,W>*> I;
};

template <typename T, typename W>
struct AdjacencyListEdge : Edge<T,W>
{
    AdjacencyListEdge<T,W>(Vertex<T,W>* v, Vertex<T,W>* w, const W& element):
        Edge<T,W>(v, w, element) {}

    AdjacencyListEdge(const AdjacencyListEdge<T,W>& other):
        Edge<T,W>(other), iteratorIv{other.iteratorIv}, iteratorIw{other.iteratorIw} {}

    AdjacencyListEdge<T, W>& operator=(const AdjacencyListEdge<T, W>& other)
    {
        if (this == &other)
            return *this;
        Edge<T,W>::operator=(other);
        iteratorIv = other.iteratorIv;
        iteratorIw = other.iteratorIw;
        return *this;
    }

    typename std::list<AdjacencyListEdge<T, W>*>::iterator iteratorIv;
    typename std::list<AdjacencyListEdge<T, W>*>::iterator iteratorIw;
};

template <typename T, typename W>
class AdjacencyListGraph : public GraphADT<T,W>
{
    private:
        typedef std::list<std::unique_ptr<Vertex<T,W>>> VlistType;
        typedef std::list<std::unique_ptr<Edge<T,W>>> ElistType;

        VlistType V;
        ElistType E;

    public:
        const sizeType sizeV() const
            { return V.size(); }

        const sizeType sizeE() const
            { return E.size(); }

        std::array<Vertex<T,W>*,2> endVertices(const Edge<T,W>* e) const
            { return std::array<Vertex<T,W>*,2>{ e->v, e->w }; }

        Vertex<T,W>* opposite(const Vertex<T,W>* v, const Edge<T,W>* e) const
            { return e->v == v ? e->w : e->v; }

        bool areAdjacent(const Vertex<T,W>* v, const Vertex<T,W>* w) const
        {
            const AdjacencyListVertex<T,W>* vAL = static_cast<const AdjacencyListVertex<T,W>*>(v);
            const AdjacencyListVertex<T,W>* wAL = static_cast<const AdjacencyListVertex<T,W>*>(w);
            if (vAL->I.size() < wAL->I.size())
            {
                for (auto & i : vAL->I)
                    if ((*i).v == w || (*i).w == w )
                        return true;
            }
            else
            {
                for (auto & i : wAL->I)
                    if ((*i).v == v || (*i).w == v )
                        return true;
            }
            return false;
        }

        void replace(Vertex<T,W>* const v, const T& x)
            { v->element = x; }
        
        void replace(Edge<T,W>* const e, const T& x)
            { e->element = x; }

        Vertex<T,W>* insertVertex(const T& x)
        {
            V.push_back(std::unique_ptr<Vertex<T,W>>(new AdjacencyListVertex<T,W>(x)));
            typename VlistType::iterator last = --(V.end());
            V.back().get()->iterator = last;
            return (*last).get();
        }

        Edge<T,W>* insertEdge(Vertex<T,W>* v, Vertex<T,W>* w, const W& x)
        {
            E.push_back(std::unique_ptr<Edge<T,W>>(new AdjacencyListEdge<T,W>(v, w, x)));
            typename ElistType::iterator last = --(E.end());

            AdjacencyListEdge<T,W>* edge = static_cast<AdjacencyListEdge<T,W>*>(E.back().get());
            edge->iterator = last;

            static_cast<AdjacencyListVertex<T,W>*>(edge->v)->I.push_back(edge);
            edge->iteratorIv = --(static_cast<AdjacencyListVertex<T,W>*>(edge->v)->I.end());

            static_cast<AdjacencyListVertex<T,W>*>(edge->w)->I.push_back(edge);
            edge->iteratorIw = --(static_cast<AdjacencyListVertex<T,W>*>(edge->w)->I.end());

            return (*last).get();
        }

        void removeVertex(Vertex<T,W>* const v)
        {
            for (auto & i : static_cast<const AdjacencyListVertex<T,W>* const>(v)->I)
                removeEdge(i);
            V.erase(v->iterator);
        }

        void removeEdge(Edge<T,W>* const e)
        {
            static_cast<AdjacencyListVertex<T,W>*>(e->v)->I.erase(static_cast<AdjacencyListEdge<T,W>* const>(e)->iteratorIv);
            static_cast<AdjacencyListVertex<T,W>*>(e->w)->I.erase(static_cast<AdjacencyListEdge<T,W>* const>(e)->iteratorIw);
            E.erase(e->iterator);
        }

        std::vector<Edge<T,W>*> incidentEdges(const Vertex<T,W>* const v) const
        {
            std::vector<Edge<T,W>*> vect;
            for (auto & i : static_cast<const AdjacencyListVertex<T,W>* const>(v)->I)
                vect.push_back(i);
            return vect;
        }
        
        std::vector<Vertex<T,W>*> vertices() const
        {
            std::vector<Vertex<T,W>*> vect;
            for (auto & i : V)
                vect.push_back(i.get());
            return vect;
        }

        std::vector<Edge<T,W>*> edges() const
        {
            std::vector<Edge<T,W>*> vect;
            for (auto & i : E)
                vect.push_back(i.get());
            return vect;
        }
};

#endif