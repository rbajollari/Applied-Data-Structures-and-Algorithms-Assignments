//Ryan Bajollari
// Assignment # 4 CPE 593 Algorithms and Data Structures
// Spring 2020
// Instructor: Dr. Aftab Ahmad
// I certifiy that no plagiarism or cheating was done
// in doing this assignment.

#include <iostream>
#include <iomanip>
#include <chrono>
#include <math.h>

using namespace std;
using namespace std::chrono;

#define min_n 10
#define step 100
#define max_n 1010

//The following function copies array a to array b
void copy_array(int* b, int* a, int n);

//The following function copies array a to array b
void reverse_copy_array(int* c, int* b, int n);

// The following function determines running time of quick sort for a given array size
void sort_time_per_job(int* A, int n, float* t);

// The following function initializes the queue array with random job size
void initialize_queue(int* A, int n);

int Partition(int* A, int start, int end);

void QUICK_SORT(int* A, int start, int end);

int main () {
    srand(time(NULL));
    float sort_times_per_job[11];
    int size = min_n;
    int loop = 0;

    cout << "\n#n\t\tNP_n/3\t\tP_n/3\t\tRP_n/3\t\tNP_2n/3\t\tP_2n/3\t\tRP_2n/3\t\tNP_n\t\tP_n\t\tRP_n\t\tNP_AQT\t\tP_AQT\t\tRP_AQT\n\n";

    while (size <= max_n) {
        int* waiting_timesA = new int[size]; int* waiting_timesB = new int[size]; int* waiting_timesC = new int[size];
        int* queueing_timesA = new int[size]; int* queueing_timesB = new int[size]; int* queueing_timesC = new int[size];
        float average_queueing_timesA = 0, average_queueing_timesB = 0, average_queueing_timesC = 0;

        //initializing waiting and queueing time arrays
        for (int i = 0; i < size; i++) {
            waiting_timesA[i] += 0; waiting_timesB[i] += 0; waiting_timesC[i] += 0;
            queueing_timesA[i] += 0; queueing_timesB[i] += 0; queueing_timesC[i] += 0;
        }

        int* a = new int[size];
        int* b = new int[size];
        int* c = new int[size];

        initialize_queue(a, size);
        copy_array(b, a, size);
        sort_time_per_job(b, size, &sort_times_per_job[loop]);
        reverse_copy_array(c, b, size);

        //calculating waiting times
        for (int i = 0; i < size; i++) {
            for (int j = 0; j <= i; j++){
                waiting_timesA[i] += a[j]; waiting_timesB[i] += b[j]; waiting_timesC[i] += c[j];
            }
        }

        //calculating queueing times
        for (int i = 0; i < size; i++) {
            queueing_timesA[i] += a[i] + waiting_timesA[i];
            queueing_timesB[i] += b[i] + waiting_timesB[i];
            queueing_timesC[i] += c[i] + waiting_timesC[i];
        }

        //calculating average queueing times
        for (int i = 0; i < size; i++) {
            average_queueing_timesA += (float)queueing_timesA[i]/(float)size;
            average_queueing_timesB += (float)queueing_timesB[i]/(float)size + sort_times_per_job[loop];
            average_queueing_timesC += (float)queueing_timesC[i]/(float)size + sort_times_per_job[loop];
        }

        //output to .dat file
        cout << size << "\t\t" << waiting_timesA[size/3] << "\t\t" << waiting_timesB[size/3] << "\t\t" << waiting_timesC[size/3]
        << "\t\t" << waiting_timesA[2*size/3] << "\t\t" << waiting_timesB[2*size/3] << "\t\t" << waiting_timesC[2*size/3]
        << "\t\t" << waiting_timesA[size - 1] << "\t\t" << waiting_timesB[size - 1] << "\t\t" << waiting_timesC[size - 1]
        << "\t\t" << average_queueing_timesA << "\t\t" << average_queueing_timesB << "\t\t" << average_queueing_timesC << endl;

        size += step;
        ++loop;
    }
    return 0;
}

void initialize_queue(int* array, int n) {
    for (int k = 0; k < n; ++k)
        array[k] = 1 + rand() % (100);

}

void copy_array(int* b, int* a, int n) {
    for (int i = 0; i < n; ++i)
        b[i] = a[i];
}

void reverse_copy_array(int* c, int* b, int n) {
    for (int i = 0; i < n; ++i)
        c[i] = b[n-1-i];
}

int Partition(int* array, int start, int end) {
    int pivot = array[end];
    int partitionIndex = start;
    for (int i = start; i < end; i++) {
        if(array[i] <= pivot) {
            swap(array[i], array[partitionIndex]);
            partitionIndex++;
        }
    }
    swap(array[partitionIndex], array[end]);
    return partitionIndex;
}

void QUICK_SORT(int* array, int start, int end){
    if (start < end) {
        int partitionIndex = Partition(array, start, end);
        QUICK_SORT(array, start, partitionIndex - 1);
        QUICK_SORT(array, partitionIndex + 1, end);
    }
}

void sort_time_per_job(int* A, int n, float* t) {
    auto start = high_resolution_clock::now();
    QUICK_SORT(A, 0, n - 1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    *t = duration.count()/n;
}