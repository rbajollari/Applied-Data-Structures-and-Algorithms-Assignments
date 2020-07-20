// Ryan Bajollari
// Assignment # 5 CPE 593 Algorithms and Data Structures
// Spring 2020
// Instructor: Dr. Aftab Ahmad
// I certifiy that no plagiarism or cheating was done
// in doing this assignment.

#include <iostream>
#include <iomanip>
#include <chrono>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace std::chrono;

#define min_n 10
#define step 10
#define max_n 10000

void initialize_array(int* array, int size);

void create_draw_freq_array(int* A, int n);

int PARTITION(int* A, int start, int end);

int RANDOMIZED_SELECT(int* A, int start, int end, int index);

void COUNTING_SORT(int* A, int* D, int k);

void BUCKET_SORT(double* B, int n);

//run time for Randomized Selection
void run_time_RS(int* A, double* t, int* lucky, int* unlucky);

//run time for counting sort
void run_time_CS(int* A, double* t, int n, int* lucky, int* unlucky);

//run time for bucket sort
void run_time_BS(double* B, double* t, int* lucky, int* unlucky);

int main() {
    srand(time(NULL));
    double RS_TIMES[5], CS_TIMES[5], BS_TIMES[5];
    int LUCKY_RS[5], LUCKY_CS[5], LUCKY_BS[5];
    int UNLUCKY_RS[5], UNLUCKY_CS[5], UNLUCKY_BS[5];

    int n = min_n;
    int loop = 0;

    cout << "\n#n\t\tRS_T\t\tCS_T\t\tBS_T\t\tRS_L#\t\tCS_L#\t\tBS_L#\t\tRS_UL#\t\tCS_UL#\t\tBS_UL#\n\n";

    while (n <= max_n) {
        auto* A = new int[13];
        auto* B = new double[13];

        initialize_array(A, 13); // initialize array elements to zero before recording draws
        create_draw_freq_array(A, n);
        for(int i=0; i<13; i++) {
            B[i] = (double)A[i]/(double)n;
        }

        run_time_CS(A, &CS_TIMES[loop], n, &LUCKY_CS[loop], &UNLUCKY_CS[loop]);
        run_time_BS(B, &BS_TIMES[loop], &LUCKY_BS[loop], &UNLUCKY_BS[loop]);
        run_time_RS(A, &RS_TIMES[loop], &LUCKY_RS[loop], &UNLUCKY_RS[loop]);

        cout << n << "\t\t" << RS_TIMES[loop] << "\t\t" << CS_TIMES[loop] << "\t\t" << BS_TIMES[loop]
        << "\t\t" << LUCKY_RS[loop] << "(" << B[12] << ")" << "\t\t" << LUCKY_CS[loop] << "(" << B[12] << ")" << "\t\t" << LUCKY_BS[loop] << "(" << B[12] << ")"
        << "\t\t" << UNLUCKY_RS[loop] << "(" << B[0] << ")" << "\t\t" << UNLUCKY_CS[loop] << "(" << B[0] << ")"<< "\t\t" << UNLUCKY_BS[loop] << "(" << B[0] << ")"
        << endl;

        n *= step;
        loop++;
    }
}

void initialize_array(int* array, int size) {
    for(int i=0; i<size; i++) {
        array[i] = 1;
    }
}

void create_draw_freq_array(int* A, int n) {
    int pick;
    for(int i=0; i<n; i++) {
        pick = rand()%13;
        A[pick]++;
    }
}

int PARTITION(int* array, int start, int end) {
    int x = array[end], i = start;
    for (int j = start; j <= end - 1; j++) {
        if (array[j] <= x) {
            swap(array[i], array[j]);
            i++;
        }
    }
    swap(array[i], array[end]);
    return i;
}

int RANDOMIZED_SELECT(int* array, int start, int end, int index) {
    if (start == end) {
        return array[start];
    }
    int partitionIndex = PARTITION(array, start, end);
    int pivot = partitionIndex - start + 1;
    if (index == pivot) {
        return array[partitionIndex];
    }
    else if (index < pivot) {
        return RANDOMIZED_SELECT(array, start, partitionIndex - 1, index);
    }
    else return RANDOMIZED_SELECT(array, partitionIndex + 1, end, index - pivot);
}

void COUNTING_SORT(int* A, int* D, int k) {
    auto* C = new int[k + 1];
    for (int i = 0; i <= k; i++) {
        C[i] = 0;
    }
    for (int j = 0; A[j]; j++) {
        C[A[j]]++;
    }
    for (int i = 1; i <= k; ++i) {
        C[i] += C[i-1];
    }
    for (int j = 0; A[j]; ++j) {
        D[C[A[j]] - 1] = A[j];
        --C[A[j]];
    }
    for(int i = 0; A[i]; ++i) {
        A[i] = D[i];
    }
}

void BUCKET_SORT(double* B, int n) {
    vector<double> E[n];
    int index = 0;
    for (int i=0; i<n; i++)
    {
        int bucket_index = n*B[i];
        E[bucket_index].push_back(B[i]);
    }
    for (int i=0; i<n; i++)
        sort(E[i].begin(), E[i].end());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < E[i].size(); j++)
            B[index++] = E[i][j];
}

void run_time_RS(int* A, double* t, int* lucky, int* unlucky) {
    auto start = high_resolution_clock::now();
    auto* arr = new int [13];
    for (int i = 0; i < 13; i++){
        arr[i] = A[i];
    }
    int low = RANDOMIZED_SELECT(A, 0, 12, 0);
    int high = RANDOMIZED_SELECT(A, 0, 12, 12);
    for (int i = 0; i < 13; i++) {
        if(arr[i] == low) {
            *unlucky = i + 1;
        }
        if(arr[i] == high) {
            *lucky = i + 1;
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop-start);
    *t = 1.0*duration.count();
}

void run_time_CS(int* A, double* t, int n, int* lucky, int* unlucky) {
    auto start = high_resolution_clock::now();
    auto* arr = new int [13];
    for (int i = 0; i < 13; i++){
        arr[i] = A[i];
    }
    auto* D = new int[13];
    COUNTING_SORT(A, D, n);
    for (int i = 0; i < 13; i++) {
        if(arr[i] == A[0]) {
            *unlucky = i + 1;
        }
        if(arr[i] == A[12]) {
            *lucky = i + 1;
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop-start);
    *t = 1.0*duration.count();
}

void run_time_BS(double* B, double* t, int* lucky, int* unlucky ) {
    auto start = high_resolution_clock::now();
    auto* arr = new double[13];
    for (int i = 0; i < 13; i++){
        arr[i] = B[i];
    }
    BUCKET_SORT(B, 13);
    for (int i = 0; i < 13; i++) {
        if(arr[i] == B[0]) {
            *unlucky = i + 1;
        }
        if(arr[i] == B[12]) {
            *lucky = i + 1;
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop-start);
    *t = 1.0*duration.count();
}

