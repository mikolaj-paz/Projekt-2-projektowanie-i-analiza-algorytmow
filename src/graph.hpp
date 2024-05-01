#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

typedef std::size_t sizeType;

template <typename T>
class Vertex;

template <typename T>
class Edge;

template <typename T>
class GraphADT
{
    public:
        // Metody dostepu
        virtual Vertex<T>* endVertices(const Edge<T>& e) const = 0;
        virtual Vertex<T>& opposite(const Vertex<T>& v, const Edge<T>& e) const = 0;
        virtual bool areAdjacent(const Vertex<T>& v, const Vertex<T>& w) const = 0;
        virtual void replace(const Vertex<T>& v, const T& x) = 0;
        virtual void replace(const Edge<T>& e, const T& x) = 0;
        
        // Metody uaktualniajace
        virtual void insertVertex(const T& x) = 0;
        virtual void insertEdge(const Vertex<T>& v, const Vertex<T>& w, const T& x) = 0;
        virtual void removeVertex(const Vertex<T>& v) = 0;
        virtual void removeEdge(const Edge<T>& e) = 0;

        // Metody iterujace
        virtual void incidentEdges(const Vertex<T>& v) const = 0;
        virtual const std::vector<Vertex<T>>& vertices() const = 0;
        virtual const std::vector<Edge<T>>& edges() const = 0;
};

#endif