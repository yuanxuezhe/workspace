#include <stdio.h>
#include <unistd.h>

int packtlv_led(char *buf, int size, int cmd)
{
    unsigned short  crc16;
    int pack_len = TLV_FIXED_SIZE+1; /* Only 1 byte data */
 
    if(!buf || size<TLV_MIN_SIZE )
    {
        printf("Invalid input arguments\n");
        return 0;
    }
 
    /* Packet head */
    buf[0] = PACK_HEADER;
 
    /* Tag */
    buf[1] = TAG_LED;
 
    /* Length, this packet total 6 bytes */
    buf[2] = pack_len;
 
    /* Value */
    buf[3] = (OFF==cmd) ? 0x00 : 0x01;

    /* Calc CRC16 checksum value from Packet Head(buf[0])~ Packet Value(buf[3]) */
    crc16 = crc_itu_t(MAGIC_CRC, buf, 4); //HTLV计算出一个unsigned short类型crc的值

    /* Append the 2 Bytes CRC16 checksum value into the last two bytes in packet buffer */ 
    ushort_to_bytes(&buf[4], crc16);//将16位的crc值(1个字节8个位)转换成两个字节加到HTLV最后变成HTLV CRC的tlv报文
 
    return pack_len;
}

int unpack_tlv(char *buf, int bytes, char *true_tlv,int *true_size)
{
    int     i ;
    char    *tlv_ptr = NULL;
    int     tlv_len = 0 ;
    unsigned short      crc16,val ;

    if(!buf||!true_tlv)
    {
        printf("Invailed input!\n") ;
        return 0 ;
    }
 
again:
 
    if(bytes < TLV_MIN_SIZE) //数据长度太短
    {
        printf("tlv packet too short\n") ;
        return bytes ;
    }
 
    for(i=0; i< bytes; i++)
    {
        if((unsigned char)buf[i]==PACK_HEADER)
        {
            if(bytes -i <= 2)//剩下的数据不能读到tlv的长度
            {
                printf("tlv_packet is too short so incomplete\n") ;
                memmove(buf, &buf[i], bytes - i) ;
                return bytes -i ;
            }
 
            tlv_ptr = &buf[i] ;
            tlv_len = tlv_ptr[2] ;
            if(tlv_len < TLV_MIN_SIZE||tlv_len>TLV_MAX_SIZE )//tlv长度不合理说明是错误的一帧
            {
                memmove(buf, &tlv_ptr[2], bytes-i-2) ;//将tlv长度后面的移到数据最前面
                bytes = bytes -i -2 ;
                goto again;
            }
            if(tlv_len > bytes - i)//如果tlv长度比剩下的数据长度还长
            {
                memmove(buf, tlv_ptr, bytes-i) ;
                printf("tlv_packet is incomplete\n") ;
                return bytes - i ;
            }
            crc16 = crc_itu_t(MAGIC_CRC,(unsigned char*)tlv_ptr, tlv_len-2);
            val = bytes_to_ushort((unsigned char*)&tlv_ptr[tlv_len-2],2) ;

            //printf("crc: %d, val: %d\n",crc16, val) ;

            if(val != crc16)//crc校验错误
            {
                printf("CRC check error\n") ;
                memmove(buf, &tlv_ptr[tlv_len], bytes-i-tlv_len) ;
                bytes = bytes -i -tlv_len ;
                goto again ;
            }

            //printf("CRC check true\n") ;

            memmove(&true_tlv[*true_size], tlv_ptr, tlv_len) ;
            *true_size += tlv_len ;
            memmove(buf,&tlv_ptr[tlv_len], bytes-i-tlv_len) ;//处理完正确的一帧之后将其移出数组

            bytes = bytes -i -tlv_len ;
            goto again ;

            }//if((unsigned char)buf[i] == PACK_HEADER)
        }//for(i=0;i<bytes;i++)

        return 0 ;
}