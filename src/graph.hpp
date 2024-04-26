#include <vector>

typedef int sizeType;

class Vertex
{
    public:
        virtual std::vector<Edge> incidentEdges() const = 0;
        virtual bool isAdjacentTo(const Vertex* const vertex) const = 0;
};

class Edge
{
    public:
        virtual std::vector<Vertex> endVertices() const = 0;
        virtual Vertex& opposite(const Vertex* const vertex) const = 0;
        virtual bool isAdjacentTo(const Edge* const edge) const = 0;
        virtual bool isAdjacentOn(const Vertex* const vertex) const = 0;
};

template <typename T>
class GraphADT
{
    public:
        virtual const std::vector<Vertex>& vertices() const = 0;
        virtual const std::vector<Edge>& edges() const = 0;
        virtual const Vertex& insertVertex(const T& element) = 0;
        virtual const Edge& insertEdge(const Vertex& v, const Vertex& w, const T& element) = 0;
        virtual void eraseVertex(Vertex* const vertex) = 0;
        virtual void eraseEdge(Edge* const edge) = 0;
};