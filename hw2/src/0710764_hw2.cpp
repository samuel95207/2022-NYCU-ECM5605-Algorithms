#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

#include "answer.h"


#define INSERTION_SORT_THRESHOLD 16
#define TIM_SORT_RUN 32


using namespace std;


struct ValuePosPair {
    double value;
    long pos;
};

bool valuePosPaircompare(ValuePosPair& a, ValuePosPair& b) { return a.value < b.value; }


template <typename T>
void printVector(const T& t) {
    std::copy(t.cbegin(), t.cend(), std::ostream_iterator<typename T::value_type>(std::cout, " "));
    cout << "\n\n";
}


void InsertionSort(std::vector<ValuePosPair>& arr, int begin, int end) {
    for (int i = begin + 1; i < end; i++) {
        auto key = arr[i];
        int j = i - 1;

        while (j >= begin && arr[j].value > key.value) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }

    // for (int i = begin + 1; i < end; i++) {
    //     int j = i;
    //     while (j > begin && arr[originalPosArr[j - 1]] > arr[originalPosArr[j]]) {
    //         swap(mapArr[originalPosArr[j - 1]], mapArr[originalPosArr[j]]);
    //         swap(originalPosArr[j - 1], originalPosArr[j]);
    //         j--;
    //     }
    // }

    // for (int i = begin + 1; i < end; i++) {
    //     int j = i;
    //     int jPos = originalPosArr[j];
    //     int jMinusPos = originalPosArr[j - 1];
    //     while (j > begin && arr[jMinusPos] > arr[jPos]) {
    //         swap(mapArr[jMinusPos], mapArr[jPos]);
    //         swap(originalPosArr[j - 1], originalPosArr[j]);
    //         j--;

    //         jPos = originalPosArr[j];
    //         jMinusPos = originalPosArr[j - 1];
    //     }
    // }


    // for (int i = begin + 1; i < end; i++) {
    //     int keyOriginalPos = originalPosArr[i];
    //     double key = arr[keyOriginalPos];

    //     int j = i - 1;
    //     while (j >= 0 && arr[originalPosArr[j]] > key) {
    //         mapArr[originalPosArr[j]] = j + 1;
    //         originalPosArr[j + 1] = originalPosArr[j];
    //         j--;
    //     }
    //     mapArr[keyOriginalPos] = j + 1;
    //     originalPosArr[j + 1] = keyOriginalPos;
    // }

    // for (int i = begin + 1; i < end; i++) {
    //     int keyOriginalPos = originalPosArr[i];
    //     double key = arr[keyOriginalPos];

    //     int j = i - 1;
    //     int jPos = originalPosArr[j];
    //     while (j >= 0 && arr[jPos] > key) {
    //         mapArr[jPos] = j + 1;
    //         originalPosArr[j + 1] = jPos;
    //         j--;

    //         jPos = originalPosArr[j];
    //     }
    //     mapArr[keyOriginalPos] = j + 1;
    //     originalPosArr[j + 1] = keyOriginalPos;
    // }

    return;
}


int medianIdx(std::vector<ValuePosPair>& arr, int aIdx, int bIdx, int cIdx) {
    int a = arr[aIdx].value;
    int b = arr[bIdx].value;
    int c = arr[cIdx].value;

    if (a <= b && b <= c) {
        return bIdx;
    }
    if (a <= c && c <= b) {
        return cIdx;
    }
    if (b <= a && a <= c) {
        return aIdx;
    }
    if (b <= c && c <= a) {
        return cIdx;
    }
    if (c <= a && a <= b) {
        return aIdx;
    }
    if (c <= b && b <= a) {
        return bIdx;
    }
    return aIdx;
}


