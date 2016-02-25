void insertionSort(int* data, int n){
  int boundary = 1;
  while(boundary < n){
    int curr = data[boundary];
    int pos = 0;
    while(pos < boundary && curr > data[pos]){
      pos++;
    }
    shift(data,pos,boundary);
    data[pos] = curr;
    boundary++;
  }
}
void shift(int* data,int pos,int boundary){
  if(pos == boundary){
    return;
  }
  else{
    int tmp = data[boundary];
    int i = boundary;
    while(i > pos){
      data[i] = data[i-1];
      i--;
    }
    data[pos] = tmp;
  }
}
