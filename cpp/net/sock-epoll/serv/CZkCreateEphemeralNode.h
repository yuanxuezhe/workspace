/*
 * @Author: your name
 * @Date: 2020-12-27 21:49:21
 * @LastEditTime: 2020-12-27 21:49:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /workspace/cpp/net/sock-epoll/serv/CZkCreateEphemeralNode.h
 */
/************************************************************************/
/* zookeeper Create Ephemeral Node                                      */
/************************************************************************/
 
#ifndef __CZKCREATETEMPORARYNODE_H__
#define __CZKCREATETEMPORARYNODE_H__
#include <string>
#include "zookeeper.h"
#include "zookeeper_log.h"
 
class CZkCreateEphemeralNode
{
public:
	CZkCreateEphemeralNode(std::string m_strHost, int m_nTimeOut, std::string strPath, std::string strValue, std::string& strError);
	~CZkCreateEphemeralNode();
 
private:
	CZkCreateEphemeralNode();
	CZkCreateEphemeralNode(const CZkCreateEphemeralNode&);
 
private:
	static void zktest_watcher_g(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx);
	static void zktest_dump_stat(const struct Stat* stat);
	static void zktest_stat_completion(int rc, const struct Stat* stat, const void* data);
	static void zktest_void_completion(int rc, const void* data);
	static void zktest_string_completion(int rc, const char* name, const void* data);
 
public:
	static int zkOpen(zhandle_t** zkHandle, std::string strHost, int nTimeOut);
	static int zkCreate(zhandle_t* zkHandle, std::string strPath, std::string strValue);
	static int zkClose(zhandle_t** zkHandle);
	//static int zkOpen(std::string strHost, int nTimeOut);
	//static int zkCreate(std::string strPath, std::string strValue);
	//static int zkClose();
 
private:
	std::string m_strHost;
	int m_nTimeOut;
	std::string m_strPath;
	std::string m_strValue;
	zhandle_t* m_zkHandle;
};
 
#endif
 