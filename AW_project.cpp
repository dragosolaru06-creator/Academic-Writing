#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <string>
#include <windows.h>

// ================= SORTING ALGORITHMS =================

template <typename T>
void bubbleSort(T arr[], int n) {
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

template <typename T>
void insertionSort(T arr[], int n) {
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

template <typename T>
void selectionSort(T arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIndex])
                minIndex = j;
        std::swap(arr[i], arr[minIndex]);
    }
}

// Quick Sort
template <typename T>
int partition(T arr[], int low, int high) {
    T pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
        if (arr[j] <= pivot)
            std::swap(arr[++i], arr[j]);

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

template <typename T>
void quickSort(T arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Merge Sort
template <typename T>
void merge(T arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    T* L = new T[n1];
    T* R = new T[n2];

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

template <typename T>
void mergeSort(T arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Heap Sort
template <typename T>
void heapify(T arr[], int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template <typename T>
void heapSort(T arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ================= GENERATORS =================

// INT
void generateRandom(int arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = rand() % 1000000;
}

// FLOAT
void generateRandom(float arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = (float)rand() / RAND_MAX;
}

// CHAR
void generateRandom(char arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = 'a' + rand() % 26;
}

// Generic patterns
template <typename T>
void generateSorted(T arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = (T)i;
}

template <>
void generateSorted<char>(char arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = 'a' + (i % 26);
}

template <typename T>
void generateReverse(T arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = (T)(n - i);
}

template <>
void generateReverse<char>(char arr[], int n) {
    for (int i = 0; i < n; i++) arr[i] = 'z' - (i % 26);
}

template <typename T>
void generateHalf(T arr[], int n) {
    for (int i = 0; i < n / 2; i++) arr[i] = (T)i;
    for (int i = n / 2; i < n; i++) arr[i] = rand();
}

template <typename T>
void generateNearly(T arr[], int n) {
    generateSorted(arr, n);
    for (int i = 0; i < n / 10; i++)
        std::swap(arr[rand() % n], arr[rand() % n]);
}

// ================= RUN ALGO =================

template <typename T>
void runAlgo(const std::string& algo, T arr[], int n) {
    if (algo == "bubble") bubbleSort(arr, n);
    else if (algo == "insertion") insertionSort(arr, n);
    else if (algo == "selection") selectionSort(arr, n);
    else if (algo == "quick") quickSort(arr, 0, n - 1);
    else if (algo == "merge") mergeSort(arr, 0, n - 1);
    else if (algo == "heap") heapSort(arr, n);
}

// ================= WORKER =================

void worker(int n, std::string algo, std::string type, std::string dist) {

    if (type == "int") {
        int* arr = new int[n];

        if (dist == "Random") generateRandom(arr, n);
        else if (dist == "Sorted") generateSorted(arr, n);
        else if (dist == "Reverse") generateReverse(arr, n);
        else if (dist == "Half") generateHalf(arr, n);
        else if (dist == "Nearly") generateNearly(arr, n);

        runAlgo(algo, arr, n);
        delete[] arr;
    }

    else if (type == "float") {
        float* arr = new float[n];

        if (dist == "Random") generateRandom(arr, n);
        else if (dist == "Sorted") generateSorted(arr, n);
        else if (dist == "Reverse") generateReverse(arr, n);
        else if (dist == "Half") generateHalf(arr, n);
        else if (dist == "Nearly") generateNearly(arr, n);

        runAlgo(algo, arr, n);
        delete[] arr;
    }

    else if (type == "char") {
        char* arr = new char[n];

        if (dist == "Random") generateRandom(arr, n);
        else if (dist == "Sorted") generateSorted(arr, n);
        else if (dist == "Reverse") generateReverse(arr, n);
        else if (dist == "Half") generateHalf(arr, n);
        else if (dist == "Nearly") generateNearly(arr, n);

        runAlgo(algo, arr, n);
        delete[] arr;
    }
}

// ================= TIMEOUT =================

double runWithTimeout(std::string algo, std::string type, std::string dist, int n) {

    std::string cmd = "AW_project.exe worker " +
        std::to_string(n) + " " + algo + " " + type + " " + dist;

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(NULL, cmd.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        return -1;

    auto start = std::chrono::high_resolution_clock::now();

    DWORD result = WaitForSingleObject(pi.hProcess, 60000);

    if (result == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return -1;
    }

    auto end = std::chrono::high_resolution_clock::now();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return std::chrono::duration<double>(end - start).count();
}

// ================= MAIN =================

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    if (argc > 1 && std::string(argv[1]) == "worker") {
        worker(
            std::stoi(argv[2]),
            argv[3],
            argv[4],
            argv[5]
        );
        return 0;
    }

    int sizes[] = {20, 50, 100,1000, 10000, 100000, 1000000};
    std::string algos[] = {"bubble", "insertion", "selection", "quick", "merge", "heap"};
    std::string types[] = {"int", "float", "char"};
    std::string dists[] = {"Random", "Sorted", "Reverse", "Half", "Nearly"};

    std::ofstream out("sorting_results.csv");
    out << "Type,Size,Algorithm,Random,Sorted,Reverse,Half,Nearly\n";

    for (auto& type : types) {
        for (int n : sizes) {
            for (auto& algo : algos) {

                out << type << "," << n << "," << algo;

                for (auto& dist : dists) {
                    std::cout << "[" << type << "] n=" << n
                              << " " << algo << " " << dist << "...\n";

                    double t = runWithTimeout(algo, type, dist, n);

                    if (t < 0) out << ",TIMEOUT";
                    else out << "," << t;
                }

                out << "\n";
            }
        }
    }

    out.close();
    return 0;
}