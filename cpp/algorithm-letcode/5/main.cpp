#include <string>
using namespace std;

class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.length();
        int nPosBegin = 0;
        int nLength = 0;
        for (size_t i = 0; i < s.length(); i++)
        {
            // aba型
            for (size_t j = 0; j <= min(n - i - 1, i); j++)
            {
                if (s[i + j] != s[i - j])
                {
                    if (nLength < 2*j - 1 )
                    {
                        nPosBegin = i - j + 1;
                        nLength = 2*j - 1;
                    }
                    break;
                } 
                else if (j == min(n - i - 1, i))
                {
                    if (nLength < 2*j + 1 )
                    {
                        nPosBegin = i - j;
                        nLength = 2*j + 1;
                    }
                }
                
            }
            // baab型
           for (size_t j = 1; j <= min(n - i - 1, i + 1); j++)
            {
                if (s[i + j] != s[i - j + 1])
                {
                    if (nLength < 2*j - 2 )
                    {
                        nPosBegin = i - j + 2;
                        nLength = 2*j - 2;
                    }
                    break;
                }
                else if (j == min(n - i - 1, i + 1))
                {
                    if (nLength < 2*j )
                    {
                        nPosBegin = i - j + 1;
                        nLength = 2*j;
                    }
                }
            } 
        }
        
        return s.substr(nPosBegin, nLength);
    }
};

int main()
{
    Solution s;
    string ss("babad");
    printf("%s\n",s.longestPalindrome(ss).c_str());

    return 0;
}


