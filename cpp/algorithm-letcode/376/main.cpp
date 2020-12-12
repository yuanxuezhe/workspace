#include <string>
#include <vector>
using namespace std;

class Solution {
public:
    int wiggleMaxLength(vector<int>& nums) {
        // 最长串满足，右侧大数，一定为几个连续递增数中最大的那个；右侧小数，一定为几个连续递减数中最小的那个
        char flag = -1; // 初始化为 -1； flag 为 0 表示取右侧大数； flag为1 表示取右侧小数
        int nCount = 0;
        int n = nums.size();
        if (n  <= 1)
        {
            return n;
        }
        for (size_t i = 0; i < n-1; i++)
        {
            // 右侧为递减序列时
            if (nums[i] > nums[i+1])
            {
                // 初始状态，flag置为1取右侧小数
                if (flag == -1)
                {
                    flag = 1;
                    nCount ++;
                }
                else if (flag == 0)
                {
                    nCount ++;
                    flag = 1;
                }  
            }
            // 右侧为递增序列时
            else if (nums[i] < nums[i+1])
            {
                // 初始状态，flag置为1取右侧大数
                if (flag == -1)
                {
                    flag = 0;
                    nCount ++;
                }
                else if (flag == 1)
                {
                    nCount ++;
                    flag = 0;
                }  
            }  
        }
        return nCount + 1;
    }
};

int main()
{
    Solution s;
    //vector <int> v = {1,7,4,9,2,5};
    vector <int> v = {1,2,3,4,5,6,7,8,9};

    //vector <int> v = {1,17,5,10,13,15,10,5,16,8};
    printf("%d\n",s.wiggleMaxLength(v));

    return 0;
}


