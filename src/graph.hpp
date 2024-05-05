#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <iterator>
#include <memory>
#include <iostream>

typedef std::size_t sizeType;

template <typename T, typename W>
struct Vertex
{
    // typedef typename std::list<std::unique_ptr<Vertex<T,W>>>::iterator Viterator;
    Vertex(const T& element): element{element} {}
    // Vertex(const Vertex<T,W>& other): element{other.element}, iterator{other.iterator} {}
    Vertex(const Vertex<T,W>& other): element{other.element}, i{other.i} {}
    Vertex<T,W>& operator=(const Vertex<T,W>& other) 
    { 
        element = other.element;
        // iterator = other.iterator;
        i = other.i;
        return *this;
    }
    T element;
    // Viterator iterator;
    sizeType i;

    friend std::ostream& operator<<(std::ostream& os, const Vertex<T, W>& obj)
    {
        os << obj.element;
        return os;
    }
};

template <typename T, typename W>
struct Edge
{
    // typedef typename std::list<std::unique_ptr<Edge<T,W>>>::iterator Eiterator;
    Edge(Vertex<T,W>* v, Vertex<T,W>* w, const W& element): element{element}, v{v}, w{w} {}
    // Edge(const Edge<T,W>& other): element{other.element}, v{other.v}, w{other.w}, iterator{other.iterator} {}
    Edge(const Edge<T,W>& other): element{other.element}, v{other.v}, w{other.w}, i{other.i} {}
    Edge<T,W>& operator=(const Edge<T,W>& other) 
    {
        element = other.element;
        v = other.v,
        w = other.w;
        // iterator = other.iterator;
        i = other.i;
        return *this;
    }
    W element;
    Vertex<T,W>* v;
    Vertex<T,W>* w;
    // Eiterator iterator;
    sizeType i;

    friend std::ostream& operator<<(std::ostream& os, const Edge<T, W>& obj)
    {
        os << obj.v->element << "--" << obj.element << "--" << obj.w->element;
        return os;
    }
};

template <typename T, typename W>
class GraphADT
{
    public:
        // Metody dostepu
        virtual const sizeType sizeV() const = 0;
        virtual const sizeType sizeE() const = 0;
        virtual std::array<Vertex<T,W>*,2> endVertices(const Edge<T,W>* e) const = 0;
        virtual Vertex<T,W>* opposite(const Vertex<T,W>* v, const Edge<T,W>* e) const = 0;
        virtual bool areAdjacent(const Vertex<T,W>* v, const Vertex<T,W>* w) const = 0;
        virtual void replace(Vertex<T,W>* const v, const T& x) = 0;
        virtual void replace(Edge<T,W>* const e, const W& x) = 0;
        
        // Metody uaktualniajace
        virtual Vertex<T,W>* insertVertex(const T& x) = 0;
        virtual Edge<T,W>* insertEdge(Vertex<T,W>* v, Vertex<T,W>* w, const W& x) = 0;
        virtual void removeVertex(Vertex<T,W>* const v) = 0;
        virtual void removeEdge(Edge<T,W>* const e) = 0;
        virtual void clear() = 0;

        // Metody iterujace
        virtual std::vector<Edge<T,W>*> incidentEdges(const Vertex<T,W>* const v) const = 0;
        virtual std::vector<Vertex<T,W>*> vertices() const = 0;
        virtual std::vector<Edge<T,W>*> edges() const = 0;
};

#endif