#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

// Sequential Bubble Sort
void sequentialBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
#pragma omp parallel
    {
        for (int i = 0; i < n - 1; i++) {
#pragma omp for
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

// Sequential Merge Sort
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void sequentialMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void parallelMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            {
                parallelMergeSort(arr, left, mid);
            }
#pragma omp section
            {
                parallelMergeSort(arr, mid + 1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    std::vector<int> arr = { 9, 3, 5, 1, 7, 2, 8, 6, 4 };
// Sequential Bubble Sort
std::vector<int> bubbleSortSequential = arr;
auto startTime = std::chrono::high_resolution_clock::now();
sequentialBubbleSort(bubbleSortSequential);
auto endTime = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> sequentialBubbleSortTime = endTime - startTime;

// Parallel Bubble Sort
std::vector<int> bubbleSortParallel = arr;
startTime = std::chrono::high_resolution_clock::now();
parallelBubbleSort(bubbleSortParallel);
endTime = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> parallelBubbleSortTime = endTime - startTime;

// Sequential Merge Sort
std::vector<int> mergeSortSequential = arr;
startTime = std::chrono::high_resolution_clock::now();
sequentialMergeSort(mergeSortSequential, 0, mergeSortSequential.size() - 1);
endTime = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> sequentialMergeSortTime = endTime - startTime;

// Parallel Merge Sort
std::vector<int> mergeSortParallel = arr;
startTime = std::chrono::high_resolution_clock::now();
parallelMergeSort(mergeSortParallel, 0, mergeSortParallel.size() - 1);
endTime = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> parallelMergeSortTime = endTime - startTime;

// Print sorted arrays and execution times
std::cout << "Sequential Bubble Sort: ";
for (int num : bubbleSortSequential) {
    std::cout << num << " ";
}
std::cout << std::endl;
std::cout << "Execution Time (Sequential Bubble Sort): " << sequentialBubbleSortTime.count() << " seconds" << std::endl;

std::cout << "Parallel Bubble Sort: ";
for (int num : bubbleSortParallel) {
    std::cout << num << " ";
}
std::cout << std::endl;
std::cout << "Execution Time (Parallel Bubble Sort): " << parallelBubbleSortTime.count() << " seconds" << std::endl;

std::cout << "Sequential Merge Sort: ";
for (int num : mergeSortSequential) {
    std::cout << num << " ";
}
std::cout << std::endl;
std::cout << "Execution Time (Sequential Merge Sort): " << sequentialMergeSortTime.count() << " seconds" << std::endl;

std::cout << "Parallel Merge Sort: ";
for (int num : mergeSortParallel) {
    std::cout << num << " ";
}
std::cout << std::endl;
std::cout << "Execution Time (Parallel Merge Sort): " << parallelMergeSortTime.count() << " seconds" << std::endl;

return 0;
}
