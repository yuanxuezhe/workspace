#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    vector<bool> prefixesDivBy5(vector<int>& A) {
        vector<bool> vcBool;
        int sum = 0;
        for (size_t i = 0; i < A.size(); i ++)
        {
            sum = (sum % 10) * 2 + A[i];
            vcBool.push_back(sum % 5 == 0);
        }

        return vcBool;
    }
};

int main()
{
    Solution s;
    vector<int> ss = {1,1,0,0,0,1,0,0,1};

    s.prefixesDivBy5(ss);

    return 0;
}


