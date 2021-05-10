#include "Graph.h"

#define MAXWEIGHT  10  // zakres wartowsci wag
#define NUMBEROFTESTS 100  // ile wykonan algorytmu testu

template<typename T>
void Graph<T>::generateGraph() {
    arrayOfLists = new AdjList<T>[numberOfVertexes];  // tworzy dynamiczna tablice list
    int  numOfNeighbors;

    if (density == 100) {
        numOfNeighbors = ((numberOfVertexes * density) / 100) - 1;
    } else {
        numOfNeighbors = ((numberOfVertexes * density) / 100);
    }
    numberOfEdges = numberOfVertexes * numOfNeighbors;

    for (int i = 0; i < numberOfVertexes; i++) {  // dla kazdego wierzcholka
        AdjList<T>* arrayOfListsPtr = &arrayOfLists[i];   //wskaznik na kolejna liste sasiedztwa

        // utworzenie sciezki miedzy koeljnymi wierzcholkami
        AdjListElem<T>* tmp = new AdjListElem<T>;

        if (i == numberOfVertexes - 1) {
            tmp->v = 0;
        } else {
            tmp->v = i + 1;
        }
        tmp->weight = (rand() % MAXWEIGHT) + 1;
        arrayOfListsPtr->add_front(tmp);

        std::list<int> listOfPrevious; // lista juz istniejacych polaczen
        listOfPrevious.push_front(i);   //zeby nie bylo petli z wierzcholka a do a
        listOfPrevious.push_front(i + 1);   // umieszcza nastepny wierzcholek(ten tworzacy sciezke laczaca wszystkie)

        for (int j = 0; j < numOfNeighbors - 1; j++) { // numOfNeighbors -1 zeby stworzyc jedna sciezke laczaca wszystkich
            AdjListElem<T>* p = new  AdjListElem<T>;

            int newVertex = rand() % numberOfVertexes;
            // petla sprawdza liste dotychczasowych saisadow i generuje nowego sasiada nie nalezacego do niej
            while (true) {
                bool notInPrevious = true;
                for (int k : listOfPrevious) {
                    if (newVertex == k) {
                        newVertex = rand() % numberOfVertexes;
                        notInPrevious = true;
                        break;
                    } else notInPrevious = false;
                }
                if (notInPrevious == false) break;
            }

            p->v = newVertex;
            p->weight = (rand() % MAXWEIGHT) + 1;
            arrayOfListsPtr->add_front(p);
            listOfPrevious.push_front(newVertex);
        }
    }
}

template<typename T>
void Graph<T>::listToMatrix() {
    //  maksymalna wartosc dla danego typu
    T max = std::numeric_limits<T>::max();
    // utworzenie macierzy kwadratowej
    matrix = new int* [numberOfVertexes];
    for (int i = 0; i < numberOfVertexes; i++) {
        matrix[i] = new int[numberOfVertexes];
    }

    // gdy nie ma krawedzi ustawianie wag na max dla kazdego pola, 0 na przekatnej
    for (int i = 0; i < numberOfVertexes; i++) {
        for (int j = 0; j < numberOfVertexes; j++) {
            if (j == i) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = max;
            }
        }
    }

    // przypisanie wlasciwych wag
    AdjListElem<T>* tmpVertex;
    int tmp;
    for (int i = 0; i < numberOfVertexes; i++) {
        AdjList<T>* arrayOfListsPtr = &arrayOfLists[i];   //wskaznik na kolejna liste sasiedztwa
        for (tmpVertex = arrayOfListsPtr->front; tmpVertex; tmpVertex = tmpVertex->next) {
            tmp = tmpVertex->v;
            matrix[i][tmp] = tmpVertex->weight;
        }
    }
}

template<typename T>
void Graph<T>::printMatrix() {
    T max = std::numeric_limits<T>::max();  // maksymalna wartosc dla danego typu
    for (int i = 0; i < numberOfVertexes; i++) {
        for (int j = 0; j < numberOfVertexes; j++) {
            if (matrix[i][j] == max) cout << " #\t";
            else cout << " " << matrix[i][j] << "\t";
        }
        cout << endl;
    }

}

template<typename T>
void Graph<T>::printList() {
    for (int i = 0; i < numberOfVertexes; i++) {
        AdjList<T>* arrayOfListsPtr = &arrayOfLists[i];
        cout << "Vertex: " << i << endl;
        arrayOfListsPtr->print();
    }
}

