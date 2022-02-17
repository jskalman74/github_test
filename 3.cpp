class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) 
    {
       unordered_map<int, int> mymap;
       vector<int> ans;
        
        for(int i=0; i<nums.size(); i++)
        {
            mymap[nums[i]] = i; 
        }
        
        for(int i=0; i<nums.size(); i++)
        {
            if(mymap.count(target-nums[i]) != 0 && i != mymap[target-nums[i]])
             return {i, mymap[target-nums[i]]};
        }
        
        
        return ans;
    }
};    
///gks kajlksjfldsajfas
