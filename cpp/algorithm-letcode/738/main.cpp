#include <string>
#include <cmath>
using namespace std;

class Solution {
public:
    int monotoneIncreasingDigits(int N) {
        // 只需要找到从最高位为递增的数，后面的不满足递增的置为0，将这个数-1。
        int m = N;
        // 记录不是递增开始的位置
        int pos = 0;
        // 记录当前位置
        int count = 0;
        int lastNum = -1;
        while (m) {
            if (lastNum < m % 10)
            {
                pos = count;
                lastNum = m % 10 - 1;
            }
            else
            {
                lastNum = m % 10;
            }
            m /= 10;
            count ++;
        }

        int x = 1;
        for (size_t i = 0; i < pos; i++)
        {
            x *= 10;
        }
        
        return (N / x) * x - (pos > 0 ? 1 : 0);
    }
};

int main()
{
    Solution s;
    int n = 668841;
    printf("%d\n",s.monotoneIncreasingDigits(n));

    return 0;
}


