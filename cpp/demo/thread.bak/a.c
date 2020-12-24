/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-25 00:56:46
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-25 00:58:10
 */
#include <stdio.h>
#include <list>
#include <string.h>
main()
{
    list<int> str;
    str.push_back(10);
    str.push_back(20);
    str.push_front(30);
    str.push_front(40);
    
    list<int>::iterator iter;
    for(iter = str.begin();iter!=str.end();iter++)
	    printf("%d\n",*iter);

    printf("num is %d\n",str.size());
}
