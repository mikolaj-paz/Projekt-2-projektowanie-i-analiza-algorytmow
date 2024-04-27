#include <vector>
#include <list>
#include <array>
#include <iterator>
#include <iostream>

#include "graph.hpp"

template <typename T>
class AdjacencyListGraph;

template <typename T>
class AdjacencyListEdge;

template <typename T>
class AdjacencyListVertex
{
    public:
        AdjacencyListVertex(const T& element):
            element{element} {}

        AdjacencyListVertex(const AdjacencyListVertex<T>& other):
            element{other.element}, iter{other.iter}, I{other.I} {}

        AdjacencyListVertex<T>& operator=(const AdjacencyListVertex<T>& other)
        {
            if (this == &other)
                return *this;
            element = other.element;
            iter = other.iter;
            I = other.I;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const AdjacencyListVertex<T>& obj)
        {
            os << obj.element;
            return os;
        }

    private:
        T element;
        typename std::list<AdjacencyListVertex<T>>::iterator iter;

        std::list<AdjacencyListEdge<T>*> I;
        
        friend class AdjacencyListEdge<T>;
        friend class AdjacencyListGraph<T>;
};

template <typename T>
class AdjacencyListEdge
{
    public:
        AdjacencyListEdge(AdjacencyListVertex<T>& v, AdjacencyListVertex<T>& w, const T& element):
            element{element}, v{&v}, w{&w}
        {
            // v.I.push_back(this);
            // w.I.push_back(this);
            // iteratorIv = --(v.I.end());
            // iteratorIw = --(w.I.end());
        }

        AdjacencyListEdge(const AdjacencyListEdge<T>& other):
            element{other.element}, v{other.v}, w{other.w},
            iter{other.iter}, iteratorIv{other.iteratorIv}, iteratorIw{other.iteratorIw} {}

        AdjacencyListEdge<T>& operator=(const AdjacencyListEdge<T>& other)
        {
            if (this == &other)
                return *this;
            element = other.element;
            v = other.v;
            w = other.w;
            iter = other.iter;
            iteratorIv = other.iteratorIv;
            iteratorIw = other.iteratorIw;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const AdjacencyListEdge<T>& obj)
        {
            os << obj.element;
            return os;
        }

    private:
        T element;
        AdjacencyListVertex<T>* v;
        AdjacencyListVertex<T>* w;
        
        typename std::list<AdjacencyListEdge<T>>::iterator iter;
        typename std::list<AdjacencyListEdge<T>*>::iterator iteratorIv;
        typename std::list<AdjacencyListEdge<T>*>::iterator iteratorIw;

        friend class AdjacencyListGraph<T>;
};

template <typename T>
class AdjacencyListGraph
{
    private:
        std::list<AdjacencyListVertex<T>> V;
        std::list<AdjacencyListEdge<T>> E;

    public:
        std::array<AdjacencyListVertex<T>,2> endVertices(const AdjacencyListEdge<T>& e)
        {
            std::array<AdjacencyListVertex<T>,2> arr = { *e.v, *e.w };
            return arr;
        }

        AdjacencyListVertex<T>& opposite(const AdjacencyListVertex<T>& v, const AdjacencyListEdge<T>& e)
            { return e.v == &v ? *e.w : *e.v; }

        bool areAdjacent(const AdjacencyListVertex<T>& v, const AdjacencyListVertex<T>& w)
        {
            if (v.I.size() < w.I.size())
            {
                for (auto & i : v.I)
                    if ((*i).v == &w || (*i).w == &w )
                        return true;
            }
            else
            {
                for (auto & i : w.I)
                    if ((*i).v == &v || (*i).w == &v )
                        return true;
            }
            return false;
        }

        void replace(const AdjacencyListVertex<T>& v, const T& x)
            { v.element = x; }
        
        void replace(const AdjacencyListEdge<T>& e, const T& x)
            { e.element = x; }

        typename std::list<AdjacencyListVertex<T>>::iterator insertVertex(const T& x)
        {
            V.push_back(AdjacencyListVertex<T>(x));
            const auto last = --(V.end());
            V.back().iter = last;
            return last;
        }

        typename std::list<AdjacencyListEdge<T>>::iterator insertEdge(AdjacencyListVertex<T>& v, AdjacencyListVertex<T>& w, const T& x)
        {
            E.push_back(AdjacencyListEdge<T>(v, w, x));
            const auto last = --(E.end());

            // potrzeba poprawy wizualnej (czytelnosci)
            E.back().iter = last;
            E.back().v->I.push_back(&(E.back()));
            E.back().w->I.push_back(&(E.back()));
            E.back().iteratorIv = --(E.back().v->I.end());
            E.back().iteratorIw = --(E.back().w->I.end());
            return last;
        }

        void removeVertex(const AdjacencyListVertex<T>& v)
        {
            for (auto & i : v.I)
                E.erase(i->iter);
            V.erase(v.iter);            
        }

        void removeEdge(const AdjacencyListEdge<T>& e)
        {
            e.v->I.erase(e.iteratorIv);
            e.w->I.erase(e.iteratorIw);
            E.erase(e.iter);
        }

        const std::vector<AdjacencyListEdge<T>> incidentEdges(const AdjacencyListVertex<T>& v) const
        {
            std::vector<AdjacencyListEdge<T>> vect;
            for (auto & iter : v.I)
                vect.push_back(*iter);
            return vect;
        }
        
        const std::vector<AdjacencyListVertex<T>> vertices() const
        {
            std::vector<AdjacencyListVertex<T>> vect;
            for (auto & iter : V)
                vect.push_back(iter);
            return vect;
        }

        const std::vector<AdjacencyListEdge<T>> edges() const
        {
            std::vector<AdjacencyListEdge<T>> vect;
            for (auto & iter : E)
                vect.push_back(iter);
            return vect;
        }
};