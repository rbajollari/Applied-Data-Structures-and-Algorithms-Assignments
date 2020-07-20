//Ryan Bajollari
// Assignment # 2 CPE 593 Algorithms and Data Structures
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

#define min_n 2
#define step 2
#define max_n 1024

//initializes matrix using random number generator between 0 and n
void initialize_rand_matrix(int *A, int n, int max_value);

//multiplies two matrices using square matrix method
void matrix_multiply_square_matrix(int* A, int* B, int *C, int n);

void matrix_sum(int *A, int *B, int *C, int n);

void matrix_subtract(int *A, int *B, int *C, int n);

//multiplies two matrices using strassen's method
void matrix_multiply_strassen(int* A, int* B, int *C, int n);

void run_time(int* A, int* B, int* C, int n, float* t1, float* t2);

void print_n4(int *C, int n);

//takes in the array of run times and
// returns the run time per item
float time_per_item(float* t1);
void output_run_times(float SM_item_t, float ST_item_t, float* SM_t, float* ST_t);

int main () {
    srand(time(NULL));
    float SM_runtime[10], ST_runtime[10];
    float SM_timeperitem = 0.0, ST_timeperitem = 0.0;
    unsigned int size = min_n;
    unsigned int loop = 0;

    while (size <= max_n) {
        int* a = new int[size*size];

        int* b = new int[size*size];

        int* c = new int[size*size];

        initialize_rand_matrix(a, size, size);
        initialize_rand_matrix(b, size, size);

        run_time(a, b, c, size, &SM_runtime[loop], &ST_runtime[loop]);

        delete [] a;
        delete [] b;
        delete [] c;
        size *= step;
        ++loop;
    }
    SM_timeperitem = time_per_item(SM_runtime);
    ST_timeperitem = time_per_item(ST_runtime);
    cout << "\n#n\t\tSM_t\t\tSM_O\t\tST_t\t\tST_O\n\n";
    output_run_times(SM_timeperitem, ST_timeperitem, SM_runtime, ST_runtime);

    return 0;
}

void initialize_rand_matrix(int* matrix, int n, int max_value) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i*n+j] = 1 + rand() % (max_value);
            if (n==4) {
                cout << "Input matrix: " << endl;
                print_n4(matrix, n);
            }
}

void matrix_multiply_square_matrix(int* A, int* B, int* C, int n) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i*n+j] = 0;
            for (int k = 0; k < n; k++)
            {
                C[i*n+j] += A[i*n+k] * B[k*n + j];
            }
        }
    }

}

void matrix_sum(int* A, int* B, int* C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i*n+j] = A[i*n+j] + B[i*n+j];
        }
    }
}

void matrix_subtract(int* A, int* B, int* C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i*n+j] = A[i*n+j] - B[i*n+j];
        }
    }
}

