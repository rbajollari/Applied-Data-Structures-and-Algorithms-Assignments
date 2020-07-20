//Ryan Bajollari
// Assignment # 3 CPE 593 Algorithms and Data Structures
// Spring 2020
// Instructor: Dr. Aftab Ahmad
// I certifiy that no plagiarism or cheating was done
// in doing this assignment.

#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>

#define min_n 10
#define step 10
#define max_n 10000

using namespace std;
using namespace chrono;

int* create_malarray (int ID);

void det_interview (int **A, int n);

void prob_interview (int **A, int n);

void run_time(int **A, int n, float* t1, float* t2);

void output_run_times(float* det_t, float* prob_t);

int main() {
    srand(time(NULL));
    float det_runtime[4], prob_runtime[4];
    unsigned int n = min_n;
    unsigned int loop = 0;
    int *arr;

    while (n <= max_n) {
        int **mal_report = new int*[n];
        for(int i = 0; i < n; i++) {
            mal_report[i] = create_malarray(i + 1);
        }

        // just to make sure malware arrays are created properly
        if (n == min_n) {
            for (int i = 0; i < 10; i++) {
                arr = mal_report[i];
                for (int j = 0; j < 11; j++) {
                    cout << arr[j] << ' ';
                }
                cout << endl;
            }
        }

        run_time(mal_report, n, &det_runtime[loop], &prob_runtime[loop]);

        delete [] mal_report;

        n *= step;
        ++loop;
    }
    cout << "\n#n\t\tdet_t\t\tprob_t\n\n";
    output_run_times(det_runtime, prob_runtime);

    return 0;
}

int* create_malarray (int ID) {
    int *arr = new int[11];
    arr[0] = ID;
    for (int i = 1; i < 11; i++) {
        arr[i] = rand() % 10;
    }
    return arr;
}

void det_interview (int **A, int n) {
    int *arr;
    for (int i = 0; i < n; i++){
        arr = A[i];
        int arr_sum = 0;
        for (int j = 1; j < 11; j++) {
            arr_sum += arr[j];
            if (arr_sum >= 70) {
                cout << "det: " << "n = " << n << " arrays: " << "Source " << arr[0] << " reported a malware with maliciousness score of " << arr_sum <<
                     " or higher. The source found after interviewing " << i << " candidates." << endl;
                return;
            }
        }
    }
}

void prob_interview (int **A, int n) {
    int *arr;
    int rand_ID;
    int count = 1;
    while (count <= n*10 /* n*10 rather than n to account for possible repeated checks */ ) {
        rand_ID = rand()%n;
        arr = A[rand_ID];
        int arr_sum = 0;
        for (int j = 1; j < 11; j++) {
            arr_sum += arr[j];
            if (arr_sum >= 70) {
                cout << "prob: " << "n = " << n << " arrays: " << "Source " << arr[0] << " reported a malware with maliciousness score of " << arr_sum <<
                     " or higher. The source found after interviewing " << count << " candidates." << endl;
                return;
            }
        }
        count++;
    }
}

void run_time(int **A, int n, float* t1, float* t2) {
    auto start = high_resolution_clock::now();
    det_interview(A, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    *t1 = 1.0*duration.count();


    start = high_resolution_clock::now();
    prob_interview(A, n);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop-start);
    *t2 = 1.0*duration.count();
}

void output_run_times(float *det_t, float *prob_t) {
    int n = min_n;
    int i = 0;
    while (n <= max_n) {
        cout << n << "\t\t" << det_t[i];
        cout << "\t\t" << prob_t[i];
        cout << endl;
        n *= step;
        ++i;
    }
}