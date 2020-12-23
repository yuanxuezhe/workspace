/*
 * @Description: 
 * @Version: 1.0
 * @Autor: yuanshuai 446968454@qq.com
 * @Date: 2020-12-22 23:50:45
 * @LastEditors: yuanshuai 446968454@qq.com
 * @LastEditTime: 2020-12-23 21:36:29
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<zookeeper/zookeeper.h>
#include<zookeeper/zookeeper_log.h>

void zktest_watcher_g(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
    printf("Something happened.\n");
    printf("type: %d\n", type);
    printf("state: %d\n", state);
    printf("path: %s\n", path);
    printf("watcherCtx: %s\n", (char *)watcherCtx);
}
void zktest_dump_stat(const struct Stat *stat)
{
	char tctimes[40];
	char tmtimes[40];
	time_t tctime;
	time_t tmtime;
	if (!stat) {
        fprintf(stderr,"null\n");
        return;
    }
    tctime = stat->ctime/1000;
    tmtime = stat->mtime/1000;

    ctime_r(&tmtime, tmtimes);
    ctime_r(&tctime, tctimes);

    fprintf(stderr, "\tctime = %s\tczxid=%llx\n"
    "\tmtime=%s\tmzxid=%llx\n"
    "\tversion=%x\taversion=%x\n"
    "\tephemeralOwner = %llx\n",
    tctimes, stat->czxid,
    tmtimes, stat->mzxid,
    (unsigned int)stat->version, (unsigned int)stat->aversion,
    stat->ephemeralOwner);
}
//当 zoo_aexists 请求完成时会调用该函数
//rc参数为: ZOK 操作完成；ZNONODE 节点不存在；ZNOAUTH 客户端没有权限删除节点。
void zktest_stat_completion(int rc, const struct Stat *stat, const void *data)
{
    fprintf(stderr, "%s: rc = %d Stat:\n", (char*)data, rc);
    zktest_dump_stat(stat);
}
void zktest_void_completion(int rc, const void *data)
{
    fprintf(stderr, "[%s]: rc = %d\n", (char*)(data==0?"null":data), rc);
}
//当创建节点请求完成时会调用该函数
//rc 参数为: ZOK 操作完成；ZNONODE 父节点不存在；ZNODEEXISTS 节点已存在；ZNOAUTH 客户端没有权限创建节点。ZNOCHILDRENFOREPHEMERALS 临时节点不能创建子节点
//value 参数即新节点的路径名
//string_completion_t completion 中 const void *data 参数即为 zoo_acreate 中的 const void *data。
void zktest_string_completion(int rc, const char *name, const void *data)
{
    fprintf(stderr, "[%s]: rc = %d\n", (char*)(data==0?"null":data), rc);
    if (!rc) {
        fprintf(stderr, "\tname = %s\n", name);
    }
}

int main(int argc, const char *argv[])
{
    const char* host = "172.16.186.71:2191";
    //如果 30 秒内客户端没有连接上 Zookeeper 服务则表示连接超时。
    int timeout = 30000;
    //设置日志等级。
    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
//初始化zookeeper句柄（zhandle_t）
//第四个参数为客户端会话id,客户端尝试重连的先前会话的ID，如果不需要重连先前的会话，则设置为 0
//第五个参数为当前zk的上下文对象，最后一个参数是预留参数
    zhandle_t* zkhandle = zookeeper_init(host,
            zktest_watcher_g, timeout, 0, "hello zookeeper.", 0);
    if (zkhandle == NULL) {
        fprintf(stderr, "Error when connecting to zookeeper servers...\n");
        exit(EXIT_FAILURE);
    }
    // struct ACL ALL_ACL[] = {{ZOO_PERM_ALL, ZOO_ANYONE_ID_UNSAFE}};
    // struct ACL_vector ALL_PERMS = {1, ALL_ACL};
//创建一个znode节点，节点路径为"/xyz",保存一个长度为5，内容为hello的数据，任何人都可以访问
//这个一个异步函数，调用之后，直接往下执行，但是此时节点并没有真正的创建成功。节点创建成功之后会调用zktest_string_compeltion函数
//最后一个参数是该回调函数，要传入的参数
    int ret = zoo_acreate(zkhandle, "/xyz", "hello", 5,
            &ZOO_OPEN_ACL_UNSAFE, 0 /* ZOO_SEQUENCE */,
            zktest_string_completion, "acreate");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "acreate");
        exit(EXIT_FAILURE);
    }
    ret = 0;

//第三个参数是监视，如果非 0，则在服务器端设置监视，当节点发生变化时客户端会得到通知，即使当前指定的节点不存在也会设置监视，这样该节点被创建时，客户端也可以得到通知。
    ret = zoo_aexists(zkhandle, "/xyz", 1, zktest_stat_completion, "aexists");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "aexists");
        exit(EXIT_FAILURE);
    }
    ret = 0;
    // Wait for asynchronous zookeeper call done.
    getchar();
//第三个参数:期望的节点版本号，如果真实的版本号与期望的版本号不同则 zoo_delete() 调用失败，-1 表示不不检查版本号。
    ret = zoo_adelete(zkhandle, "/xyz", -1, zktest_void_completion, "adelete");
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "adelete");
        exit(EXIT_FAILURE);
    }
    // Wait for asynchronous zookeeper call done.
    getchar();
    zookeeper_close(zkhandle);
    return 0;
}