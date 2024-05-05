#ifndef ADJACENCY_MATRIX_GRAPH_HPP
#define ADJACENCY_MATRIX_GRAPH_HPP

#include <vector>
#include <list>
#include <array>
#include <iterator>
#include <memory>

#include "graph.hpp"

#ifndef _amg_vertex_ptr
#define _amg_vertex_ptr static_cast<AdjacencyMatrixVertex<T,W>*>
#endif

#ifndef _amg_cvertex_ptr
#define _amg_cvertex_ptr static_cast<const AdjacencyMatrixVertex<T,W>*>
#endif

#ifndef _amg_vertex_cptr
#define _amg_vertex_cptr static_cast<AdjacencyMatrixVertex<T,W>* const>
#endif

#ifndef _amg_cvertex_cptr
#define _amg_cvertex_cptr static_cast<const AdjacencyMatrixVertex<T,W>* const>
#endif

template <typename T, typename W>
struct AdjacencyMatrixEdge;

template <typename T, typename W>
struct AdjacencyMatrixVertex : public Vertex<T,W>
{
    public:
        AdjacencyMatrixVertex(const T& element):
            Vertex<T,W>(element) {}

        AdjacencyMatrixVertex(const AdjacencyMatrixVertex<T,W>& other):
            Vertex<T,W>(other), Aindex{other.Aindex} {}

        AdjacencyMatrixVertex<T,W>& operator=(const AdjacencyMatrixVertex<T,W>& other)
        {
            if (this == &other)
                return *this;
            Vertex<T,W>::operator=(other);
            Aindex = other.Aindex;
            return *this;
        }
        
        sizeType Aindex;
};

template <typename T, typename W>
struct AdjacencyMatrixEdge : public Edge<T,W>
{
    public:
        AdjacencyMatrixEdge(Vertex<T,W>* v, Vertex<T,W>* w, const W& element):
            Edge<T,W>(v, w, element) {}

        AdjacencyMatrixEdge(const AdjacencyMatrixEdge<T,W>& other):
            Edge<T,W>(other) {}

        AdjacencyMatrixEdge<T,W>& operator=(const AdjacencyMatrixEdge<T,W>& other)
        {
            if (this == &other)
                return *this;
            Edge<T,W>::operator=(other);
            return *this;
        }
};

template <typename T, typename W>
class AdjacencyMatrixGraph : public GraphADT<T,W>
{
    public:
        void clear()
        {
            V.clear();
            E.clear();
            A.clear();
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
            if (A[_amg_cvertex_ptr(v)->Aindex][_amg_cvertex_ptr(w)->Aindex]) 
                return true;
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
            for (int i = 0; i < A.size(); i++)
                A[i].push_back(nullptr);
            _amg_vertex_ptr(V.back().get())->Aindex = A.size();
            A.push_back(std::vector<Edge<T,W>*>(A.size() + 1, nullptr));
            // return (*last).get();
            return V[last].get();
        }

        Edge<T,W>* insertEdge(Vertex<T,W>* v, Vertex<T,W>* w, const W& x)
        {
            E.push_back(std::unique_ptr<Edge<T,W>>(new AdjacencyListEdge<T,W>(v, w, x)));
            // typename ElistType::iterator last = --(E.end());
            // E.back().get()->iterator = last;
            sizeType last = E.size() - 1;
            E.back().get()->i = last;
            A[_amg_vertex_ptr(v)->Aindex][_amg_vertex_ptr(w)->Aindex] = E.back().get();
            A[_amg_vertex_ptr(w)->Aindex][_amg_vertex_ptr(v)->Aindex] = E.back().get();
            //  return (*last).get();
             return E[last].get();
        }

        void removeVertex(Vertex<T,W>* const v)
        {
            AdjacencyMatrixVertex<T,W>* const vAM = _amg_vertex_cptr(v);

            // Porzadkowanie indeksow
            sizeType* newIndex = &vAM->Aindex;
            // for (auto & i = ++(v->iterator); i != V.end(); i++)
            //     std::swap(*newIndex, _amg_vertex_ptr(i->get())->Aindex);
            for (sizeType i = v->i; i < V.size(); i++)
            {
                std::swap(*newIndex, _amg_vertex_ptr(V[i].get())->Aindex);
                std::swap(*newIndex, V[i].get()->i);
            }

            // Usuwanie wpisow krawedzi incydentnych z listy krawedzi
            for (auto & i : A[vAM->Aindex])
                // if (i) E.erase(i->iterator);
                if (i) E.erase(E.begin() + i->i);

            // Czyszczenie macierzy
            A.erase(A.begin() + vAM->Aindex);
            for (auto & i : A)
                i.erase(i.begin() + vAM->Aindex);

            // Usuwanie wpisu wierzcholka z listy wierzcholkow
            // V.erase(--(v->iterator));
            V.erase(V.begin() + v->i);
        }

        void removeEdge(Edge<T,W>* const e)
        {
            A[_amg_vertex_cptr(e->v)->Aindex][_amg_vertex_cptr(e->w)->Aindex] = nullptr;
            A[_amg_vertex_cptr(e->w)->Aindex][_amg_vertex_cptr(e->v)->Aindex] = nullptr;

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
            for (auto & i : A[_amg_cvertex_cptr(v)->Aindex])
                if (i) vect.push_back(i);
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
        std::vector<std::vector<Edge<T,W>*>> A;
};

#endif