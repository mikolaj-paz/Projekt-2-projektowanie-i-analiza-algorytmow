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

/// @brief Klasa wierzcholka grafu w reprezentacji macierzy sasiadow.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
struct AdjacencyMatrixVertex : public Vertex<T,W>
{
    /// @brief Podstawowy konstruktor wierzcholka.
    /// @param element wartosc na wierzcholku
    AdjacencyMatrixVertex(const T& element):
        Vertex<T,W>(element) {}

    /// @brief Konstruktor kopiujacy.
    /// @param other obiekt do skopiowania
    AdjacencyMatrixVertex(const AdjacencyMatrixVertex<T,W>& other):
        Vertex<T,W>(other), Aindex{other.Aindex} {}

    /// @brief Przeciazenie operatora kopiujacego-przypisywania
    /// @param other obiekt do skopiowania
    /// @return Zwraca skopiowany wierzcholek.
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

/// @brief Klasa krawedzi grafu w reprezentacji macierzy sasiadow.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
struct AdjacencyMatrixEdge : public Edge<T,W>
{
    /// @brief Podstawowy konstruktor krawedzi.
    /// @param v referencja do jednego wierzcholka koncowego
    /// @param w referencja do drugiego wierzcholka koncowego
    /// @param element wartosc na krawedzi
    AdjacencyMatrixEdge(Vertex<T,W>* v, Vertex<T,W>* w, const W& element):
        Edge<T,W>(v, w, element) {}

    /// @brief Konstruktor kopiujacy.
    /// @param other obiekt do skopiowania
    AdjacencyMatrixEdge(const AdjacencyMatrixEdge<T,W>& other):
        Edge<T,W>(other) {}
    
    /// @brief Przeciazenie operatora kopiujacego-przypisywania
    /// @param other obiekt do skopiowania
    /// @return Zwraca skopiowana krawedz.
    AdjacencyMatrixEdge<T,W>& operator=(const AdjacencyMatrixEdge<T,W>& other)
    {
        if (this == &other)
            return *this;
        Edge<T,W>::operator=(other);
        return *this;
    }
};

/// @brief Klasa implementujaca graf w reprezentacji macierzy sasiadow.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
class AdjacencyMatrixGraph : public GraphADT<T,W>
{
    public:
        /// @brief "Czysci" graf. Wynikiem dzialania jest pusta struktura.
        void clear()
        {
            V.clear();
            E.clear();
            A.clear();
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
            if (A[_amg_cvertex_ptr(v)->Aindex][_amg_cvertex_ptr(w)->Aindex]) 
                return true;
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
            for (int i = 0; i < A.size(); i++)
                A[i].push_back(nullptr);
            _amg_vertex_ptr(V.back().get())->Aindex = A.size();
            A.push_back(std::vector<Edge<T,W>*>(A.size() + 1, nullptr));
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
            // E.back().get()->iterator = last;
            sizeType last = E.size() - 1;
            E.back().get()->i = last;
            A[_amg_vertex_ptr(v)->Aindex][_amg_vertex_ptr(w)->Aindex] = E.back().get();
            A[_amg_vertex_ptr(w)->Aindex][_amg_vertex_ptr(v)->Aindex] = E.back().get();
            //  return (*last).get();
             return E[last].get();
        }

        /// @brief Usuwa wierzcholek z grafu.
        /// @param v referencja do wierzcholka w grafie
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

        /// @brief Usuwa krawedz z grafu
        /// @param e referencja do krawedzi w grafie
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

        /// @param v referencja do wierzcholka w grafie
        /// @return Zwraca std::vector przechowujacy referencje do krawedzi incydentnych.
        std::vector<Edge<T,W>*> incidentEdges(const Vertex<T,W>* const v) const
        {
            std::vector<Edge<T,W>*> vect;
            for (auto & i : A[_amg_cvertex_cptr(v)->Aindex])
                if (i) vect.push_back(i);
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
        std::vector<std::vector<Edge<T,W>*>> A;
};

#endif