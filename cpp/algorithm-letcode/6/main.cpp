#include <string>
using namespace std;

class Solution {
public:
    string convert(string s, int numRows) {
        int n;
        if ((n = s.length()) <= numRows || numRows == 1)
        {
            return s;
        }
        
        int diff = 2 * (numRows - 1);
        int pos;
        int diff1;
        string str("");
        
        for (size_t i = 1; i <= numRows; i++)
        {
            pos = i -1;
            diff1 = (i-1)*2;

            do
            {
                str += s[pos];
                    
                if ((diff1 = diff - diff1) == 0)
                {
                    diff1 = diff - diff1;
                } 
            } while ((pos = pos + diff1) < n);
        }
        return str;
    }
};

int main()
{
    Solution s;
    string ss("A");
    int n = 1;
    printf("%s\n",s.convert(ss, 4).c_str());

    return 0;
}