template<typename T>
void Graph<T>::deleteMatrix() {
    for (int i = 0; i < numberOfVertexes; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    matrix = nullptr;
}

template<typename T>
void Graph<T>::deleteList() {
    for (int i = 0; i < numberOfVertexes; i++) {
        AdjList<T>* arrayOfListsPtr = &arrayOfLists[i];   //wskaznik na kolejna liste sasiedztwa
        AdjListElem<T>* p;
        p = arrayOfListsPtr->front;
        if (p) {
            arrayOfListsPtr->front = p->next; // nowy poczatek
            delete p;    // usun element z pamieci
        }
    }
    delete[] arrayOfLists;
}

/*##################################################################################################*/

template<typename T>
void Graph<T>::bfList() {
    cost = new T[numberOfVertexes];   // tablica kosztow
    previousVertexes = new int[numberOfVertexes];  //tablica numerow poprzednikow

    T max = std::numeric_limits<T>::max();  // maksymalna wartosc dla danego typu

    for (int i = 0; i < numberOfVertexes; i++) {
        cost[i] = max;           // dla pozostalych nieskonczonosc
    }

    cost[firstVertex] = 0;   // dla wierzcholka startowego 0

    for (int i = 0; i < numberOfVertexes; i++) {
        previousVertexes[i] = -1;
    }

    AdjListElem<T>* pv;
    for (int i = 1; i < numberOfVertexes; i++) {  // Petla relaksacji
        for (int j = 0; j < numberOfVertexes; j++) { // Przechodzimy przez kolejne wierzcholki grafu
            AdjList<T>* pvL = &arrayOfLists[j];
            for (pv = pvL->front; pv; pv = pv->next) { // Przegladamy liste sasiadow wierzcholka x
                if (cost[pv->v] > cost[j] + pv->weight) {// Sprawdzamy warunek relaksacji

                    cost[pv->v] = cost[j] + pv->weight; // Relaksujemy krawedz z x do jego sasiada
                    previousVertexes[pv->v] = j;   // Poprzednikiem sasiada bedzie x
                }
            }
        }
    }
}

template<typename T>
void Graph<T>::bfMatrix() {
    cost = new T[numberOfVertexes];   // tablica kosztow
    previousVertexes = new int[numberOfVertexes];  //tablica numerow poprzednikow

    T max = std::numeric_limits<T>::max();  // maksymalna wartosc dla danego typu

    for (int i = 0; i < numberOfVertexes; i++) {
        cost[i] = max;           // dla pozostalych nieskonczonosc
    }

    cost[firstVertex] = 0;   // dla wierzcholka startowego 0

    for (int i = 0; i < numberOfVertexes; i++) {
        previousVertexes[i] = -1;
    }

    for (int i = 1; i < numberOfVertexes; i++) {
        for (int j = 0; j < numberOfVertexes; j++) {
            for (int k = 0; k < numberOfVertexes; k++) {
                if (cost[j] != max && matrix[j][k] != max && matrix[k][j] != 0 && cost[k] > cost[j] + matrix[j][k]) {
                    cost[k] = cost[j] + matrix[j][k];
                    previousVertexes[k] = j;
                }
            }
        }
    }
}

/*##################################################################################################*/
template<typename T>
void Graph<T>::writeGenerated() {
    std::fstream file;
    file.open("data.txt", std::ios::out);
    if( file.good() == true ) {
        // liczba krawedzi  liczba wierzcholkow     wierzcholek poczatkowy
        file << numberOfEdges << "\t" << numberOfVertexes << "\t" << firstVertex << endl;
        AdjListElem<T>* pv;
        for (int i = 0; i < numberOfVertexes; i++) {
            AdjList<T>* arrayOfListsPtr = &arrayOfLists[i];
            for (pv = arrayOfListsPtr->front; pv; pv = pv->next) {
                // wierzcholek poczatkowy     wierzcholek koncowy    waga
                file << i << "\t" << pv->v << "\t" << pv->weight << endl;
            }
        }
        file.close();
        cout << "Zapisano do pliku" << endl;
    } else {
        cout << "Blad. Nie udalo sie otworzyc pliku." << endl;
    }
}

template<typename T>
void Graph<T>::printCost() {
    int* tmp = new int[numberOfVertexes];    // pomocnicza tablica ktora uzywam jak stos
    int index = 0;

    for (int i = 0; i < numberOfVertexes; i++) {
        cout << "Wierzcholek " << i <<  " koszt: " << cost[i] << " sciezka: ";

        for (int j = i; j != -1; j = previousVertexes[j]) { // wierzcholki sciezki umieszczamy na stosie
            tmp[index++] = j; // w kolejnosci od ostatniego do pierwszego
        }
        while (index) {      // Wierzcholki ze stosu drukujemy w kolejnowci od pierwszego do ostatniego
            cout << tmp[--index] << " ";
        }
        cout << endl;
    }
    delete[] tmp;         // Usuwamy stos
}

template<typename T>
void Graph<T>::writePath() { //analogicznie jak printCost
    std::fstream file;
    file.open("path.txt", std::ios::out);
    if( file.good() == true ) {
        int *S = new int[numberOfVertexes]; // pomocnicza tablica ktora uzywam jak stos
        int index = 0;

        for (int i = 0; i < numberOfVertexes; i++) {
            file << "Wierzcholek " << i <<  " koszt: " << cost[i] << " sciezka: ";

            for (int j = i; j != -1; j = previousVertexes[j]) {
                S[index++] = j;
            }
            while (index) {
                file << S[--index] << " ";
            }
            file << endl;
        }
        delete[] S;
        file.close();
        cout << "Sciezke zapisano do pliku" << endl;
    } else {
        cout << "Blad. Nie udalo sie otworzyc pliku." << endl;
    }
}

template<typename T>
void Graph<T>::readFromFile() {
    std::ifstream file;
    file.open("data.txt", std::ios_base::in);
    if( file.good() == true ) {
        file >> numberOfEdges >> numberOfVertexes >> firstVertex;
        arrayOfLists = new AdjList<T>[numberOfVertexes];  // tworzy tablice list
        int newV1, newV2;
        T newWeight;
        AdjList<T> *arrayOfListsPtr;

        for (int i = 0; i < numberOfEdges; i++) {
            file >> newV1 >> newV2 >> newWeight;
            arrayOfListsPtr = &arrayOfLists[newV1];
            AdjListElem<T> *p = new AdjListElem<T>;

            p->v = newV2;
            p->weight = newWeight;
            arrayOfListsPtr->add_front(p);
        }
        file.close();
        cout << "Wczytano pomyslnie" << endl;
    } else {
        cout << "Blad. Nie udalo sie otworzyc pliku." << endl;
    }
}

template<typename T>
void Graph<T>::test() {
    int densities[4] = {25, 50, 75, 100 };  //tablica gestosci
    int size[5] = { 10, 50, 100, 500, 1000 }; //tablica rozmiarow

    std::ofstream file1("testList.txt");
    std::ofstream file2("testMatrix.txt");

    if( file1.good() == true  && file2.good() == true) {
        float time, timeList, timeMatrix;

        for (int i = 0; i < 4; i++) { // petla gestosci
            setDensity(densities[i]);
            for (int j = 0; j < 5; j++) {

                timeList = 0;
                timeMatrix = 0;
                for (int k = 0; k < NUMBEROFTESTS; k++) {
                    setNumberOfVertexes(size[j]);
                    generateGraph();
                    listToMatrix();

                    auto start = std::chrono::steady_clock::now();
                    bfList();
                    auto end = std::chrono::steady_clock::now();

                    deleteList();


                    std::chrono::duration<double> elapsed_seconds = end - start;
                    time = elapsed_seconds.count();
                    timeList += time;

                    start = std::chrono::steady_clock::now();
                    bfMatrix();
                    end = std::chrono::steady_clock::now();

                    deleteMatrix();

                    elapsed_seconds = end - start;
                    time = elapsed_seconds.count();
                    timeMatrix += time;
                }

                file1 << timeList << endl;
                std::cout << "list, density " << densities[i] << "% size:" << size[j] << " time: " << timeList << endl;
                file2 << timeMatrix << endl;
                std::cout << "matrix, density " << densities[i] << "% size:" << size[j] << " time: " << timeMatrix << endl;
            }
            file1 << endl;
            file2 << endl;

        }
        file1.close();
        file2.close();
    }   else {
        cout << "Blad. Nie udalo sie otworzyc pliku." << endl;
    }
}

