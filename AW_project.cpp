#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <future>
#include <string>

// SORTING ALGORITHMS

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIndex])
                minIndex = j;
        std::swap(arr[i], arr[minIndex]);
    }
}

// ✅ FIXED Quick Sort - Median-of-three pivot + tail recursion (the preevious quick sort implementation had a bug where it could get stuck in an infinite loop for certain inputs, especially with duplicates. The new implementation uses median-of-three pivot selection to improve performance on sorted and nearly sorted data, and it also optimizes tail recursion to prevent stack overflow on large inputs.)
int partition(int arr[], int low, int high) {
    // Median-of-three pivot selection
    int mid = low + (high - low) / 2;
    if (arr[mid] < arr[low]) std::swap(arr[mid], arr[low]);
    if (arr[high] < arr[low]) std::swap(arr[high], arr[low]);
    if (arr[high] < arr[mid]) std::swap(arr[high], arr[mid]);
    
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {  // <= handles duplicates better
            std::swap(arr[++i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    while (low < high) {
        if (high - low < 10) {  // Small arrays: use insertion sort
            for (int i = low + 1; i <= high; i++) {
                int key = arr[i];
                int j = i - 1;
                while (j >= low && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    j--;
                }
                arr[j + 1] = key;
            }
            break;
        }
        
        int pi = partition(arr, low, high);
        
        // Tail recursion optimization - process smaller partition first
        if (pi - low < high - pi) {
            quickSort(arr, low, pi - 1);
            high = pi - 1;  // low stays the same
        } else {
            quickSort(arr, pi + 1, high);
            low = pi + 1;   // high stays the same
        }
    }
}

// Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int *L = new int[n1], *R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Heap Sort
void heapify(int arr[], int n, int i) {
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Radix Sort
void countingSort(int arr[], int n, int exp) {
    int *output = new int[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    for (int i = 0; i < n; i++)
        arr[i] = output[i];

    delete[] output;
}

int max_element(int arr[], int n) {
    int maxVal = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > maxVal)
            maxVal = arr[i];
    return maxVal;
}

void radixSort(int arr[], int n) {
    int maxVal = max_element(arr, n);
    for (int exp = 1; maxVal / exp > 0; exp *= 10)
        countingSort(arr, n, exp);
}

// Tim Sort
void timSort(int arr[], int n) {
    const int RUN = 32;

    for (int i = 0; i < n; i += RUN) {
        int len = std::min(RUN, n - i);
        insertionSort(arr + i, len);
    }

    for (int size = RUN; size < n; size *= 2)
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);
            if (mid < right)
                merge(arr, left, mid, right);
        }
}

// ARRAY GENERATORS

void generateRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;
}

void generateSorted(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = i;
}

void generateReverse(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = n - i;
}

void generateHalf(int arr[], int n) {
    for (int i = 0; i < n / 2; i++) arr[i] = i;
    for (int i = n / 2; i < n; i++) arr[i] = rand() % 1000000;
}

void generateNearly(int arr[], int n) {
    generateSorted(arr, n);
    for (int i = 0; i < n / 10; i++)
        std::swap(arr[rand() % n], arr[rand() % n]);
}

// TIMEOUT MEASURE

template <typename Func>
double measureWithTimeout(Func f, int arr[], int n, double maxSeconds) {
    auto future = std::async(std::launch::async, [=]() {
        auto start = std::chrono::high_resolution_clock::now();
        f(arr, n);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    });

    if (future.wait_for(std::chrono::duration<double>(maxSeconds)) == std::future_status::ready)
        return future.get();
    else
        return -1.0; // TIMEOUT
}

// MAIN

int main() {
    srand(time(nullptr));

    int sizes[] = {20, 50, 100, 500, 1000, 5000, 10000, 100000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    std::ofstream out("sorting_results.csv");
    out << std::fixed << std::setprecision(6);
    out << "Size,Algorithm,Random,Sorted,Reverse,Half,Nearly\n";

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *base = new int[n];
        int *arr = new int[n];

        auto run = [&](std::string name, auto func) {
            std::string t[5];
            const char* types[5] = {"Random", "Sorted", "Reverse", "Half", "Nearly"};

            auto runCase = [&](int caseIndex, auto generator) {
                std::cout << "[SIZE " << n << "] Running " << name
                          << " on " << types[caseIndex] << "...\n";

                generator(base, n);
                memcpy(arr, base, n * sizeof(int));

                double time = measureWithTimeout(func, arr, n, 5.0);

                if (time < 0) {
                    t[caseIndex] = "TIMEOUT";
                    std::cout << "[SIZE " << n << "] " << name
                              << " on " << types[caseIndex] << " TIMEOUT\n";
                } else {
                    t[caseIndex] = std::to_string(time);
                    std::cout << "[SIZE " << n << "] " << name
                              << " on " << types[caseIndex]
                              << " completed in " << time << "s\n";
                }
            };

            runCase(0, generateRandom);
            runCase(1, generateSorted);
            runCase(2, generateReverse);
            runCase(3, generateHalf);
            runCase(4, generateNearly);

            out << n << "," << name;
            for (int i = 0; i < 5; i++)
                out << "," << t[i];
            out << "\n";
        };

        run("Bubble Sort", bubbleSort);
        run("Insertion Sort", insertionSort);
        run("Selection Sort", selectionSort);
        run("Quick Sort", [](int a[], int n) { quickSort(a, 0, n - 1); });
        run("Merge Sort", [](int a[], int n) { mergeSort(a, 0, n - 1); });
        run("Heap Sort", heapSort);
        run("Radix Sort", radixSort);
        run("Tim Sort", timSort);

        delete[] base;
        delete[] arr;
    }

    out.close();
    return 0;
}