#include <string>
using namespace std;

class Solution {
public:
    string predictPartyVictory(string senate) {
        // 优先消灭下家对手，才会最容易取得胜利，就像玩英雄杀3V3,画地为牢一般都是先画下家，哈哈
        // 有权利的R
        int nR = 0; 
        // 有权利的DD
        int nD = 0;
        // 被预停止的R（表示向后面被停止，但是还没有被置为无权利的R）
        int nCloseR = 0;
        // 被预停止的D（表示向后面被停止，但是还没有被置为无权利的D）
        int nCloseD = 0;

        size_t i;
        for (i = 0; i < senate.length(); i++)
        { 
            if (senate[i] == 'R')
            {
                // 如果当前为R，当预停止R数大于0，表示该R已被停止。 被停止权利将字符置为0，同时，预停止的R数减一
                if (nCloseR > 0)
                {
                    nCloseR --;
                    senate[i] = '0';
                }
                // 如果当前为R，当预停止R数等于0，表示该R未被停止。 则有权利的R数加一，D预关闭数加一
                else
                {
                    nR ++;
                    nCloseD ++;
                }       
            }
            else if (senate[i] == 'D')
            {
                // 如果当前为D，当预停止D数大于0，表示该D已被停止。 被停止权利将字符置为0，同时，预停止的D数减一
                if (nCloseD > 0)
                {
                    nCloseD --;
                    senate[i] = '0';
                }
                // 如果当前为D，当预停止D数等于0，表示该D未被停止。 则有权利的D数加一，R预关闭数加一
                else
                {
                    nD ++;

                    nCloseR ++;
                }       
            }
            // 当遍历到最后，有权力的R和D的数量都大于0，则将他们都置为0,游标置到字符串初始位置，重新遍历一遍，直至某一个有权力数为0为止
            if (i == senate.length() - 1 && nR > 0 && nD > 0)
            {
                nR = 0;
                nD = 0;
                i = -1;
            }
        
        }

        if (nR > 0)
        {
            return "Radiant";
        }
        else if (nD > 0)
        {
            return "Dire";
        }
        
        return "";
    }
};

int main()
{
    Solution s;
    string ss("DRRDRDRDRDDRDRDR");
    printf("%s\n",s.predictPartyVictory(ss).c_str());

    return 0;
}


