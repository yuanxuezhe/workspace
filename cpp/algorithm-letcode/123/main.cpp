#include <iostream>
#include <string>
#include <vector>
#include <map>
#include<algorithm>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        
        return prices[0];
    }
};

int main()
{
    Solution s;
    vector<int> ss = {3,3,5,0,0,3,1,4};
    printf("%d\n",s.maxProfit(ss));

    return 0;
}


