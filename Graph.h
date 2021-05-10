#ifndef GRAFY_GRAPH_H
#define GRAFY_GRAPH_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <list>                  //do generowania grafu
#include <chrono>

using  std::cout;
using  std::endl;

template<typename T>
struct AdjListElem {        // struktura elementu listy sasiedztwa
public:
    int v;                  // numer sasiada
    T weight;               // waga krawedzi
    AdjListElem<T>* next;   // wskaznik na nastepnego sasiada
};

template<typename T>
class AdjList  {                // klasa listy sasiedztwa
public:
    AdjListElem<T> * front, * back;
    unsigned size;
    AdjList() {
        front = back = NULL;
        size = 0;
    };

    ~AdjList() {
        AdjListElem<T>* p;
        while (front) {
            p = front->next;
            delete front;
            front = p;
        }
    };

    // zwraca dlugosc listy
    unsigned get_size() { return size;};

    // Dodaje element na poczatku listy i zwraca jego adres
    AdjListElem<T>* add_front(AdjListElem<T>* p) {
        p->next = front;
        front = p;
        if (!back) back = front; //jesli lista byla pusta
        size++;
        return front;
    }

    // Dodaje element na koncu listy i zwraca jego adres
    AdjListElem<T>* add_back(AdjListElem<T>* p) {
        if (back)
            back->next = p;
        p->next = NULL;
        back = p;
        if (!front) front = back; //jesli lista byla pusta
        size++;
        return back;
    }

    void print() {
        AdjListElem<T>* p;

        if (front == NULL)
            cout << "Blad. Lista jest pusta." << endl;
        else {
            p = front;
            while (p) {
                cout << p->v << " - nr sasiada, " << p->weight << " - waga" << endl;
                p = p->next;
            }
            cout << endl;
        }
    }
};

template<typename T>
class Graph {
private:
    AdjList<T>* arrayOfLists;       // wskaznik na tablice list sasiedztwa
    int** matrix;                   // wskaznik na macierz
    int numberOfVertexes;           // ilosc wierzcholkow
    int density;                    // gestosc grafu
    int numberOfEdges;              // ilosc krawedxzi
    int firstVertex;                //wierzcholek startowy
    T* cost;                        // wskaznik na tablce kosztow
    int* previousVertexes;          // wskanik na tablice poprzednikow
public:
    AdjList<T>* getArrayOfLists() { return arrayOfLists; };
    int getNumberOfVertexes() { return numberOfVertexes; };
    void setNumberOfVertexes(int newNum) { numberOfVertexes = newNum; };
    void setDensity(int newDensity) { density = newDensity; };
    void setFirstVertex(int newFirstV) { firstVertex = newFirstV; };

    void generateGraph();
    void listToMatrix();

    void printMatrix();
    void printList();
    void printCost();

    void bfList();
    void bfMatrix();

    void writeGenerated();
    void writePath();
    void readFromFile();

    void deleteList();
    void deleteMatrix();
    void test();
};

#endif //GRAFY_GRAPH_H
