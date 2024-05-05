#ifndef ADJACENCY_LIST_GRAPH_HPP
#define ADJACENCY_LIST_GRAPH_HPP

#include <vector>
#include <list>
#include <array>
#include <iterator>
#include <memory>

#include "graph.hpp"

#ifndef _alg_vertex_ptr
#define _alg_vertex_ptr static_cast<AdjacencyListVertex<T,W>*>
#endif

#ifndef _alg_cvertex_ptr
#define _alg_cvertex_ptr static_cast<const AdjacencyListVertex<T,W>*>
#endif

#ifndef _alg_vertex_cptr
#define _alg_vertex_cptr static_cast<AdjacencyListVertex<T,W>* const>
#endif

#ifndef _alg_cvertex_cptr
#define _alg_cvertex_cptr static_cast<const AdjacencyListVertex<T,W>* const>
#endif

#ifndef _alg_edge_cptr
#define _alg_edge_cptr static_cast<AdjacencyListEdge<T,W>* const>
#endif

template <typename T, typename W>
struct AdjacencyListEdge;

template <typename T, typename W>
struct AdjacencyListVertex : public Vertex<T,W>
{
    AdjacencyListVertex(const T& element):
        Vertex<T,W>(element) {}

    AdjacencyListVertex(const AdjacencyListVertex<T,W>& other):
        Vertex<T,W>(other), I{other.I} {}

    AdjacencyListVertex<T,W>& operator=(const AdjacencyListVertex<T,W>& other)
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
    public:
        void clear()
        {
            V.clear();
            E.clear();
        }

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
            const AdjacencyListVertex<T,W>* vAL = _alg_cvertex_ptr(v);
            const AdjacencyListVertex<T,W>* wAL = _alg_cvertex_ptr(w);
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
        
        void replace(Edge<T,W>* const e, const W& x)
            { e->element = x; }

        Vertex<T,W>* insertVertex(const T& x)
        {
            V.push_back(std::unique_ptr<Vertex<T,W>>(new AdjacencyListVertex<T,W>(x)));
            // typename VlistType::iterator last = --(V.end());
            // V.back().get()->iterator = last;
            sizeType last = V.size() - 1;
            V.back().get()->i = last;
            // return (*last).get();
            return V[last].get();
        }

        Edge<T,W>* insertEdge(Vertex<T,W>* v, Vertex<T,W>* w, const W& x)
        {
            E.push_back(std::unique_ptr<Edge<T,W>>(new AdjacencyListEdge<T,W>(v, w, x)));
            // typename ElistType::iterator last = --(E.end());
            sizeType last = E.size() - 1;

            AdjacencyListEdge<T,W>* edge = static_cast<AdjacencyListEdge<T,W>*>(E.back().get());
            // edge->iterator = last;
            edge->i = last;

            _alg_vertex_ptr(edge->v)->I.push_back(edge);
            edge->iteratorIv = --(_alg_vertex_ptr(edge->v)->I.end());

            _alg_vertex_ptr(edge->w)->I.push_back(edge);
            edge->iteratorIw = --(_alg_vertex_ptr(edge->w)->I.end());

            // return (*last).get();
            return E[last].get();
        }

        void removeVertex(Vertex<T,W>* const v)
        {
            // Usuwanie krawędzi incydentnych
            for (auto & i : _alg_cvertex_cptr(v)->I)
                removeEdge(i);

            // Porzadkowanie indeksow
            sizeType* newIndex = &v->i;
            for (sizeType i = v->i; i < V.size(); i++)
                std::swap(*newIndex, V[i].get()->i);

            // Usuwanie wpisu wierchołka z listy wierzchołków
            // V.erase(v->iterator);
            V.erase(V.begin() + v->i);
        }

        void removeEdge(Edge<T,W>* const e)
        {
            _alg_vertex_ptr(e->v)->I.erase(_alg_edge_cptr(e)->iteratorIv);
            _alg_vertex_ptr(e->w)->I.erase(_alg_edge_cptr(e)->iteratorIw);

            // Porzadkowanie indeksow
            sizeType* newIndex = &e->i;
            for (sizeType i = e->i; i < V.size(); i++)
                std::swap(*newIndex, E[i].get()->i);

            // E.erase(e->iterator);
            E.erase(E.begin() + e->i);
        }

        std::vector<Edge<T,W>*> incidentEdges(const Vertex<T,W>* const v) const
        {
            std::vector<Edge<T,W>*> vect;
            for (auto & i : _alg_cvertex_cptr(v)->I)
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

    private:
        // typedef std::list<std::unique_ptr<Vertex<T,W>>> VlistType;
        // typedef std::list<std::unique_ptr<Edge<T,W>>> ElistType;
        typedef std::vector<std::unique_ptr<Vertex<T,W>>> VlistType;
        typedef std::vector<std::unique_ptr<Edge<T,W>>> ElistType;

        VlistType V;
        ElistType E;
};

#endif