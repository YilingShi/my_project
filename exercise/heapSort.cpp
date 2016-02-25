#include <iostream>

using namespace std;

void siftDown( int *a, int k, int N); 

void swap(int *m, int *n)
{
  int tmp;
  tmp = *m;
  *m = *n;
  *n = tmp;
}

void heapsort( int a[], int N){       
  /* heapify */    
  for (int k = N/2; k >= 0; k--) { 
    siftDown( a, k, N);    
  }     

  while (N-1 > 0) {  
    /* swap the root(maximum value) of the heap 
       with the last element of the heap */
    swap(a[N-1], a[0]); 
    /* put the heap back in max-heap order */
    siftDown(a, 0, N-1);  
    /* N-- : decrease the size of the heap by one 
       so that the previous max value will
       stay in its proper placement */
    N--;
  }
}  

void siftDown( int *a, int k, int N){     
  while ( k*2 + 1 < N ) { 
    /* For zero-based arrays, the children are 2*i+1 and 2*i+2 */
    int child = 2*k + 1;    

    /* get bigger child if there are two children */
    if ((child + 1 < N) && (a[child] < a[child+1])) child++;        
       
    if (a[k] < a[child]) {   /* out of max-heap order */        
      swap( a[child], a[k] );  
      /* repeat to continue sifting down the child now */          
      k = child;  
    }        
    else            
      return;    
  }
}   

int main()
{    
  int i;    
  int a[] = {19, 17, 16, 12, 9, 15, 1, 2, 11, 7, 3, 10, 14};
  const size_t sz = sizeof(a)/sizeof(a[0]); 
  for (i = 0; i < sz; i++) 
    cout << a[i] << " ";
  cout << endl;
  cout << "----------------------------------" << endl;

  heapsort(a, sz);     

  for (i = 0; i < sz; i++) 
    cout << a[i] << " ";
  cout << endl;

  return 0;
}