int Partition(std::vector<ValuePosPair>& arr, int begin, int end) {
    // Three random Idx
    int randIdx1 = rand() % (end - begin) + begin;
    int randIdx2 = rand() % (end - begin) + begin;
    int randIdx3 = rand() % (end - begin) + begin;
    int randIdx = medianIdx(arr, randIdx1, randIdx2, randIdx3);

    // Begin, middle, end
    // int randIdx = medianIdx(arr, begin, begin + (end - begin) / 2, end - 1);

    // One random Idx
    // int randIdx = rand() % (end - begin) + begin;

    swap(arr[randIdx], arr[end - 1]);

    double pivot = arr[end - 1].value;
    int i = begin - 1;
    for (int j = begin; j < end - 1; j++) {
        if (arr[j].value < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    i++;
    swap(arr[i], arr[end - 1]);
    return i;
}

void QuickSort(std::vector<ValuePosPair>& arr, int begin, int end) {
    int size = end - begin;
    if (size > 1) {
        if (size < INSERTION_SORT_THRESHOLD) {
            InsertionSort(arr, begin, end);
        } else {
            int pivot = Partition(arr, begin, end);
            QuickSort(arr, begin, pivot);
            QuickSort(arr, pivot + 1, end);
        }

        // int pivot = Partition(arr, mapArr, originalPosArr, begin, end);
        // QuickSort(arr, mapArr, originalPosArr, begin, pivot);
        // QuickSort(arr, mapArr, originalPosArr, pivot + 1, end);
    }
}


void heapify(std::vector<ValuePosPair>& arr, int begin, int end, int i) {
    int n = end - begin;

    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;


    if (l < n && arr[l + begin].value > arr[largest + begin].value) {
        largest = l;
    }


    if (r < n && arr[r + begin].value > arr[largest + begin].value) {
        largest = r;
    }


    if (largest != i) {
        swap(arr[i + begin], arr[largest + begin]);
        heapify(arr, begin, end, largest);
    }
}

void HeapSort(std::vector<ValuePosPair>& arr, int begin, int end) {
    int size = end - begin;
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, begin, end, i);
    }


    for (int i = size - 1; i > 0; i--) {
        swap(arr[i + begin], arr[begin]);
        heapify(arr, begin, begin + i, 0);
    }
}


void IntrosortUtil(std::vector<ValuePosPair>& arr, int begin, int end, int depthLimit) {
    // Count the number of elements
    int size = end - begin;

    // If partition size is low then do insertion sort
    if (size < INSERTION_SORT_THRESHOLD) {
        InsertionSort(arr, begin, end);
        return;
    }

    // If the depth is zero use heapsort
    if (depthLimit == 0) {
        HeapSort(arr, begin, end);
        return;
    }


    // Quick Sort
    int pivot = Partition(arr, begin, end);
    IntrosortUtil(arr, begin, pivot, depthLimit - 1);
    IntrosortUtil(arr, pivot + 1, end, depthLimit - 1);

    return;
}



void IntroSort(std::vector<ValuePosPair>& arr, int begin, int end) {
    int depthLimit = 2 * log(end - begin);

    // Perform a recursive Introsort
    IntrosortUtil(arr, begin, end, depthLimit);

    return;
}



void merge(std::vector<ValuePosPair>& arr, int l, int m, int r) {
    int len1 = m - l + 1, len2 = r - m;

    auto leftArr = new ValuePosPair[len1];
    auto rightArr = new ValuePosPair[len2];

    for (int i = 0; i < len1; i++) {
        leftArr[i] = arr[l + i];
    }
    for (int i = 0; i < len2; i++) {
        rightArr[i] = arr[m + 1 + i];
    }


    int i = 0;
    int j = 0;
    int k = l;

    while (i < len1 && j < len2) {
        if (leftArr[i].value <= rightArr[j].value) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < len1) {
        arr[k] = leftArr[i];
        k++;
        i++;
    }

    while (j < len2) {
        arr[k] = rightArr[j];
        k++;
        j++;
    }

    delete leftArr;
    delete rightArr;
}


void MergeSort(std::vector<ValuePosPair>& arr, int begin, int end) {
    if (begin >= end - 1) return;

    int mid = begin + (end - begin - 1) / 2;
    MergeSort(arr, begin, mid + 1);
    MergeSort(arr, mid + 1, end);
    merge(arr, begin, mid, end - 1);
}



void TimSort(std::vector<ValuePosPair>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i += TIM_SORT_RUN) {
        InsertionSort(arr, i, min((i + TIM_SORT_RUN), n));
    }

    for (int size = TIM_SORT_RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));
            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}



std::vector<long> my_sort(std::vector<long> input) {
    // to get the size of the sequence, use input.size()
    srand(0);

    int size = input.size();
    std::vector<ValuePosPair> inputPairs;
    std::vector<long> result;

    inputPairs.resize(size);
    result.resize(size);

    for (int i = 0; i < size; i++) {
        auto inputPairPtr = &inputPairs[i];
        inputPairPtr->value = f(input[i]);
        inputPairPtr->pos = i;
    }

    // for (int i = 0; i < size; i++) {
    //     cout << inputPairs[i].value << " ";
    // }
    // cout << endl;

    // InsertionSort(inputPairs, 0, size);
    // QuickSort(inputPairs, 0, size);
    // IntroSort(inputPairs, 0, size);
    // HeapSort(inputPairs, 0, size);
    // MergeSort(inputPairs, 0, size);
    TimSort(inputPairs);
    // sort(inputPairs.begin(), inputPairs.end(), valuePosPaircompare);


    // for (int i = 0; i < size; i++) {
    //     result[inputPairs[i].pos] = i;
    // }

    // for (int i = 0; i < size; i++) {
    //     cout << inputPairs[i].value << " ";
    // }
    // cout << endl;


    return result;
}
