#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

#include "answer.h"


#define INSERTION_SORT_THRESHOLD 16
#define TIM_SORT_RUN 32


using namespace std;

template <typename T>
void printVector(const T& t) {
    std::copy(t.cbegin(), t.cend(), std::ostream_iterator<typename T::value_type>(std::cout, " "));
    cout << "\n\n";
}


void InsertionSort(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
                   int end) {
    for (int i = begin + 1; i < end; i++) {
        int j = i;
        while (j > begin && arr[originalPosArr[j - 1]] > arr[originalPosArr[j]]) {
            swap(mapArr[originalPosArr[j - 1]], mapArr[originalPosArr[j]]);
            swap(originalPosArr[j - 1], originalPosArr[j]);
            j--;
        }
    }

    return;
}


int medianIdx(std::vector<double>& arr, int aIdx, int bIdx, int cIdx) {
    int a = arr[aIdx];
    int b = arr[bIdx];
    int c = arr[cIdx];

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


int Partition(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
              int end) {
    // Three random Idx
    int randIdx1 = rand() % (end - begin) + begin;
    int randIdx2 = rand() % (end - begin) + begin;
    int randIdx3 = rand() % (end - begin) + begin;
    int randIdx = medianIdx(arr, randIdx1, randIdx2, randIdx3);

    // Begin, middle, end
    // int randIdx = medianIdx(arr, begin, begin + (end - begin) / 2, end - 1);

    // One random Idx
    // int randIdx = rand() % (end - begin) + begin;

    swap(mapArr[originalPosArr[randIdx]], mapArr[originalPosArr[end - 1]]);
    swap(originalPosArr[randIdx], originalPosArr[end - 1]);

    double pivot = arr[originalPosArr[end - 1]];
    int i = begin - 1;
    for (int j = begin; j < end - 1; j++) {
        if (arr[originalPosArr[j]] < pivot) {
            i++;
            swap(mapArr[originalPosArr[i]], mapArr[originalPosArr[j]]);
            swap(originalPosArr[i], originalPosArr[j]);
        }
    }
    i++;
    swap(mapArr[originalPosArr[i]], mapArr[originalPosArr[end - 1]]);
    swap(originalPosArr[i], originalPosArr[end - 1]);
    return i;
}

void QuickSort(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
               int end) {
    int size = end - begin;
    if (size > 1) {
        if (size < INSERTION_SORT_THRESHOLD) {
            InsertionSort(arr, mapArr, originalPosArr, begin, end);
        } else {
            int pivot = Partition(arr, mapArr, originalPosArr, begin, end);
            QuickSort(arr, mapArr, originalPosArr, begin, pivot);
            QuickSort(arr, mapArr, originalPosArr, pivot + 1, end);
        }

        // int pivot = Partition(arr, mapArr, originalPosArr, begin, end);
        // QuickSort(arr, mapArr, originalPosArr, begin, pivot);
        // QuickSort(arr, mapArr, originalPosArr, pivot + 1, end);
    }
}


void heapify(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin, int end,
             int i) {
    int n = end - begin;

    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;


    if (l < n && arr[originalPosArr[l + begin]] > arr[originalPosArr[largest + begin]]) {
        largest = l;
    }


    if (r < n && arr[originalPosArr[r + begin]] > arr[originalPosArr[largest + begin]]) {
        largest = r;
    }


    if (largest != i) {
        swap(mapArr[originalPosArr[i + begin]], mapArr[originalPosArr[largest + begin]]);
        swap(originalPosArr[i + begin], originalPosArr[largest + begin]);
        heapify(arr, mapArr, originalPosArr, begin, end, largest);
    }
}

void HeapSort(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
              int end) {
    int size = end - begin;
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(arr, mapArr, originalPosArr, begin, end, i);
    }


    for (int i = size - 1; i > 0; i--) {
        swap(mapArr[originalPosArr[i + begin]], mapArr[originalPosArr[0 + begin]]);
        swap(originalPosArr[i + begin], originalPosArr[0 + begin]);
        heapify(arr, mapArr, originalPosArr, begin, begin + i, 0);
    }
}


void IntrosortUtil(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
                   int end, int depthLimit) {
    // Count the number of elements
    int size = end - begin;

    // If partition size is low then do insertion sort
    if (size < INSERTION_SORT_THRESHOLD) {
        InsertionSort(arr, mapArr, originalPosArr, begin, end);
        return;
    }

    // If the depth is zero use heapsort
    if (depthLimit == 0) {
        HeapSort(arr, mapArr, originalPosArr, begin, end);
        return;
    }


    // Quick Sort
    int pivot = Partition(arr, mapArr, originalPosArr, begin, end);
    IntrosortUtil(arr, mapArr, originalPosArr, begin, pivot, depthLimit - 1);
    IntrosortUtil(arr, mapArr, originalPosArr, pivot + 1, end, depthLimit - 1);

    // IntrosortUtil(arr, begin, partitionPoint - 1, depthLimit - 1);
    // IntrosortUtil(arr, partitionPoint + 1, end, depthLimit - 1);

    return;
}



