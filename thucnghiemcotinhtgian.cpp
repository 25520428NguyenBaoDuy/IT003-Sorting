#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int N = 1000000; // 1 triệu phần tử
const int NUM_ARRAYS = 10;

// --- 1. QuickSort ---
void quickSort(vector<double>& a, int l, int r) {
    if (l >= r) return;
    double pivot = a[l + (r - l) / 2];
    int i = l, j = r;
    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }
    quickSort(a, l, j);
    quickSort(a, i, r);
}

// --- 2. HeapSort ---
void heapify(vector<double>& a, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && a[left] > a[largest]) largest = left;
    if (right < n && a[right] > a[largest]) largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);
        heapify(a, n, largest);
    }
}

void heapSort(vector<double>& a) {
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(a, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}

// --- 3. MergeSort ---
void merge(vector<double>& a, int l, int m, int r, vector<double>& temp) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
    }
    while (i <= m) temp[k++] = a[i++];
    while (j <= r) temp[k++] = a[j++];
    for (i = l; i <= r; i++) a[i] = temp[i];
}

void mergeSort(vector<double>& a, int l, int r, vector<double>& temp) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(a, l, m, temp);
    mergeSort(a, m + 1, r, temp);
    merge(a, l, m, r, temp);
}

int main() {
    srand(time(0));

    // Khởi tạo bộ dữ liệu
    vector<vector<double>> datasets(NUM_ARRAYS, vector<double>(N));

    // Sinh ngẫu nhiên 10 dãy số thực
    for (int i = 0; i < NUM_ARRAYS; i++) {
        for (int j = 0; j < N; j++) {
            datasets[i][j] = (double)rand() / RAND_MAX * 100000.0;
        }
    }

    // Dãy 1 (index 0): Tăng dần
    sort(datasets[0].begin(), datasets[0].end());

    // Dãy 2 (index 1): Giảm dần
    sort(datasets[1].begin(), datasets[1].end(), greater<double>());

    // Mảng lưu trữ thời gian thực thi (đơn vị ms): [Thuật toán][Dãy]
    // 0: std::sort, 1: QuickSort, 2: HeapSort, 3: MergeSort
    double time_results[4][NUM_ARRAYS];

    // --- 1. Đo thời gian std::sort ---
    for (int i = 0; i < NUM_ARRAYS; i++) {
        vector<double> a = datasets[i];
        auto start = high_resolution_clock::now();
        sort(a.begin(), a.end());
        auto end = high_resolution_clock::now();
        time_results[0][i] = duration<double, milli>(end - start).count();
    }

    // --- 2. Đo thời gian QuickSort ---
    for (int i = 0; i < NUM_ARRAYS; i++) {
        vector<double> a = datasets[i];
        auto start = high_resolution_clock::now();
        quickSort(a, 0, N - 1);
        auto end = high_resolution_clock::now();
        time_results[1][i] = duration<double, milli>(end - start).count();
    }

    // --- 3. Đo thời gian HeapSort ---
    for (int i = 0; i < NUM_ARRAYS; i++) {
        vector<double> a = datasets[i];
        auto start = high_resolution_clock::now();
        heapSort(a);
        auto end = high_resolution_clock::now();
        time_results[2][i] = duration<double, milli>(end - start).count();
    }

    // --- 4. Đo thời gian MergeSort ---
    vector<double> temp(N);
    for (int i = 0; i < NUM_ARRAYS; i++) {
        vector<double> a = datasets[i];
        auto start = high_resolution_clock::now();
        mergeSort(a, 0, N - 1, temp);
        auto end = high_resolution_clock::now();
        time_results[3][i] = duration<double, milli>(end - start).count();
    }

    // --- In bảng kết quả ---
    cout << fixed << setprecision(2);
    cout << "==========================================================================================\n";
    cout << "KET QUA THU NGHIEM THOI GIAN THUC THI (ms)\n";
    cout << "==========================================================================================\n";
    cout << left << setw(15) << "Day du lieu"
        << setw(18) << "std::sort (C++)"
        << setw(18) << "QuickSort"
        << setw(18) << "HeapSort"
        << setw(18) << "MergeSort" << "\n";
    cout << "------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < NUM_ARRAYS; i++) {
        string day_label = "Day " + to_string(i + 1);
        if (i == 0) day_label += " (Tang)";
        else if (i == 1) day_label += " (Giam)";

        cout << left << setw(15) << day_label
            << setw(18) << time_results[0][i]
            << setw(18) << time_results[1][i]
            << setw(18) << time_results[2][i]
            << setw(18) << time_results[3][i] << "\n";
    }
    cout << "==========================================================================================\n";

    return 0;
}