#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <memory>
#include <utility>

#include "answer.h"

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class Chain {
    int id;
    std::shared_ptr<int[]> chain;
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
        chain = C.chain;
    }

    friend std::istream &operator>>(std::istream &in, Chain &C) {
        in >> C.cost >> C.size;
        std::shared_ptr<int[]> chain(new int[C.size]);
        for (int i = 0; i < C.size; i++) {
            in >> chain[i];
        }
        C.chain = chain;
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

    std::shared_ptr<Chain[]> chains;
    int *dataCenter;
    int *dataCenterCount;
    int *locks;
    std::list<std::pair<int, double>> *adjList;

    std::unordered_map<std::pair<int, int>, double, pair_hash> costMap;
    // std::map<std::pair<int, int>, double> costMap;


   public:
    DataCenter(int k, int n, double c, int t, int lockSize, std::shared_ptr<Chain[]> chains)
        : k(k), n(n), c(c), t(t), lockSize(lockSize), chains(chains) {
        snMin = std::floor(double(n) / (c * k));
        snMax = std::ceil((c * n) / double(k));
        // std::cout << "k=" << k << " n=" << n << " c=" << c << " t=" << t << " snMin=" << snMin
        //           << " snMax=" << snMax << "\n";

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
    }

    ~DataCenter() {
        delete[] dataCenter;
        delete[] dataCenterCount;
        delete[] locks;
        delete[] adjList;
    }


    void initialPartition() {
        std::sort(chains.get(), chains.get() + t,
                  [](Chain const &a, Chain const &b) -> bool { return a.cost > b.cost; });
        // for (int i = 0; i < t; i++) {
        //     std::cout<<chains[i];
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
            Chain *chain = &chains[i];
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
    }



    void runOptimization(int maxIter = 10000) {
        if (maxIter == 0) {
            return;
        }
        _createAdjList();
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
                std::unordered_map<std::pair<int, int>, double, pair_hash> filteredCostMap;
                // std::map<std::pair<int, int>, double> filteredCostMap;
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

        // clock_t start, finish;
        // start = clock();

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

        // finish = clock();
        // std::cout << "Create Adj List :\t " << double(finish - start) / CLOCKS_PER_SEC << " seconds\n";
    }

    void _createCostMap() {
        
        // clock_t start, finish;
        // start = clock();

        costMap.clear();
        for (int service = 0; service < n; service++) {
            // std::cout << "service=" << service << "\n";
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
                    costMap.insert(std::pair<std::pair<int, int>,double>(serviceDatacenterPair,initialValue));
                }
                costMap[serviceDatacenterPair] += adjPair.second;
            }
        }

        // finish = clock();
        // std::cout << "Create Cost Map :\t " << double(finish - start) / CLOCKS_PER_SEC << " seconds\n";
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


    // Read parameters
    int k, n, t;
    double c;
    int snMin, snMax;

    std::ifstream infile(file_name + ".in");
    infile >> k >> n >> c >> t;
    snMin = std::floor(n / (c * k));
    snMax = std::ceil((c * n) / k);

    std::cout << "k=" << k << " n=" << n << " c=" << c << " t=" << t << " snMin=" << snMin
              << " snMax=" << snMax << "\n\n";


    // Determine lock size
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


    // Determine iterate times
    int maxIter;
    double nIterIndex = 1;
    double tIterIndex = 1;
    if (n < 10) {
        nIterIndex = 10000000.0 / n;
    } else if (10 <= n && n < 1000) {
        nIterIndex = 10000000.0 / n;
    } else if (1000 <= n && n < 10000) {
        nIterIndex = 10000000.0 / n;
    } else if (10000 <= n && n < 100000) {
        nIterIndex = 10000000.0 / n;
    } else if (100000 <= n && n < 1000000) {
        nIterIndex = 10000000.0 / n * 2;
    } else if (1000000 <= n && n < 10000000) {
        nIterIndex = 10000000.0 / n * 4;
    }
    if(t < 10000)
        tIterIndex = 1;
    else if(10000 <= t && t < 50000){
        tIterIndex = 5000.0 / t;
    }else if(50000 <= t && t < 100000){
        tIterIndex = 5000.0 / t;
    }else if(100000 <= t && t < 500000){
        tIterIndex = 5000.0 / t;
    }else if(500000 <= t){
        tIterIndex = 0;
    }

    maxIter = nIterIndex * tIterIndex;

    std::cout<<"maxIter="<<maxIter<<"\n\n";


    clock_t start, finish;


    // Read Service Chains
    start = clock();

    std::shared_ptr<Chain[]> chains(new Chain[t]);
    for (int i = 0; i < t; i++) {
        infile >> chains[i];
        chains[i].setId(i);
    }

    finish = clock();
    std::cout << "Read Chain :\t " << double(finish - start) / CLOCKS_PER_SEC << " seconds\n";

    infile.close();


    // Print Chains
    // for (int i = 0; i < t; i++) {
    //     std::cout << chains[i];
    // }
    // std::cout << "\n";


    // Initialize DataCenter
    start = clock();

    DataCenter DC(k, n, c, t, lockSize, chains);

    finish = clock();
    std::cout << "Initialize DC:\t " << double(finish - start) / CLOCKS_PER_SEC << " seconds\n";


    // Initial Partition
    start = clock();

    DC.initialPartition();

    finish = clock();
    std::cout << "Initialize Partition:\t " << double(finish - start) / CLOCKS_PER_SEC
              << " seconds\n";


    // DC.printDataCenterCount();

    // Output initial partition
    std::ofstream outfile1(file_name + ".out");
    outfile1 << DC;
    outfile1.close();


    // Run optimization;
    start = clock();

    DC.runOptimization(maxIter);

    finish = clock();
    std::cout << "Optimization Total:\t " << double(finish - start) / CLOCKS_PER_SEC
              << " seconds\n";
    if (maxIter != 0) {
        std::cout << "Optimization Avg:\t " << double(finish - start) / CLOCKS_PER_SEC / maxIter
                  << " seconds\n";
    }




    // std::cout << "cost=" << DC.calculateCost() << "\n";
    // DC.printAdjList();
    // DC.printDataCenter();
    // DC.printDataCenterCount();



    // Output
    start = clock();

    std::ofstream outfile2(file_name + ".out");
    outfile2 << DC;
    outfile2.close();

    finish = clock();
    std::cout << "Output:\t " << double(finish - start) / CLOCKS_PER_SEC << " seconds\n\n";


    // Don't forget to write the ".out" file
    return;
}
