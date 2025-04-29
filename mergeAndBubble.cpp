#include <iostream>
#include <iomanip>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// ------------------ BUBBLE SORT FUNCTIONS ------------------

void sequentialBubbleSort(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(int a[], int n) {
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {
                if (a[j] > a[j + 1]) {
                    swap(a[j], a[j + 1]);
                }
            }
        } else {
            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {
                if (a[j] > a[j + 1]) {
                    swap(a[j], a[j + 1]);
                }
            }
        }
    }
}

// ------------------ MERGE SORT FUNCTIONS ------------------

void merge(int a[], int i1, int j1, int i2, int j2) {
    int size = j2 - i1 + 1;
    int* temp = new int[size];
    int i = i1, j = i2, k = 0;

    while (i <= j1 && j <= j2) {
        if (a[i] < a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= j1) temp[k++] = a[i++];
    while (j <= j2) temp[k++] = a[j++];

    for (i = i1, k = 0; i <= j2; i++, k++)
        a[i] = temp[k];

    delete[] temp;
}

void mergesort(int a[], int i, int j) {
    if (i < j) {
        int mid = (i + j) / 2;

        if (j - i < 1000) {
            mergesort(a, i, mid);
            mergesort(a, mid + 1, j);
        } else {
            #pragma omp task firstprivate(a, i, mid)
            mergesort(a, i, mid);
            #pragma omp task firstprivate(a, mid, j)
            mergesort(a, mid + 1, j);
            #pragma omp taskwait
        }

        merge(a, i, mid, mid + 1, j);
    }
}

// ------------------ MAIN FUNCTION ------------------

int main() {
    int n, choice;
    cout << "\nEnter total number of elements: ";
    cin >> n;

    int* a = new int[n];
    int* b = new int[n];

    srand(time(0));
    cout << "\nGenerating random array...\n";
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 100000;
        b[i] = a[i];
    }
    cout << "\nRandom array generated.\n";

    cout << "\nChoose Sorting Algorithm:\n";
    cout << "1. Bubble Sort\n2. Merge Sort\nEnter choice: ";
    cin >> choice;

    double start_time, end_time;

    if (choice == 1) {
        // -------- BUBBLE SORT --------
        start_time = omp_get_wtime();
        sequentialBubbleSort(a, n);
        end_time = omp_get_wtime();
        cout << "\nSequential Bubble Sort Time: " << fixed << setprecision(8) << (end_time - start_time) << " seconds" << endl;

        start_time = omp_get_wtime();
        parallelBubbleSort(b, n);
        end_time = omp_get_wtime();
        cout << "Parallel Bubble Sort Time:   " << fixed << setprecision(8) << (end_time - start_time) << " seconds" << endl;
    }
    else if (choice == 2) {
        // -------- MERGE SORT --------
        start_time = omp_get_wtime();
        mergesort(a, 0, n - 1);
        end_time = omp_get_wtime();
        cout << "\nSequential Merge Sort Time: " << fixed << setprecision(8) << (end_time - start_time) << " seconds" << endl;

        start_time = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp single
            mergesort(b, 0, n - 1);
        }
        end_time = omp_get_wtime();
        cout << "Parallel Merge Sort Time:   " << fixed << setprecision(8) << (end_time - start_time) << " seconds" << endl;
    }
    else {
        cout << "Invalid choice!" << endl;
        delete[] a;
        delete[] b;
        return 1;
    }

    if (n <= 100) {
        cout << "\nSorted array:\n";
        for (int i = 0; i < n; i++) {
            cout << b[i] << " ";
        }
        cout << endl;
    }

    delete[] a;
    delete[] b;
    return 0;
}
