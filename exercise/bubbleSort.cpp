void bubbleSort(int* data, int n){
  int changed = 1;
  while(changed){
    changed = 0;
    for(int i = 0;i < n-1;i++){
      if(data[i+1] < data[i]){
	swap(&data[i+1],&data[i]);
	changed = 1;
      }
    }
  }
}
void swap(int* ptr1,int* ptr2){
  int tmp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = tmp;
}
