#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include "answer.h"


class Chain {
    int id;
    std::vector<int> chain;
    int size;
    double cost;

   public:
    Chain() {
        id = -1;
        size = 0;
        cost = 0;
    }


    void setId(int id_in) { id = id_in; }

    void operator=(Chain const &C) {
        id = C.id;
        size = C.size;
        cost = C.cost;

        if (C.size != 0) {
            chain.resize(size);
            for (int i = 0; i < size; i++) {
                chain[i] = C.chain[i];
            }
        }
    }

    friend std::istream &operator>>(std::istream &in, Chain &C) {
        in >> C.cost >> C.size;
        C.chain.resize(C.size);
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
    int *dataCenterCount;
    int *locks;
    std::list<std::pair<int, double>> *adjList;

    std::map<std::pair<int, int>, double> costMap;


   public:
    DataCenter(int k, int n, double c, int t, int lockSize, Chain *chains)
        : k(k), n(n), c(c), t(t), lockSize(lockSize), chains(chains) {
        snMin = std::floor(double(n) / (c * k));
        snMax = std::ceil((c * n) / double(k));
        std::cout << "k=" << k << " n=" << n << " c=" << c << " t=" << t << " snMin=" << snMin
                  << " snMax=" << snMax << "\n";

        dataCenter = new int[n];
        dataCenterCount = new int[k];
        locks = new int[n];
        adjList = new std::list<std::pair<int, double>>[n];
        for (int i = 0; i < n; i++) {
            dataCenter[i] = -1;
            locks[i] = 0;
        }
        for (int i = 0; i < k; i++) {
            dataCenterCount[i] = 0;
        }

        _createAdjList();
    }

    ~DataCenter() {
        delete[] dataCenter;
        delete[] dataCenterCount;
        delete[] locks;
        delete[] adjList;
    }


    void initialPartition() {
        Chain *sortedChains = new Chain[t];
        for (int i = 0; i < t; i++) {
            sortedChains[i] = chains[i];
        }
        std::sort(sortedChains, sortedChains + t,
                  [](Chain const &a, Chain const &b) -> bool { return a.cost > b.cost; });
        // for (int i = 0; i < t; i++) {
        //     std::cout<<sortedChains[i];
        // }
        int partitionSize = n / k;
        if (partitionSize > snMax) {
            partitionSize = snMax;
        } else if (partitionSize < snMin) {
            partitionSize = snMin;
        }


        int partitionRemainder = n % k;
        int partitionedServiceCount = 0;
        int partitionDatacenterCount = 0;
        int partitionDatacenterIter = 0;

        for (int i = 0; i < t; i++) {
            Chain *chain = &sortedChains[i];
            for (int j = 0; j < chain->size; j++) {
                if (partitionedServiceCount >= n) {
                    break;
                }
                int service = chain->chain[j];
                if (dataCenter[service] == -1) {
                    // Not allocated
                    dataCenter[service] = partitionDatacenterIter;
                    dataCenterCount[partitionDatacenterIter]++;
                    // std::cout << "dataCenter[" << service << "]=" << partitionDatacenterIter
                    //           << "\n";
                    partitionedServiceCount++;
                    partitionDatacenterCount++;

                    if (partitionDatacenterCount >=
                        partitionSize + (partitionRemainder > 0 ? 1 : 0)) {
                        partitionDatacenterIter++;
                        partitionDatacenterCount = 0;
                        partitionRemainder--;
                    }
                }
            }
        }
        for (int service = 0; service < n; service++) {
            if (partitionedServiceCount >= n) {
                break;
            }
            if (dataCenter[service] == -1) {
                // Not allocated
                dataCenter[service] = partitionDatacenterIter;
                dataCenterCount[partitionDatacenterIter]++;
                // std::cout << "dataCenter[" << service << "]=" << partitionDatacenterIter << "\n";
                partitionedServiceCount++;
                partitionDatacenterCount++;

                if (partitionDatacenterCount >= partitionSize + (partitionRemainder > 0 ? 1 : 0)) {
                    partitionDatacenterIter++;
                    partitionDatacenterCount = 0;
                    partitionRemainder--;
                }
            }
        }
        delete[] sortedChains;
    }



    void runOptimization(int maxIter = 10000) {
        _createCostMap();

        double bestCost = calculateCost();
        int *bestDataCenter = new int[n];
        for (int service = 0; service < n; service++) {
            bestDataCenter[service] = dataCenter[service];
        }

        for (int i = 0; i < maxIter; i++) {
            // printCostMap();

            using pair_type_1 = decltype(costMap)::value_type;
            auto maxCostMovePair1 = *std::max_element(
                std::begin(costMap), std::end(costMap),
                [](const pair_type_1 &p1, const pair_type_1 &p2) { return p1.second < p2.second; });

            // std::cout << "maxCostMovePair1=(" << maxCostMovePair1.first.first << ", "
            //           << maxCostMovePair1.first.second << "): " << maxCostMovePair1.second <<
            //           "\n";

            int oldDataCenter = dataCenter[maxCostMovePair1.first.first];

            _moveService(maxCostMovePair1.first.first, maxCostMovePair1.first.second,
                         maxCostMovePair1.second);

            locks[maxCostMovePair1.first.first]++;


            // printDataCenter();
            // printCostMap();


            if ((!_checkBalance(oldDataCenter)) ||
                (!_checkBalance(maxCostMovePair1.first.second))) {
                std::map<std::pair<int, int>, double> filteredCostMap;
                for (const auto &kvPair : costMap) {
                    if (dataCenter[kvPair.first.first] == maxCostMovePair1.first.second &&
                        kvPair.first.second == oldDataCenter) {
                        filteredCostMap[kvPair.first] = kvPair.second;
                    }
                }

                // std::cout << "\n";
                // for (const auto &kvPair : filteredCostMap) {
                //     std::cout << "(" << kvPair.first.first << ", " << kvPair.first.second
                //               << "): " << kvPair.second << "\n";
                // }

                std::pair<std::pair<int, int>, double> maxCostMovePair2;
                if (filteredCostMap.size() != 0) {
                    using pair_type_2 = decltype(filteredCostMap)::value_type;
                    maxCostMovePair2 =
                        *std::max_element(std::begin(filteredCostMap), std::end(filteredCostMap),
                                          [](const pair_type_2 &p1, const pair_type_2 &p2) {
                                              return p1.second < p2.second;
                                          });
                } else {
                    const std::pair<int, int> serviceDatacenterPair(maxCostMovePair1.first.first,
                                                                    oldDataCenter);
                    maxCostMovePair2 = std::pair<std::pair<int, int>, double>(
                        serviceDatacenterPair, maxCostMovePair1.second);
                }

                // std::cout << "maxCostMovePair2=(" << maxCostMovePair2.first.first << ", "
                //           << maxCostMovePair2.first.second << "): " << maxCostMovePair2.second
                //           << "\n\n";

                _moveService(maxCostMovePair2.first.first, maxCostMovePair2.first.second,
                             maxCostMovePair2.second);

                locks[maxCostMovePair2.first.first]++;
            }



            double cost = calculateCost();
            if (cost < bestCost) {
                bestCost = cost;
                for (int service = 0; service < n; service++) {
                    bestDataCenter[service] = dataCenter[service];
                }
            }
            // std::cout << "cost=" << cost << " bestCost=" << bestCost << "\n\n";

            if (_checkLock()) {
                std::cout << "\niterate " << i << " times\n\n";
                break;
            }
        }

        for (int service = 0; service < n; service++) {
            dataCenter[service] = bestDataCenter[service];
        }

        delete[] bestDataCenter;
    }


    double calculateCost() {
        double totalCost = 0.0;
        for (int i = 0; i < t; i++) {
            int prevDatacenter = dataCenter[chains[i].chain[0]];
            for (int j = 1; j < chains[i].size; j++) {
                int currentDatacenter = dataCenter[chains[i].chain[j]];
                if (prevDatacenter != currentDatacenter) {
                    totalCost += chains[i].cost;
                }
                prevDatacenter = currentDatacenter;
            }
        }
        return totalCost;
    }



    void printAdjList() {
        for (int i = 0; i < n; i++) {
            std::cout << "service=" << i << ": ";
            for (const auto &adjPair : adjList[i]) {
                std::cout << "(" << adjPair.first << ", " << adjPair.second << ") ";
            }
            std::cout << "\n";
        }
    }

    void printDataCenter() {
        for (int i = 0; i < n; i++) {
            std::cout << dataCenter[i] << " ";
        }
        std::cout << "\n";
    }

    void printDataCenterCount() {
        for (int i = 0; i < k; i++) {
            std::cout << dataCenterCount[i] << " ";
        }
        std::cout << "\n";
    }

    void printCostMap() {
        for (const auto &kvPair : costMap) {
            std::cout << "(" << kvPair.first.first << ", " << kvPair.first.second
                      << "): " << kvPair.second << "\n";
        }
    }

    friend std::ostream &operator<<(std::ostream &out, DataCenter &DC) {
        std::list<int> serviceDatacenterMap[DC.k];
        for (int service = 0; service < DC.n; service++) {
            int dataCenter = DC.dataCenter[service];
            serviceDatacenterMap[dataCenter].push_back(service);
        }
        for (int i = 0; i < DC.k; i++) {
            out << serviceDatacenterMap[i].size();
            for (int service : serviceDatacenterMap[i]) {
                out << " " << service;
            }
            out << "\n";
        }
        return out;
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

    void _createCostMap() {
        costMap.clear();
        for (int service = 0; service < n; service++) {
            auto adjs = adjList[service];
            double initialValue = 0.0;
            for (const auto &adjPair : adjs) {
                if (dataCenter[service] != dataCenter[adjPair.first]) {
                    continue;
                }
                initialValue -= adjPair.second;
            }
            for (const auto &adjPair : adjs) {
                if (dataCenter[service] == dataCenter[adjPair.first]) {
                    continue;
                }
                std::pair<int, int> serviceDatacenterPair(service, dataCenter[adjPair.first]);
                if (costMap.find(serviceDatacenterPair) == costMap.end()) {
                    costMap[serviceDatacenterPair] = initialValue;
                }
                costMap[serviceDatacenterPair] += adjPair.second;
            }
        }
    }

    // Modify costMap to make locked service has INT_MIN cost
    bool _checkLock() {
        int lockCount = 0;
        for (int service = 0; service < n; service++) {
            if (locks[service] >= lockSize) {
                for (const auto &kvPair : costMap) {
                    if (kvPair.first.first == service) {
                        lockCount++;
                        costMap[kvPair.first] = INT_MIN;
                    }
                }
            }
        }
        return lockCount >= costMap.size();
    }

    bool _checkBalance(int datacenter) {
        int count = dataCenterCount[datacenter];
        return count >= snMin && count <= snMax;
    }

    void _moveService(int service, int newDatacenter, double cost) {
        // int oldDatacenter = dataCenter[service];
        // costMap.erase(std::pair<int, int>(service, newDatacenter));
        // auto adjs = adjList[service];
        // for (const auto &adjPair : adjs) {
        //     if (dataCenter[adjPair.first] == newDatacenter) {
        //         costMap[std::pair<int, int>(adjPair.first, oldDatacenter)] -= adjPair.second;
        //     }
        //     if (dataCenter[adjPair.first] == oldDatacenter) {
        //         costMap[std::pair<int, int>(adjPair.first, newDatacenter)] += 2 * adjPair.second;
        //     }
        // }
        // costMap[std::pair<int, int>(service, oldDatacenter)] = -cost;
        dataCenterCount[newDatacenter]++;
        dataCenterCount[dataCenter[service]]--;
        dataCenter[service] = newDatacenter;

        _createCostMap();
    }
};

void servie_chain_deployment(std::string file_name) {
    // The input file name does NOT includes ".in"
    // You'll need to handle the file I/O here by yourself
    int k, n, t;
    double c;
    int snMin, snMax;


    std::ifstream infile(file_name + ".in");
    infile >> k >> n >> c >> t;
    snMin = std::floor(n / (c * k));
    snMax = std::ceil((c * n) / k);

    std::cout << "k=" << k << " n=" << n << " c=" << c << " t=" << t << " snMin=" << snMin
              << " snMax=" << snMax << "\n";

    int lockSize;
    if (n < 10) {
        lockSize = 5;
    } else if (10 <= n && n < 1000) {
        lockSize = 5;
    } else if (1000 <= n && n < 10000) {
        lockSize = 2;
    } else if (10000 <= n) {
        lockSize = 1;
    }


    Chain *chains = new Chain[t];
    for (int i = 0; i < t; i++) {
        infile >> chains[i];
        chains[i].setId(i);
    }
    infile.close();


    // for (int i = 0; i < t; i++) {
    //     std::cout << chains[i];
    // }
    // std::cout << "\n";


    DataCenter DC(k, n, c, t, lockSize, chains);
    DC.initialPartition();
    // DC.printDataCenterCount();
    DC.runOptimization(10000000 / n - 1);
    // DC.runOptimization(100);
    // DC.printDataCenterCount();
    // std::cout << "cost=" << DC.calculateCost() << "\n";

    // DC.printAdjList();
    // DC.printDataCenter();


    std::ofstream outfile(file_name + ".out");
    outfile << DC;
    // std::cout << DC;
    outfile.close();
    delete[] chains;

    // Don't forget to write the ".out" file
    return;
}
