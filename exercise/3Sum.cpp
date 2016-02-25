()vector<vector<int>> threeSum(vector<int>& nums){
  vector<vector<int>> vec_ans;
  if(nums.size() <= 2){
    return vec_ans;
  }
  sort(nums.begin(),nums.end());
  for(int i = 0;i < nums.size()-2;i++){
    int j = i+1;
    int k = nums.size()-1;
    while(j < k){
      vector<int> tmp;
      if(nums[i] + nums[j] + nums[k] == 0){
	tmp.push_back(nums[i]);
	tmp.push_back(nums[j]);
	tmp.push_back(nums[k]);
	ans_vec.push_back(tmp);
	++j;
	--k;
	while(j < k && nums[j] == nums[j-1]){
	  ++j;
	}
	while(j < k && nums[k] == nums[k+1]){
	  --k;
	}
      }
      else if(nums[i] + nums[j] + nums[k] < 0){
	++j;
      }
      else{
	--k;
      }
      while(i < nums.size()-1 && nums[i] == nums[i+1]){
	++i;
      }
    }
  }
  return vec_ans;
}
