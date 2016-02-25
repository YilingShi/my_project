void selectionSort(int* data, int n){
  for(int pos = 0;pos < n;pos++){
    int minIndex = findMin(data,pos,n);
    if(minIndex != pos){
      swap(data,minIndex,pos);
    }
  }
}
int findMin(int* data, int start,int n){
  int min = start;
  int curr = start+1;
  while(curr < n){
    if(data[curr] < data[min]){
      min = curr;
    }
    curr++;
  }
  return min;
}
void swap(int* data,int index,int start){
  int tmp = data[start];
  data[start] = data[index];
  data[index] = tmp;
}