void IntroSort(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
               int end) {
    int depthLimit = 2 * log(end - begin);

    // Perform a recursive Introsort
    IntrosortUtil(arr, mapArr, originalPosArr, begin, end, depthLimit);

    return;
}



void merge(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int l, int m,
           int r) {
    int len1 = m - l + 1, len2 = r - m;

    int* leftOriginalPosArr = new int[len1];
    int* rightOriginalPosArr = new int[len2];


    for (int i = 0; i < len1; i++) {
        leftOriginalPosArr[i] = originalPosArr[l + i];
    }
    for (int i = 0; i < len2; i++) {
        rightOriginalPosArr[i] = originalPosArr[m + 1 + i];
    }

    // cout << "left ";
    // for (int i = 0; i < len1; i++) {
    //     cout << arr[leftOriginalPosArr[i]] << " ";
    // }
    // cout << endl << "right ";
    // for (int i = 0; i < len2; i++) {
    //     cout << arr[rightOriginalPosArr[i]] << " ";
    // }
    // cout << endl;




    int i = 0;
    int j = 0;
    int k = l;

    // cout << "len1=" << len1 << " len2=" << len2 << endl;
    while (i < len1 && j < len2) {
        // cout << "i=" << i << " j=" << j << " arr[i]=" << arr[leftOriginalPosArr[i]]
        //      << " arr[j]=" << arr[rightOriginalPosArr[j]];
        if (arr[leftOriginalPosArr[i]] <= arr[rightOriginalPosArr[j]]) {
            int originPos = leftOriginalPosArr[i];
            // cout << " choose " << arr[originPos] << endl;
            mapArr[originPos] = k;
            originalPosArr[k] = originPos;
            i++;
        } else {
            int originPos = rightOriginalPosArr[j];
            // cout << " choose " << arr[originPos] << endl;
            mapArr[originPos] = k;
            originalPosArr[k] = originPos;
            j++;
        }
        k++;
    }

    while (i < len1) {
        int originPos = leftOriginalPosArr[i];
        mapArr[originPos] = k;
        originalPosArr[k] = originPos;
        k++;
        i++;
    }

    while (j < len2) {
        int originPos = rightOriginalPosArr[j];
        mapArr[originPos] = k;
        originalPosArr[k] = originPos;
        k++;
        j++;
    }

    // for (int i = l; i <= r; i++) {
    //     cout << arr[mapArr[i]] << " ";
    // }
    // cout << endl << endl;

    delete leftOriginalPosArr;
    delete rightOriginalPosArr;
}


void MergeSort(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
               int end) {
    if (begin >= end - 1) return;

    int mid = begin + (end - begin - 1) / 2;
    // cout << "split " << begin << " - " << mid << " " << mid + 1 << " - " << end - 1 << endl;
    MergeSort(arr, mapArr, originalPosArr, begin, mid + 1);
    MergeSort(arr, mapArr, originalPosArr, mid + 1, end);
    merge(arr, mapArr, originalPosArr, begin, mid, end - 1);
}


void TimSort(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr) {
    int n = arr.size();

    for (int i = 0; i < n; i += TIM_SORT_RUN) {
        InsertionSort(arr, mapArr, originalPosArr, i, min((i + TIM_SORT_RUN), n));
    }

    for (int size = TIM_SORT_RUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = min((left + 2 * size - 1), (n - 1));
            if (mid < right) {
                merge(arr, mapArr, originalPosArr, left, mid, right);
            }
        }
    }
}




std::vector<long> my_sort(std::vector<long> input) {
    // to get the size of the sequence, use input.size()
    srand(0);

    int size = input.size();
    std::vector<double> doubleInput;
    std::vector<long> originalPos;
    std::vector<long> result;

    doubleInput.resize(size);
    originalPos.resize(size);
    result.resize(size);

    for (int i = 0; i < size; i++) {
        doubleInput[i] = f(input[i]);
        originalPos[i] = i;
        result[i] = i;
    }

    // printVector(doubleInput);

    // sort(doubleInput.begin(), doubleInput.end());


    // InsertionSort(doubleInput, result, originalPos, 0, size);
    // QuickSort(doubleInput, result, originalPos, 0, size);
    // IntroSort(doubleInput, result, originalPos, 0, size);
    // HeapSort(doubleInput, result, originalPos, 0, size);
    // MergeSort(doubleInput, result, originalPos, 0, size);
    TimSort(doubleInput, result, originalPos);


    // std::vector<double> sortedArr;
    // for (int i = 0; i < size; i++) {
    //     sortedArr.push_back(doubleInput[result[i]]);
    // }
    // printVector(sortedArr);
    // printVector(result);




    return result;
}
