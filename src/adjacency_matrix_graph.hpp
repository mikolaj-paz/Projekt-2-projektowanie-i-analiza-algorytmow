#ifndef ADJACENCY_MATRIX_GRAPH_HPP
#define ADJACENCY_MATRIX_GRAPH_HPP

#include <vector>
#include <list>
#include <array>
#include <iterator>
#include <iostream>

#include "graph.hpp"

template <typename T>
class AdjacencyMatrixGraph;

template <typename T>
class AdjacencyMatrixEdge;

template <typename T>
class AdjacencyMatrixVertex
{
    public:
        AdjacencyMatrixVertex(const T& element):
            element{element} {}

        AdjacencyMatrixVertex(const AdjacencyMatrixVertex<T>& other):
            element{other.element}, iter{other.iter}, Aindex{other.Aindex} {}

        AdjacencyMatrixVertex<T>& operator=(const AdjacencyMatrixVertex<T>& other)
        {
            if (this == &other)
                return *this;
            element = other.element;
            iter = other.iter;
            Aindex = other.Aindex;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const AdjacencyMatrixVertex<T>& obj)
        {
            os << obj.element;
            return os;
        }

    private:
        T element;
        typename std::list<AdjacencyMatrixVertex<T>>::iterator iter;
        
        int Aindex;
        
        friend class AdjacencyMatrixEdge<T>;
        friend class AdjacencyMatrixGraph<T>;
};

template <typename T>
class AdjacencyMatrixEdge
{
    public:
        AdjacencyMatrixEdge(AdjacencyMatrixVertex<T>& v, AdjacencyMatrixVertex<T>& w, const T& element):
            element{element}, v{&v}, w{&w} {}

        AdjacencyMatrixEdge(const AdjacencyMatrixEdge<T>& other):
            element{other.element}, v{other.v}, w{other.w}, iter{other.iter} {}

        AdjacencyMatrixEdge<T>& operator=(const AdjacencyMatrixEdge<T>& other)
        {
            if (this == &other)
                return *this;
            element = other.element;
            v = other.v;
            w = other.w;
            iter = other.iter;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const AdjacencyMatrixEdge<T>& obj)
        {
            os << obj.element;
            return os;
        }

    private:
        T element;
        AdjacencyMatrixVertex<T>* v;
        AdjacencyMatrixVertex<T>* w;
        
        typename std::list<AdjacencyMatrixEdge<T>>::iterator iter;

        friend class AdjacencyMatrixGraph<T>;
};

template <typename T>
class AdjacencyMatrixGraph
{
    private:
        std::list<AdjacencyMatrixVertex<T>> V;
        std::list<AdjacencyMatrixEdge<T>> E;
        std::vector<std::vector<AdjacencyMatrixEdge<T>*>> A;

    public:
        std::array<AdjacencyMatrixVertex<T>,2> endVertices(const AdjacencyMatrixEdge<T>& e)
        {
            std::array<AdjacencyMatrixVertex<T>,2> arr = { *e.v, *e.w };
            return arr;
        }

        AdjacencyMatrixVertex<T>& opposite(const AdjacencyMatrixVertex<T>& v, const AdjacencyMatrixEdge<T>& e)
            { return e.v == &v ? *e.w : *e.v; }

        bool areAdjacent(const AdjacencyMatrixVertex<T>& v, const AdjacencyMatrixVertex<T>& w)
        {
            if (A[v.Aindex][w.Aindex]) return true;
            return false;
        }

        void replace(const AdjacencyMatrixVertex<T>& v, const T& x)
            { v.element = x; }
        
        void replace(const AdjacencyMatrixEdge<T>& e, const T& x)
            { e.element = x; }

        typename std::list<AdjacencyMatrixVertex<T>>::iterator insertVertex(const T& x)
        {
            V.push_back(AdjacencyMatrixVertex<T>(x));
            const auto last = --(V.end());
            V.back().iter = last;
            // std::cout <<  - 1 << std::endl;
            for (int i = 0; i < A.size(); i++)
                A[i].push_back(nullptr);
            V.back().Aindex = A.size();
            A.push_back(std::vector<AdjacencyMatrixEdge<T>*>(A.size() + 1, nullptr));
            return last;
        }

        typename std::list<AdjacencyMatrixEdge<T>>::iterator insertEdge(AdjacencyMatrixVertex<T>& v, AdjacencyMatrixVertex<T>& w, const T& x)
        {
            E.push_back(AdjacencyMatrixEdge<T>(v, w, x));
            const auto last = --(E.end());
            E.back().iter = last;
            A[v.Aindex][w.Aindex] = &(E.back());
            A[w.Aindex][v.Aindex] = &(E.back());
            return last;
        }

        void removeVertex(AdjacencyMatrixVertex<T>& v)
        {
            // Porzadkowanie indeksow
            int newIndex = v.Aindex;
            for (auto & i = ++(v.iter); i != V.end(); i++)
                std::swap(newIndex, (*i).Aindex);

            // Usuwanie wpisow krawedzi incydentnych z listy krawedzi
            for (auto & iter : A[v.Aindex])
                if (iter) E.erase((*iter).iter);

            // Czyszczenie macierzy
            A.erase(A.begin() + v.Aindex);
            for (auto & i : A)
                i.erase(i.begin() + v.Aindex); 

            // Usuwanie wpisu wierzcholka z listy wierzcholkow
            V.erase(--(v.iter));
        }

        void removeEdge(const AdjacencyMatrixEdge<T>& e)
        {
            A[e.v->Aindex][e.w->Aindex] = nullptr;
            A[e.w->Aindex][e.v->Aindex] = nullptr;
            E.erase(e.iter);
        }

        const std::vector<AdjacencyMatrixEdge<T>> incidentEdges(const AdjacencyMatrixVertex<T>& v) const
        {
            std::vector<AdjacencyMatrixEdge<T>> vect;
            for (auto & iter : A[v.Aindex])
                if (iter) vect.push_back(*iter);
            return vect;
        }
        
        const std::vector<AdjacencyMatrixVertex<T>> vertices() const
        {
            std::vector<AdjacencyMatrixVertex<T>> vect;
            for (auto & iter : V)
                vect.push_back(iter);
            return vect;
        }

        const std::vector<AdjacencyMatrixEdge<T>> edges() const
        {
            std::vector<AdjacencyMatrixEdge<T>> vect;
            for (auto & iter : E)
                vect.push_back(iter);
            return vect;
        }
};

#endif