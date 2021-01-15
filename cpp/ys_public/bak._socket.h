
class CPacket
{
public:
	enum PacketType {
		PT_First					= (BYTE)0,
		PT_Command					= (BYTE)0,		// 普通命令 至于命令是什么 在包内容中给出 由各程序自行规定
		PT_HeartBeat				= (BYTE)1,		// 心跳
		PT_Broadcast				= (BYTE)2,		// 广播信息
		PT_Exit						= (BYTE)3,		// 客户端退出
		PT_UserInfo					= (BYTE)4,		// 用户信息
		PT_ForceClientExit			= (BYTE)5,		// 强制通知客户端退出
		PT_NewCaller				= (BYTE)6,		// 新来电
		PT_NewSmsRecv				= (BYTE)7,		// 接收新短信
		PT_NewSmsSend				= (BYTE)8,		// 发送新短信
		PT_GsmSignal				= (BYTE)9,		// 板卡各GSM通道的网络信号强度
		PT_CardStatus				= (BYTE)10,		// 板卡各GSM通道的状态
		PT_Last
	};

#pragma pack(push,1)
	typedef struct _PacketHead {
		BYTE	Header;		// 数据头		: 0x01
		DWORD	Check;		// 校验			: 不包括数据头和自身的Alder32校验
		BYTE	Type;		// 数据类型		: PacketType 类型
		BYTE	GUID[33];	// 包序号		: 使用生成的全球唯一的GUID作为每一个包的序号 GUID 32个字节即可，多一个字节是为了方便在用strcpy等函数拷贝时因为有'\0'结束符可以直接拷贝
		UINT	Length;		// 数据长度		: 包含PacketHead在内的所有数据的全部长度
		BYTE	Tail;		// 数据尾		: 0x02
	}PacketHead, *PPacketHead;
#pragma pack(pop)




void CSocketBroadcast::ThreadFunc()
{
	SOCKADDR_IN saClient;
	int fromlen = sizeof(SOCKADDR_IN);

	static char buf[BUFFER_SIZE] = { 0 };
	static int nDataLen = 0;

	int nRecvLen = 0;
	char * pHeader = NULL, * pHeader2 = NULL;

	while( this->m_socket != INVALID_SOCKET )
	{
		nRecvLen = recvfrom( this->m_socket, buf + nDataLen, BUFFER_SIZE - nDataLen, 0, (SOCKADDR*)&saClient, &fromlen );
		//Otherwise, a value of SOCKET_ERROR is returned, and a specific error 
		//code can be retrieved by calling WSAGetLastError.
		if( nRecvLen == SOCKET_ERROR )
		{
		}
		else
		{
			if( nRecvLen == 0 )//If the connection has been gracefully closed, the return value is zero.
			{
			} 
			else if( nRecvLen > 0 )//If no error occurs, recvfrom returns the number of bytes received.
			{
				nDataLen += nRecvLen;
				pHeader = buf; pHeader2 = pHeader;
				
				CPacket packet;
				while( ( pHeader = (char*)memchr( pHeader,0x01,nDataLen - ( pHeader - buf ) ) ) != NULL )
				{
					if( packet.FromBuffer( (PBYTE)pHeader,nDataLen - ( pHeader - buf ) ) != NULL )//是一个有效的 CPacket 包构成
					{
						// 是有效数据，开始处理数据
						HandleData( &packet,&saClient );
						// 处理完数据 将指针后移
						pHeader += packet.GetPacketLength();
						pHeader2 = pHeader;
					}
					else
						pHeader++;
				}
				// 所有的段都找完了
				if( pHeader2 == buf )// 一个段都没有找到
				{
					// 而且数据区还有多，说明数据有错误，存在很多杂乱无用的数据，将前面一半的数据清掉。
					if( nDataLen > (( BUFFER_SIZE * 2 ) / 3) )
					{
						int len = nDataLen;
						nDataLen /= 2;
						memcpy( buf,buf + ( len - nDataLen ),nDataLen );
						memset( buf + nDataLen,0,len - nDataLen );
					}
				}
				else// 找到段了
				{
					if( pHeader2 == buf + BUFFER_SIZE )// 刚好在找下一段时数据头移动到缓冲区的尾部的外面，已经是无效指针了
					{
						memset( buf,0,BUFFER_SIZE );
						nDataLen = 0;
					}
					else
					{
						nDataLen -= pHeader2 - buf;
						if( nDataLen < 0 ) nDataLen = 0;
						memcpy( buf,pHeader2,nDataLen );
						memset( buf + nDataLen,0,pHeader2 - buf );
					}
				}
			}
		}
	}
}
