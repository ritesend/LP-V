#include<iostream>
#include<omp.h>
#include<iomanip>
#include<cstdlib>
#include<ctime>

using namespace std;

void mergesort(int a[],int i,int j);
void merge(int a[],int i1,int j1,int i2,int j2);

void mergesort(int a[],int i,int j){
    if(i<j){
        int mid=(i+j)/2;

        if(j-i<1000){
            mergesort(a,i,mid);
            mergesort(a,mid+1,j);
        }
        else{
            #pragma omp task firstprivate(a,i,mid)
            {
                mergesort(a,i,mid);
            }
            #pragma omp task firstprivate(a,mid,j)
            {
                mergesort(a,mid+1,j);
            }
            #pragma omp taskwait
        }
        merge(a,i,mid,mid+1,j);
    }
}

void merge(int a[],int i1,int j1,int i2,int j2){
    int size=j2-i1+1;
    int *temp=new int[size];
    int i=i1,j=i2,k=0;

    while(i<=j1 && j<=j2){
        if(a[i]<a[j]){
            temp[k++]=a[i++];
        }
        else{
            temp[k++]=a[j++];
        }
    }
    while(i<=j1){
        temp[k++]=a[i++];
    }

    while(j<=j2){
        temp[k++]=a[j++];
    }

    for(i=i1,k=0;i<=j2;i++,k++){
        a[i]=temp[k];
    }

    delete[] temp;
}

int main(){
    int n;
    cout << "\nEnter total number of elements: ";
    cin >> n;

    int* a = new int[n];
    int* b = new int[n];

    srand(time(0));
    cout<<"Generating random array: "<<endl;
    for(int i=0;i<n;i++){
        a[i]=rand()%100000;
        b[i]=a[i];
    }

    cout << "\nRandom array generated.\n";

    double start_time, end_time;

    start_time=omp_get_wtime();
    mergesort(a,0,n-1);
    end_time=omp_get_wtime();
    double seq_time=end_time-start_time;
    cout << "\nSequential Time: " << fixed << setprecision(8) << seq_time << " seconds" << endl;

    start_time=omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            mergesort(b,0,n-1);
        }
    }
    
    end_time = omp_get_wtime();
    double par_time = end_time - start_time;

    cout << "\nParallel Time: " << fixed << setprecision(8) << par_time << " seconds" << endl;

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