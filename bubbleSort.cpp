#include<iostream>
#include<omp.h>
#include<iomanip>
#include<cstdlib>
#include<ctime>

using namespace std;

void sequentialBubbleSort(int a[],int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){
            if(a[j]>a[j+1]){
                swap(a[j],a[j+1]);
            }
        }
    }
}

void parallelBubbleSort(int a[],int n){
    for(int i=0;i<n;i++){
        // Even Phase
        if(i%2==0){
            #pragma omp parallel for
            for(int j=0;j<n-1;j+=2){
                if(a[j]>a[j+1]){
                    swap(a[j],a[j+1]);
                }
            }
        }
        else{
            #pragma omp parallel for
            for(int j=1;j<n-1;j+=2){
                if(a[j]>a[j+1]){
                    swap(a[j],a[j+1]);
                }
            }
        }
    }
}

int main(){
    int n;
    cout<<"Enter Number of elements: ";
    cin>>n;

    int *a=new int[n];
    int *b=new int[n];

    srand(time(0));
    cout<<"Generating Random Array: "<<endl;
    for(int i=0;i<n;i++){
        a[i]=rand()%100000;
        b[i]=a[i];
    }
    cout<<"Random array generated"<<endl;

    double start_time,end_time;
    start_time=omp_get_wtime();
    sequentialBubbleSort(a,n);
    end_time=omp_get_wtime();
    double seq_time=end_time-start_time;

    cout<<"Sequential Bubble Sort Time"<<fixed<<setprecision(8)<<seq_time<<" Seconds"<<endl;

    start_time=omp_get_wtime();
    parallelBubbleSort(b,n);
    end_time=omp_get_wtime();
    double par_time=end_time-start_time;

    cout<<"Parallel Bubble Sort Time"<<fixed<<setprecision(8)<<par_time<<" Seconds"<<endl;
    
    if(n<=100){
        cout<<"Sorted array: "<<endl;
        for(int i=0;i<n;i++){
            cout<<b[i]<<" ";
        }
        cout<<endl;
    }

    delete[] a;
    delete[] b;

    return 0;
}