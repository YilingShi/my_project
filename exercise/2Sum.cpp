vector<int> twoSum(vector<int>& nums, int target){
  vector<int> ans_vec;
  unordered_map<int,int> myMap;
  for(int i = 0 ;i < nums.size();i++){
    int rem = target - nums[i];
    if(myMap.find(nums[i]) != nums.end()){
      vec_ans.push_back(myMap[nums[i]]);
      vec_ans.push_back(i);
      return ans_vec;
    }
    myMap[rem] = i;
  }
}
