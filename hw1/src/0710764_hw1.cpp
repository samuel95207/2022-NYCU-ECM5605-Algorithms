#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <utility>

#include "answer.h"


class Chain {
    int id;
    int *chain;
    int size;
    double cost;

   public:
    Chain() {
        id = -1;
        size = 0;
        cost = 0;
        chain = nullptr;
    }

    ~Chain() {
        if (chain != nullptr) {
            delete chain;
        }
    }

    void setId(int id_in) { id = id_in; }

    friend std::istream &operator>>(std::istream &in, Chain &C) {
        in >> C.cost >> C.size;
        C.chain = new int[C.size];
        for (int i = 0; i < C.size; i++) {
            in >> C.chain[i];
        }
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, Chain &C) {
        out << "id=" << C.id << " size=" << C.size << " cost=" << C.cost << "\n";
        for (int i = 0; i < C.size; i++) {
            out << C.chain[i] << " ";
        }
        out << "\n";
        return out;
    }

    friend class DataCenter;
};


class DataCenter {
    std::string filename;
    int k, n, t;
    double c;
    int snMin, snMax;
    int lockSize;

    Chain *chains;
    int *dataCenter;
    int *locks;
    std::list<std::pair<int, double>> *adjList;


   public:
    DataCenter(int k, int n, int c, int t, int lockSize, Chain *chains)
        : k(k), n(n), c(c), t(t), lockSize(lockSize), chains(chains) {
        snMin = std::floor(n / (c * k));
        snMax = std::ceil((c * n) / k);

        dataCenter = new int[t];
        locks = new int[t];
        adjList = new std::list<std::pair<int, double>>[n];
        for (int i = 0; i < t; i++) {
            dataCenter[i] = -1;
            locks[i] = 0;
        }
        _createAdjList();
    }

    ~DataCenter() {
        delete[] dataCenter;
        delete[] locks;
        delete[] adjList;
    }

    void printAdjList() {
        for (int i = 0; i < n; i++) {
            std::cout << "service=" << i << ": ";
            for (auto adjPair : adjList[i]) {
                std::cout << "(" << adjPair.first << ", " << adjPair.second << ") ";
            }
            std::cout << "\n";
        }
    }


   private:
    void _createAdjList() {
        for (int i = 0; i < t; i++) {
            Chain *chain = &chains[i];
            for (int j = 0; j < chain->size; j++) {
                int prevIdx = j - 1;
                int nextIdx = j + 1;
                int service = chain->chain[j];
                if (prevIdx >= 0) {
                    adjList[service].push_back(
                        std::pair<int, double>(chain->chain[prevIdx], chain->cost));
                }
                if (nextIdx < chain->size) {
                    adjList[service].push_back(
                        std::pair<int, double>(chain->chain[nextIdx], chain->cost));
                }
            }
        }
    }
};

void servie_chain_deployment(std::string file_name) {
    // The input file name does NOT includes ".in"
    // You'll need to handle the file I/O here by yourself
    int k, n, t;
    double c;
    int snMin, snMax;

    int lockSize = 1;

    std::ifstream infile(file_name + ".in");
    infile >> k >> n >> c >> t;
    snMin = std::floor(n / (c * k));
    snMax = std::ceil((c * n) / k);

    std::cout << "k=" << k << " n=" << n << " c=" << c << " t=" << t << " snMin=" << snMin
              << " snMax=" << snMax << "\n";

    Chain chains[t];
    for (int i = 0; i < t; i++) {
        infile >> chains[i];
        chains[i].setId(i);
    }


    for (int i = 0; i < t; i++) {
        std::cout << chains[i];
    }

    DataCenter DC(k, n, c, t, lockSize, chains);
    DC.printAdjList();




    // Don't forget to write the ".out" file
    return;
}
