/*
 * @Author: your name
 * @Date: 2020-12-27 21:50:19
 * @LastEditTime: 2020-12-27 21:55:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /workspace/cpp/demo/zookeeper1/main.cpp
 */
#include "CZkCreateEphemeralNode.h"
#include <string.h>
#include <iostream>
using namespace std;
 
int main() 
{
	/************************************************************************/
	/* 二种方式：1、new的方式创建CZkCreateEphemeralNode                     */
	/*			 2、定义全局变量CZkCreateEphemeralNode                     */
	/************************************************************************/
	std::string strError;
	CZkCreateEphemeralNode* pZk = new CZkCreateEphemeralNode("127.0.0.1:2181", 3000, "/servers/test_client", "test", strError);
	if ( strError.length() > 0 )
	{
		wcout << L"CZkCreateEphemeralNode error." << endl;
	}
 
	//g_Zk变量必须在程序的整个生命周期（建议使用全局变量）
	CZkCreateEphemeralNode g_Zk("127.0.0.1:2181", 3000, "/servers/test_client", "test", strError);
	if (strError.length() > 0)
	{
		wcout << L"CZkCreateEphemeralNode error." << endl;
	}
	char ch;
	cin >> ch;
 
	return 0;
}