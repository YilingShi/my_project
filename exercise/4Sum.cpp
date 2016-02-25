vector<vector<int>> fourSum(vector<int>& nums, int target){
  vector<vector<int>> ans_vec;
  if(nums.size() <= 3){
    return ans_vec;
  }
  sort(nums.begin(),nums.end());
  for(int i = 0; i < nums.size()-3;i++){
    for(int l = nums.size()-1;l >= 3;l--){
      int j = i+1;
      int k = l-1;
      while(j < k && k < l){
	vector<int> tmp;
	if(nums[i] + nums[j] + nums[k] + nums[l] == target){
	  tmp.push_back(nums[i]);
	  tmp.push_back(nums[j]);
	  tmp.push_back(nums[k]);
	  tmp.push_back(nums[l]);
	  ans_vec.push_back(tmp);
	  j++;
	  k--;
	  while(j < k && k < l && nums[j] == nums[j-1]){
	    j++;
	  }
	  while(j < k && k < l && nums[k] == nums[k+1]){
	    k--;
	  }
	}
	else if(nums[i] + nums[j] + nums[k] + nums[l] < target){
	  j++;
	}
	else{
	  k--;
	}
      }
      while(l > 0 && nums[l-1] == nums[l]){
	l--;
      }
    }
    while(i < nums.size()-1 && nums[i] == nums[i+1]){
      i++;
    }
  }
  return ans_vec;
}
