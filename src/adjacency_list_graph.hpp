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

/// @brief Klasa wierzcholka grafu w reprezentacji listy sasiadow.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
struct AdjacencyListVertex : public Vertex<T,W>
{
    /// @brief Podstawowy konstruktor wierzcholka.
    /// @param element wartosc na wierzcholku
    AdjacencyListVertex(const T& element):
        Vertex<T,W>(element) {}

    /// @brief Konstruktor kopiujacy.
    /// @param other obiekt do skopiowania
    AdjacencyListVertex(const AdjacencyListVertex<T,W>& other):
        Vertex<T,W>(other), I{other.I} {}

    /// @brief Przeciazenie operatora kopiujacego-przypisywania
    /// @param other obiekt do skopiowania
    /// @return Zwraca skopiowany wierzcholek.
    AdjacencyListVertex<T,W>& operator=(const AdjacencyListVertex<T,W>& other)
    {
        if (this == &other)
            return *this;
        Vertex<T,W>::operator=(other);
        I = other.I;
        return *this;
    }

    /// Krawedzie incydentne
    std::vector<Edge<T,W>*> I;
};

/// @brief Klasa krawedzi grafu w reprezentacji listy sasiadow.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
struct AdjacencyListEdge : Edge<T,W>
{
    /// @brief Podstawowy konstruktor krawedzi.
    /// @param v referencja do jednego wierzcholka koncowego
    /// @param w referencja do drugiego wierzcholka koncowego
    /// @param element wartosc na krawedzi
    AdjacencyListEdge<T,W>(Vertex<T,W>* v, Vertex<T,W>* w, const W& element):
        Edge<T,W>(v, w, element) {}

    /// @brief Konstruktor kopiujacy.
    /// @param other obiekt do skopiowania
    AdjacencyListEdge(const AdjacencyListEdge<T,W>& other):
        Edge<T,W>(other), iteratorIv{other.iteratorIv}, iteratorIw{other.iteratorIw} {}

    /// @brief Przeciazenie operatora kopiujacego-przypisywania
    /// @param other obiekt do skopiowania
    /// @return Zwraca skopiowana krawedz.
    AdjacencyListEdge<T, W>& operator=(const AdjacencyListEdge<T, W>& other)
    {
        if (this == &other)
            return *this;
        Edge<T,W>::operator=(other);
        iteratorIv = other.iteratorIv;
        iteratorIw = other.iteratorIw;
        return *this;
    }

    // Iteratory w strukturze krawedzi incydentnych.

    typename std::vector<Edge<T, W>*>::iterator iteratorIv;
    typename std::vector<Edge<T, W>*>::iterator iteratorIw;
};

template <typename T, typename W>
class AdjacencyListGraph : public GraphADT<T,W>
{
    public:
        /// @brief "Czysci" graf. Wynikiem dzialania jest pusta struktura.
        void clear()
        {
            V.clear();
            E.clear();
        }

        /// @return Zwraca rozmiar struktury przechowujacej wierzcholki.
        const sizeType sizeV() const
            { return V.size(); }

        /// @return Zwraca rozmiar struktury przechowujacej krawedzi.
        const sizeType sizeE() const
            { return E.size(); }

        /// @brief Funkcja zwracajaca wierzcholki koncowe krawedzi.
        /// @param e referencja do krawedzi w grafie
        /// @return Zwraca tablice STL o rozmiarze 2, przechowujaca referencje do wierzcholkow.
        std::array<Vertex<T,W>*,2> endVertices(const Edge<T,W>* e) const
            { return std::array<Vertex<T,W>*,2>{ e->v, e->w }; }

        /// @brief Funkcja znajdujaca wierzcholek sasiedni po danej krawedzi.
        /// @param v referencja do wierzcholka w grafie
        /// @param e referencja do krawedzi w grafie
        /// @return Zwraca referencje do wierzcholka sasiedniego.
        Vertex<T,W>* opposite(const Vertex<T,W>* v, const Edge<T,W>* e) const
            { return e->v == v ? e->w : e->v; }

        /// @brief Sprawdza czy dane wierzcholki sa sasiednie.
        /// @param v referencja do pierwszego wierzcholka w grafie
        /// @param w referencja do drugiego wierzcholka w grafie
        /// @return Zwraca prawde jezeli wierzcholki sa sasiednie. W przec
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

        /// @brief Modyfikuje wartosc na wierzcholku.
        /// @param v referencja do wierzcholka w grafie
        /// @param x nowa wartosc
        void replace(Vertex<T,W>* const v, const T& x)
            { v->element = x; }

        /// @brief Modyfikuje wartosc na krawedzi.
        /// @param e referencja do krawedzi w grafie
        /// @param x nowa wartosc
        void replace(Edge<T,W>* const e, const W& x)
            { e->element = x; }

        /// @brief Dodaje wierzcholek do grafu.
        /// @param x wartosc przechowywana na wierzcholku
        /// @return Zwraca referencje do nowo utworzonego wierzcholka.
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

        /// @brief Dodaje krawedz do grafu.
        /// @param v jeden z wierzcholkow koncowych nowej krawedzi
        /// @param w drugi z wierzcholkow koncowych nowej krawedzi
        /// @param x wartosc przechowywana na krawedzi
        /// @return Zwraca referencje do nowo utworzonej krawedzi.
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

        /// @brief Usuwa wierzcholek z grafu.
        /// @param v referencja do wierzcholka w grafie
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

        /// @brief Usuwa krawedz z grafu
        /// @param e referencja do krawedzi w grafie
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

        /// @param v referencja do wierzcholka w grafie
        /// @return Zwraca std::vector przechowujacy referencje do krawedzi incydentnych.
        std::vector<Edge<T,W>*> incidentEdges(const Vertex<T,W>* const v) const
        {
            std::vector<Edge<T,W>*> vect;
            for (auto & i : _alg_cvertex_cptr(v)->I)
                vect.push_back(i);
            return vect;
        }
        
        /// @return Zwraca std::vector przechowujacy referencje do wszystkich wierzcholkow w grafie.
        std::vector<Vertex<T,W>*> vertices() const
        {
            std::vector<Vertex<T,W>*> vect;
            for (auto & i : V)
                vect.push_back(i.get());
            return vect;
        }

        /// @return Zwraca std::vector przechowujacy referencje do wszystkich krawedzi w grafie.
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