void matrix_multiply_strassen(int* A , int* B, int* C, int n){
    int* A11 = new int[n*n/4]; int* A12 = new int[n*n/4]; int* A21 = new int[n*n/4]; int* A22 = new int[n*n/4];
    int* B11 = new int[n*n/4]; int* B12 = new int[n*n/4]; int* B21 = new int[n*n/4]; int* B22 = new int[n*n/4];
    int* C11 = new int[n*n/4]; int* C12 = new int[n*n/4]; int* C21 = new int[n*n/4]; int* C22 = new int[n*n/4];
    int* T1 = new int[n*n/4]; int* T2 = new int[n*n/4];
    int* P1 = new int[n*n/4]; int* P2 = new int[n*n/4]; int* P3 = new int[n*n/4]; int* P4 = new int[n*n/4]; int* P5 = new int[n*n/4];
    int* P6 = new int[n*n/4]; int* P7 = new int[n*n/4];

    if (n==1) {
        matrix_multiply_square_matrix(A, B, C, n);
    } else {
        //partition A and B into 4 sections
        for (int i = 0; i < n / 2; i++) {
            for (int j = 0; j < n / 2; j++) {
                A11[(i*(n/2))+j] = A[i*n+j];
                A12[(i*(n/2))+j] = A[(i*n)+(j+n/2)];
                A21[(i*(n/2))+j] = A[((i+n/2)*n)+j];
                A22[(i*(n/2))+j] = A[((i+n/2)*n)+(j+n/2)];

                B11[(i*(n/2))+j] = B[i*n+j];
                B12[(i*(n/2))+j] = B[(i*n)+(j+n/2)];
                B21[(i*(n/2))+j] = B[((i+n/2)*n)+j];
                B22[(i*(n/2))+j] = B[((i+n/2)*n)+(j+n/2)];
            }
        }

        //P1 = A11 * S1
        //S1 = B12 - B22
        matrix_subtract(B12, B22, T1, n/2);
        matrix_multiply_strassen(A11, T1, P1, n/2);

        //P2 = S2 * B22
        //S2 = A11 + A12
        matrix_sum(A11, A12, T1, n/2);
        matrix_multiply_strassen(B22, T1, P2, n/2);

        //P3 = S3 * B11
        //S3 = A21 + A22
        matrix_sum(A21, A22, T1, n/2);
        matrix_multiply_strassen(B11, T1, P3, n/2);

        //P4 = A22 * S4
        //S4 = B21 - B11
        matrix_subtract(B21, B11, T1, n/2);
        matrix_multiply_strassen(A22, T1, P4, n/2);

        //P5 = S5 * S6
        //S5 = A11 + A22
        matrix_sum(A11, A22, T1, n/2);
        //S6 = B11 + B22
        matrix_sum(B11, B22, T2, n/2);
        matrix_multiply_strassen(T1, T2, P5, n/2);

        //P6 = S7 * S8
        //S7 = A12 - A22
        matrix_subtract(A12, A22, T1, n/2);
        //S8 = B21 + B22
        matrix_sum(B21, B22, T2, n/2);
        matrix_multiply_strassen(T1, T2, P6, n/2);

        //P7 = S9 * S10
        //S9 = A11 - A21
        matrix_subtract(A11, A21, T1, n/2);
        //S10 = B11 + B12
        matrix_sum(B11, B12, T2, n/2);
        matrix_multiply_strassen(T1, T2, P7, n/2);

        //C11 = P5 + P4 - P2 + P6
        matrix_sum(P5, P4, T1, n/2);
        matrix_subtract(T1, P2, T2, n/2);
        matrix_sum(T2, P6, C11, n/2);

        //C12 = P1 + P2
        matrix_sum(P1, P2, C12, n/2);

        //C21 = P3 + P4
        matrix_sum(P3, P4, C21, n/2);

        //C22 = P5 + P1 - P3 - P7
        matrix_sum(P5, P1, T1, n/2);
        matrix_subtract(T1, P3, T2, n/2);
        matrix_subtract(T2, P7, C22, n/2);

        //Set the result to C
        for (int i = 0; i < n / 2; i++) {
            for (int j = 0; j < n / 2; j++) {
                C[i*n+j] = C11[(i*(n/2))+j];
                C[(i*n)+(j+n/2)] = C12[(i*(n/2))+j];
                C[((i+n/2)*n)+j] = C21[(i*(n/2))+j];
                C[((i+n/2)*n)+(j+n/2)] = C22[(i*(n/2))+j];
            }
        }
    }
    delete [] A11; delete [] A12; delete [] A21; delete [] A22;
    delete [] B11; delete [] B12; delete [] B21; delete [] B22;
    delete [] C11; delete [] C12; delete [] C21; delete [] C22;
    delete [] T1; delete [] T2;
    delete [] P1; delete [] P2; delete [] P3; delete [] P4; delete [] P5;
    delete [] P6; delete [] P7;
}

void print_n4(int *C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << C[i*n+j] << ' ';
        }
        cout << endl;
    }
}


void run_time(int* A, int* B, int* C, int n, float* t1, float* t2) {
    auto start = high_resolution_clock::now();
    matrix_multiply_square_matrix(A, B, C, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop-start);
    *t1 = 1.0*duration.count();
    if (n == 4) {
        cout << "Regularly multiplied n = 4 matrix" << endl;
        print_n4(C, n);
    }

    start = high_resolution_clock::now();
    matrix_multiply_strassen(A, B, C, n);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop-start);
    *t2 = 1.0*duration.count();
    if (n == 4) {
        cout << "Strassen multiplied n = 4 matrix" << endl;
        print_n4(C, n);
    }
}

float time_per_item(float* t1) {
    float t2 = 0.0;
    for (int k = 0; k < 10; ++k){
        t2 += t1[k];
    }
    t2 = t2/1398100.0;
    return t2;
}
void output_run_times(float SM_item_t, float ST_item_t, float* SM_t, float* ST_t) {
    int n = min_n;
    int i = 0;
    while (n <= max_n) {
        cout << n << "\t\t" << SM_t[i] << "\t\t" << SM_item_t * n * n;
        cout << "\t\t" << ST_t[i] << "\t\t" << ST_item_t * n * log(n);
        cout << endl;
        n *= step;
        ++i;
    }
}
