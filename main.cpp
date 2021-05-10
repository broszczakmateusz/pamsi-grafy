// Mateusz Broszczak 253988

#include <iostream>
#include "Graph.cpp"
#include <time.h>

using namespace std;

#define TYPE int

template<typename T>
void podmenu(Graph<T>* graph) {
    int wybor, newFirstV;
    bool czyPodmenu = true;
    while (true) {

        cout << "Wybierz opcje: " << endl
             << "1 " << "Wyswietl liste sasiedztwa\n"
             << "2 " << "Wyswietl macierz\n"
             << "3 " << "Pokaz sciezke i koszt\n"
             << "4 " << "Zmien wierzcholek poczatkowy\n"
             << "0 " << "Powrot do menu\n";
        cin >> wybor;
        switch (wybor) {
            case 1:
                graph->printList();
                cin.get();
                break;
            case 2:
                graph->printMatrix();
                cin.get();
                break;
            case 3:
                graph->bfList();
                graph->bfMatrix();
                graph->printCost();
                graph->writePath();
                cin.get();
                break;
            case 4:
                cout << "Wybierz od 0 do " << graph->getNumberOfVertexes() << endl;
                cin >> newFirstV;
                graph->setFirstVertex(newFirstV);
                break;
            case 0:
                czyPodmenu = false;
                break;
            default:
                break;
        }

        if (czyPodmenu == false) break;
    }
};
template<typename T>
void menuWczytaj(Graph<T>* graph){
    graph->readFromFile();
    graph->listToMatrix();
    podmenu(graph);
}

template<typename T>
void menuGeneruj(Graph<T>* graph){
    int tmp;
    cout << "Podaj liczbe wierzcholkow: " << endl;
    cin >> tmp;
    graph->setNumberOfVertexes(tmp);

    cout << "Podaj gestosc grafu: " << endl;
    cin >> tmp;
    graph->setDensity(tmp);

    cout << "Podaj wierzcholek poczatkowy od 0 do " << graph->getNumberOfVertexes() << endl;
    cin >> tmp;
    graph->setFirstVertex(tmp);

    graph->generateGraph();
    graph->listToMatrix();
    graph->writeGenerated();

    podmenu(graph);
}
int main(){
    srand(time(0));

    Graph<TYPE> graph;
    graph.setFirstVertex(0);
    graph.setNumberOfVertexes(10);
    graph.setDensity(50);
    int wybor;
    while (true) {
        cout << "Menu: " << endl
             << "1 "<<"Wczytaj graf z pliku\n"
             << "2 " << "Generuj graf\n"
             << "3 " << "Testuj algorytm Bellmana-Forda\n"
             << "0 " << "Wyjscie\n";
        cin >> wybor;
        switch (wybor)
        {
            case 1:
                menuWczytaj(&graph);
                break;
            case 2:
                menuGeneruj(&graph);
                break;
            case 3:
                graph.test();
                break;
            case 0:
                return 0;
                break;
            default:
                cout << "Niepoprawna opcja wyboru" << endl;
                break;
        }
    }
    return 0;
}
