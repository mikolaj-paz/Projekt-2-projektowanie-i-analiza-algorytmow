#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <iterator>
#include <memory>
#include <iostream>

typedef std::size_t sizeType;

/// @brief Klasa bazowa wierzcholka grafu.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
struct Vertex
{
    // typedef typename std::list<std::unique_ptr<Vertex<T,W>>>::iterator Viterator;

    /// @brief Podstawowy konstruktor wierzcholka.
    /// @param element wartosc na wierzcholku
    Vertex(const T& element): element{element} {}
    // Vertex(const Vertex<T,W>& other): element{other.element}, iterator{other.iterator} {}

    /// @brief Konstruktor kopiujacy.
    /// @param other obiekt do skopiowania
    Vertex(const Vertex<T,W>& other): element{other.element}, i{other.i} {}
    
    /// @brief Przeciazenie operatora kopiujacego-przypisywania
    /// @param other obiekt do skopiowania
    /// @return Zwraca skopiowany wierzcholek.
    Vertex<T,W>& operator=(const Vertex<T,W>& other) 
    { 
        element = other.element;
        // iterator = other.iterator;
        i = other.i;
        return *this;
    }

    T element;

    // Iterator do struktury wierzcholkow
    // Viterator iterator;

    // Indeks krawedzi w strukturze wierzcholkow
    sizeType i;

    friend std::ostream& operator<<(std::ostream& os, const Vertex<T, W>& obj)
    {
        os << obj.element;
        return os;
    }
};

/// @brief Klasa bazowa krawedzi grafu.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
struct Edge
{
    // typedef typename std::list<std::unique_ptr<Edge<T,W>>>::iterator Eiterator;

    /// @brief Podstawowy konstruktor krawedzi.
    /// @param v referencja do jednego wierzcholka koncowego
    /// @param w referencja do drugiego wierzcholka koncowego
    /// @param element wartosc na krawedzi
    Edge(Vertex<T,W>* v, Vertex<T,W>* w, const W& element): element{element}, v{v}, w{w} {}

    // Edge(const Edge<T,W>& other): element{other.element}, v{other.v}, w{other.w}, iterator{other.iterator} {}

    /// @brief Konstruktor kopiujacy.
    /// @param other obiekt do skopiowania
    Edge(const Edge<T,W>& other): element{other.element}, v{other.v}, w{other.w}, i{other.i} {}

    /// @brief Przeciazenie operatora kopiujacego-przypisywania
    /// @param other obiekt do skopiowania
    /// @return Zwraca skopiowana krawedz.
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

    // Iterator do struktury krawedzi
    // Eiterator iterator;

    // Indeks krawedzi w strukturze krawedzi
    sizeType i;

    friend std::ostream& operator<<(std::ostream& os, const Edge<T, W>& obj)
    {
        os << obj.v->element << "--" << obj.element << "--" << obj.w->element;
        return os;
    }
};

/// @brief Klasa abstrakcyjna, tworzaca schemat dla dzialania implementacji grafu.
/// @tparam T typ danych na wierzcholkach grafu
/// @tparam W typ danych na krawedziach grafu
template <typename T, typename W>
class GraphADT
{
    public:
        // Metody dostepu

        /// @return Zwraca rozmiar struktury przechowujacej wierzcholki.
        virtual const sizeType sizeV() const = 0;

        /// @return Zwraca rozmiar struktury przechowujacej krawedzi.
        virtual const sizeType sizeE() const = 0;

        /// @brief Funkcja zwracajaca wierzcholki koncowe krawedzi.
        /// @param e referencja do krawedzi w grafie
        /// @return Zwraca tablice STL o rozmiarze 2, przechowujaca referencje do wierzcholkow.
        virtual std::array<Vertex<T,W>*,2> endVertices(const Edge<T,W>* e) const = 0;

        /// @brief Funkcja znajdujaca wierzcholek sasiedni po danej krawedzi.
        /// @param v referencja do wierzcholka w grafie
        /// @param e referencja do krawedzi w grafie
        /// @return Zwraca referencje do wierzcholka sasiedniego.
        virtual Vertex<T,W>* opposite(const Vertex<T,W>* v, const Edge<T,W>* e) const = 0;

        /// @brief Sprawdza czy dane wierzcholki sa sasiednie.
        /// @param v referencja do pierwszego wierzcholka w grafie
        /// @param w referencja do drugiego wierzcholka w grafie
        /// @return Zwraca prawde jezeli wierzcholki sa sasiednie. W przeciwnym wypadku falsz.
        virtual bool areAdjacent(const Vertex<T,W>* v, const Vertex<T,W>* w) const = 0;
        
        /// @brief Modyfikuje wartosc na wierzcholku.
        /// @param v referencja do wierzcholka w grafie
        /// @param x nowa wartosc
        virtual void replace(Vertex<T,W>* const v, const T& x) = 0;
        
        /// @brief Modyfikuje wartosc na krawedzi.
        /// @param e referencja do krawedzi w grafie
        /// @param x nowa wartosc
        virtual void replace(Edge<T,W>* const e, const W& x) = 0;
        

        // Metody uaktualniajace

        /// @brief Dodaje wierzcholek do grafu.
        /// @param x wartosc przechowywana na wierzcholku
        /// @return Zwraca referencje do nowo utworzonego wierzcholka.
        virtual Vertex<T,W>* insertVertex(const T& x) = 0;

        /// @brief Dodaje krawedz do grafu.
        /// @param v jeden z wierzcholkow koncowych nowej krawedzi
        /// @param w drugi z wierzcholkow koncowych nowej krawedzi
        /// @param x wartosc przechowywana na krawedzi
        /// @return Zwraca referencje do nowo utworzonej krawedzi.
        virtual Edge<T,W>* insertEdge(Vertex<T,W>* v, Vertex<T,W>* w, const W& x) = 0;

        /// @brief Usuwa wierzcholek z grafu.
        /// @param v referencja do wierzcholka w grafie
        virtual void removeVertex(Vertex<T,W>* const v) = 0;

        /// @brief Usuwa krawedz z grafu
        /// @param e referencja do krawedzi w grafie
        virtual void removeEdge(Edge<T,W>* const e) = 0;

        /// @brief "Czysci" graf. Wynikiem dzialania jest pusta struktura.
        virtual void clear() = 0;


        // Metody iterujace

        /// @param v referencja do wierzcholka w grafie
        /// @return Zwraca std::vector przechowujacy referencje do krawedzi incydentnych.
        virtual std::vector<Edge<T,W>*> incidentEdges(const Vertex<T,W>* const v) const = 0;
        
        /// @return Zwraca std::vector przechowujacy referencje do wszystkich wierzcholkow w grafie.
        virtual std::vector<Vertex<T,W>*> vertices() const = 0;
        
        /// @return Zwraca std::vector przechowujacy referencje do wszystkich krawedzi w grafie.
        virtual std::vector<Edge<T,W>*> edges() const = 0;
};

#endif