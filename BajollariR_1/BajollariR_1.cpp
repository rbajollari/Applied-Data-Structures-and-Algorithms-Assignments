//Ryan Bajollari
// Assignment # 1 CPE 593 Algorithms and Data Structures
// Spring 2020
// Instructor: Dr. Aftab Ahmad
// I certifiy that no plagiarism or cheating was done
// in doing this assignment.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <math.h>

using namespace std;
using namespace std::chrono;

#define min_n 10
#define step 10
#define max_n 200

//The following function sets array b = array a
void assign_array(int* b, int* a, int n);

// The following function determines running times t1 and t2
// for two algorithms sorting arrays A and B between index
// values low and high-1
void run_time(int* A, int* B, int low, int high, float* t1, float* t2);

// The following function initializes the array A by having
// each of the elements from A[0] to A[max] get a random
// number between 1 and max)value
void initialize_rand_array(int* A, int n, int max_value);

// The following function uses INSERTION-SORT to sort an array A of size n
void INSERTION_SORT(int* A, int n); //run-time measure

// The following function merges two sorted sub-arrays A[r..q-1]
// and A[q..r] into one sorted array A[p..r]
void MERGE (int *A, int p, int q, int r);

// Following function recursively combines two sorted arrays
// using divide and conquer.
void MERGE_SORT(int* A, int p, int r);

// Following procedure prints three values from the sorted
// array A of size n from three random positions at roughly one-third
// location.
void print3(int* A, int n);
//The following function takes in the array of run times and
// returns the run time per item
float time_per_item(float*);
void output_run_times(float IS_item_t, float MS_item_t, float* IS_t, \
float* MS_t);

int main () {
    srand(time(NULL));
    float IS_runtime[20], MS_runtime[20];
    float IS_timeperitem = 0.0, MS_timeperitem = 0.0;
    unsigned int size = min_n;
    unsigned int loop = 0;
    cout << "#Three randomly selected array elements in order\n";
    cout << "#The first line for INSERTION-SORT and the second for MERGE-SORT.";
    cout << "\n\n";
    cout << setw(7) << left << "#[i]" << setw(7) << left << "A[i]";
    cout << setw(7) << left << "[j]" << setw(7) << left << "A[j]";
    cout << setw(7) << left << "[k]" << setw(7) << left << "A[k]\n" <<endl;

    while (size <= max_n) {
        int* a = new int[size];
        int* b = new int[size];
        initialize_rand_array(a, size, 10*size);
        assign_array(b, a, size);

        run_time(a, b, 0, size, &IS_runtime[loop], &MS_runtime[loop]);

        size += step;
        ++loop;
    }
    IS_timeperitem = time_per_item(IS_runtime);
    MS_timeperitem = time_per_item(MS_runtime);
    cout << "\n#n\t\tIS_t\t\tIS_O\t\tMS_t\t\tMS_O\n\n";
    output_run_times(IS_timeperitem, MS_timeperitem, IS_runtime, MS_runtime);

    return 0;
}

void initialize_rand_array(int* array, int n, int max_value) {
    for (int k = 0; k < n; ++k)
        array[k] = 1 + rand() % (max_value);

}
void INSERTION_SORT(int* array, int n){

    int key;
    int i, j;

    for (j = 1; j < n; ++j){
        key = array[j];
        i = j - 1;
        while ((i >= 0) && (array[i] > key)) {
            array[i + 1] = array[i];
            --i;
        }
        array[i+1] = key;
    }
}
void MERGE (int *A, int p, int q, int r) {
    unsigned int n1 = q - p + 1;
    unsigned int n2 = r - q;
    int L[n1], R[n2];
    unsigned int i, j, k;

    for (i = 0; i < n1; ++i) {
        L[i] = A[p+i];
    }
    for (j = 0; j < n2; ++j) {
        R[j] = A[q+1+j];
    }
    i = 0;
    j = 0;
    k = p;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            ++i;
        }
        else {
            A[k] = R[j];
            ++j;
        }
        k++;
    }
    while (i < n1) {
        A[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        A[k] = R[j];
        j++;
        k++;
    }

}

void MERGE_SORT(int* A, int p, int r) {

    int q;
    if (p < r) {
        q = p+(r-p)/2;
        MERGE_SORT(A, p, q);
        MERGE_SORT(A, q+1, r);
        MERGE(A, p, q, r);
    }

}
void assign_array(int* b, int* a, int n) {
    for (int i = 0; i < n; ++i)
        b[i] = a[i];
}
void print3(int* c, int n) {
    unsigned int one_third = n/3;
    unsigned int index0 = rand() % one_third;
    unsigned int index1 = one_third + rand() % one_third;
    unsigned int index2 = 2*one_third + rand() % one_third;

    cout << setw(7) << left << index0 << setw(7) << left << c[index0];
    cout << setw(7) << left << index1 << setw(7) << left << c[index1];
    cout << setw(7) << left << index2 << setw(7) << left << c[index2] <<endl;
}
void run_time(int* A, int* B, int n1, int n2, float* t1, float* t2) {

    auto start = high_resolution_clock::now();
    INSERTION_SORT(A, n2);
    print3(A, n2);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    *t1 = 1.0*duration.count();

    start = high_resolution_clock::now();
    MERGE_SORT(B, n1, n2);
    print3(B, n2);
    cout << endl;
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop-start);
    *t2 = 1.0*duration.count();
}
float time_per_item(float* t1) {
    float t2 = 0.0;
    for (int k = 0; k < 20; ++k){
        t2 += t1[k];
    }
    t2 = t2/2100.0;
    return t2;
}
void output_run_times(float IS_item_t, float MS_item_t, float* IS_t,\
float* MS_t) {
    int n = 10;
    int i = 0;
    while (i < 20) {
        cout << n << "\t\t" << IS_t[i] << "\t\t" << IS_item_t*n*n;
        cout << "\t\t" << MS_t[i] << "\t\t" << MS_item_t*n*log(n);
        cout << endl;
        ++i;
        n +=10;
    }
}