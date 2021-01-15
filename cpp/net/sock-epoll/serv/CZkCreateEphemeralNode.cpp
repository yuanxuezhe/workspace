/*
 * @Author: your name
 * @Date: 2020-12-27 21:49:38
 * @LastEditTime: 2020-12-27 21:59:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /workspace/cpp/net/sock-epoll/serv/CZkCreateEphemeralNode.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <string.h>
#include "CZkCreateEphemeralNode.h"
#include <iostream>
using namespace std;
 
void CZkCreateEphemeralNode::zktest_watcher_g(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
	printf("Something happened.\n");
	printf("type: %d\n", type);
	printf("state: %d\n", state);
	printf("path: %s\n", path);
	printf("watcherCtx: %s\n", (char*)watcherCtx);
}
 
void CZkCreateEphemeralNode::zktest_dump_stat(const struct Stat* stat)
{
	char tctimes[40];
	char tmtimes[40];
 
	time_t tctime;
	time_t tmtime;
 
	if (!stat)
	{
		fprintf(stderr, "null\n");
		return;
	}
 
	tctime = stat->ctime / 1000;
	tmtime = stat->mtime / 1000;
 
	ctime_r(&tmtime, tmtimes);
	ctime_r(&tctime, tctimes);
 
	fprintf(stderr, "\tctime = %s\tczxid=%llx\n"
		"\tmtime=%s\tmzxid=%llx\n"
		"\tversion=%x\taversion=%x\n"
		"\tephemearalOwner=%llx\n",
		tctimes, stat->czxid,
		tmtimes, stat->mzxid,
		(unsigned int)stat->version, (unsigned int)stat->aversion,
		stat->ephemeralOwner
	);
}
 
void CZkCreateEphemeralNode::zktest_stat_completion(int rc, const struct Stat* stat, const void* data)
{
	fprintf(stderr, "%s: rc=%d Stat:\n", (char*)data, rc);
	zktest_dump_stat(stat);
}
 
void CZkCreateEphemeralNode::zktest_void_completion(int rc, const void* data)
{
	fprintf(stderr, "[%s]: rc = %d\n", (char*)(data == 0 ? "null" : data), rc);
}
 
void CZkCreateEphemeralNode::zktest_string_completion(int rc, const char* name, const void* data)
{
	fprintf(stderr, "[%s]: rc = %d\n", (char*)(data == 0 ? "null" : data), rc);
	if (!rc)
	{
		fprintf(stderr, "\tname = %s\n", name);
	}
}
 
int CZkCreateEphemeralNode::zkOpen(zhandle_t** zkHandle, std::string strHost, int nTimeOut)
{
	if (strHost.length() < 0 || nTimeOut < 0)
	{
		fprintf(stderr, "Init failed.Param error.\n");
		return -1;
	}
 
	zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
	*zkHandle = zookeeper_init(strHost.c_str(), zktest_watcher_g, nTimeOut, 0, (void*)"hello zookeeper.", 0);
	if (*zkHandle == nullptr)
	{
		fprintf(stderr, "Error when connecting to zookeeper servers...\n");
		return -1;
	}
	return 0;
}
 
int CZkCreateEphemeralNode::zkCreate(zhandle_t* zkHandle, std::string strPath, std::string strValue)
{
	if ( strPath.length() <= 0 )
	{
		fprintf(stderr, "Create failed.Param error.\n");
		return -1;
	}
 
	cout << "zoo_acreate: path:" << strPath << ", value:" << strValue << endl;
	int nRet = zoo_acreate(zkHandle, strPath.c_str(), strValue.c_str(), strValue.length(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL|ZOO_SEQUENCE, zktest_string_completion, "acreate");
	if (nRet)
	{
		fprintf(stderr, "Error %d for %s\n", nRet, "acreate");
		exit(EXIT_FAILURE);
	}
	nRet = 0;
	sleep(1);
 
	nRet = zoo_aexists(zkHandle, strPath.c_str(), 1, zktest_stat_completion, "aexists");
	if (nRet)
	{
		fprintf(stderr, "Error %d for %s", nRet, "aexists");
		exit(EXIT_FAILURE);
	}
 
	return 0;
}
 
int CZkCreateEphemeralNode::zkClose(zhandle_t** zkHandle)
{
	if (*zkHandle)
	{
		zookeeper_close(*zkHandle);
		*zkHandle = nullptr;
	}
	return 0;
}
 
 
CZkCreateEphemeralNode::CZkCreateEphemeralNode(std::string strHost
	, int nTimeOut
	, std::string strPath
	, std::string strValue
	, std::string& strError
)
	: m_strHost(strHost)
	, m_nTimeOut(nTimeOut)
	, m_strPath(strPath)
	, m_strValue(strValue)
	, m_zkHandle(nullptr)
{
	strError = "";
	if (m_strHost.length() <= 0 || m_nTimeOut <= 0 || m_strPath.length() <= 0 || m_strValue.length() <= 0 )
	{
		strError = "Param error.";
	}
	int nRet = CZkCreateEphemeralNode::zkOpen(&m_zkHandle, m_strHost, nTimeOut);
	if ( nRet != 0 )
	{
		strError = "zkOpen fialed.";
	}
	else
	{
		nRet = CZkCreateEphemeralNode::zkCreate(m_zkHandle, m_strPath, m_strValue);
		if (nRet != 0)
		{
			strError = "zkCreate fialed.";
		}
	}
}
 
CZkCreateEphemeralNode::~CZkCreateEphemeralNode() 
{
	zkClose(&m_zkHandle);
}
 
CZkCreateEphemeralNode::CZkCreateEphemeralNode()
{
 
}
 
CZkCreateEphemeralNode::CZkCreateEphemeralNode(const CZkCreateEphemeralNode&) 
{
 
}