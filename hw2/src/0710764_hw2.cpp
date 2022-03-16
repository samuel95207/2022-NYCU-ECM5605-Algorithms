#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "answer.h"


#define INSERTION_SORT_THRESHOLD 100

using namespace std;

template <typename T>
void printVector(const T& t) {
    std::copy(t.cbegin(), t.cend(), std::ostream_iterator<typename T::value_type>(std::cout, " "));
    cout << "\n";
}


void InsertionSort(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
                   int end) {
    for (int i = begin + 1; i < end; i++) {
        int j = i;
        while (j > begin && arr[j - 1] > arr[j]) {
            swap(arr[j - 1], arr[j]);
            swap(mapArr[originalPosArr[j - 1]], mapArr[originalPosArr[j]]);
            swap(originalPosArr[j - 1], originalPosArr[j]);
            j--;
        }
    }

    return;
}


int Partition(std::vector<double>& arr, std::vector<long>& mapArr, std::vector<long>& originalPosArr, int begin,
              int end) {
    int pivot = arr[end - 1];
    int i = begin - 1;
    for (int j = begin; j < end; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
            swap(mapArr[originalPosArr[i]], mapArr[originalPosArr[j]]);
            swap(originalPosArr[i], originalPosArr[j]);
        }
    }
    i++;
    swap(arr[i], arr[end - 1]);
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

std::vector<long> my_sort(std::vector<long> input) {
    // to get the size of the sequence, use input.size()
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

    if (size < INSERTION_SORT_THRESHOLD) {
        InsertionSort(doubleInput, result, originalPos, 0, size);
    } else {
        QuickSort(doubleInput, result, originalPos, 0, size);
    }



    // printVector(doubleInput);
    // printVector(result);




    return result;
}
