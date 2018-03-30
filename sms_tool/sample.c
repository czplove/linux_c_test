#define SMS_GLOBAL

#include <stdio.h>
#include "sample.h"

/*
[14:07:26.263]收←◆+++
[14:07:26.793]收←◆
OK
AT+CMGL=0
[14:07:27.587]收←◆
+CMGL: 3,0,,70
0891683108200505F0240D91683185157052F7000081300331855523392A331DAE7BBDAC65747ACC2EEBAC65747ACC2E8F5400194CE68AC1642E580EE68AD96C3A59EC454D6A83112315EA12A7DD23

OK

*/

///**********************************************************************
//* 函数名称： uint8 SmsDecodePdu( uint8 *p_src, uint8 *p_dst, uint8 *plen_dst ) 
//* 功能描述： 对短信源码进行解析
//* 输入参数： p_src 接收到的短信源码数据;p_dst 指向解析后待存数据地址
//* 输出参数： 
//* 返 回 值： 
//* 其它说明： 
//* 修改日期        版本号     修改人	      修改内容
//* -----------------------------------------------
//* 2018/03/02	    V1.1.0	    zdj	          XXXX
//***********************************************************************/
//uint8 SmsDecodePdu( uint8 *p_src, uint8 *p_dst, uint8 *plen_dst )   
//{
//	uint8 tmp_len,need_decode_flag,dcs_flag;
//	uint16 tmp_len_16,txt_len,tmp_index=0;   
//	
//	/// SMSC地址信息段    
//	AsciiToHex(p_src,2,&tmp_len);    /// 取长度    
//	if(tmp_len > 12)
//	{   
//		tmp_len = 0;
//		goto RETURN_LAB;
//	}
//	
//	if(tmp_len == 0)///无短信中心情况
//	{
////-		MemCpy(sms_struct.sms_center_num,(uint8*)sys_const_para_struct.sms_city_center_num,StrLen((uint8*)sys_const_para_struct.sms_city_center_num,LEN_32));
//		p_src += 2;
//		tmp_index += 2;
//	}
//	else
//	{
//		tmp_len = (tmp_len - 1) << 1;						  /// SMSC号码串长度,去掉了91;    
//		p_src += 4;              							  /// 指针后移,长度两个字节，91两个字节。共4个字节    
//		tmp_index += 4; 
//		SmsNumDecode(p_src,tmp_len,sms_struct.sms_center_num);    /// 转换SMSC号码到目标PDU串      
//		p_src += tmp_len;        /// 指针后移,此时到了PDUType    
//		tmp_index += tmp_len;   
//	}
//	/// TPDU段基本参数、回复地址等    
//	///--PDUType     
//	p_src += 2;        					/// 指针后移    
//	tmp_index += 2;   
//	///--OA-- 
//	///包含回复地址，取回复地址信息    
//	AsciiToHex(p_src,2,&tmp_len);    /// 取长度,OA的长度是指具体的号码长度
//	if(tmp_len & 1) 
//	{
//		tmp_len += 1;         				/// 调整奇偶性
//	}
//	p_src += 4;          			  /// 指针后移，除去长度，和91,共4个字节    
//	tmp_index += 4;   
//	if((tmp_len < 2*2)||(tmp_len > 12*2))   
//	{
//		tmp_len = 0;
//		goto RETURN_LAB;
//	}	
//	SmsNumDecode(p_src,tmp_len,sms_struct.sms_tx_num);
//	p_src += tmp_len;        		 /// 指针后移    
//	tmp_index += tmp_len;   
//
//	/// TPDU段协议标识、编码方式、用户信息等    
//	AsciiToHex(p_src,2,&sms_struct.pid);    /// 取协议标识(TP-PID)    
//	p_src += 2;   
//	tmp_index += 2;        
//	AsciiToHex(p_src,2,&sms_struct.dcs);    /// 取编码方式(TP-DCS),这个解码时和回复时用    
//	p_src += 2;       
//	tmp_index += 2;   
//
//	p_src += 14;       				/// 指针后移,时间戳14字节
//	tmp_index+=14;   
//	AsciiToHex(p_src,2,&tmp_len);    /// 用户信息长度(TP-UDL)    
//	p_src += 2;         
//	tmp_index += 2;   
//	tmp_len_16 = tmp_len;
//	
//	need_decode_flag = FALSE;
//	if((sms_struct.dcs & 0x0c) == GSM_7BIT)       
//	{
//		/// 7-bit解码    
//		txt_len = (tmp_len_16 % 8)?(tmp_len_16 * 7 / 8 + 1):(tmp_len_16 * 7 / 8);   
//		txt_len *= 2;   
//		need_decode_flag = TRUE;
//		dcs_flag = GSM_7BIT;   
//	}   
//	else if((sms_struct.dcs & 0x0c) == GSM_UCS2)   
//	{
//		/// UCS2解码    
//		txt_len = tmp_len_16 * 2;   
//		need_decode_flag = TRUE;
//		dcs_flag = GSM_UCS2;
//	}   
//	else   
//	{
//		/// 8-bit解码 
//		dcs_flag = GSM_8BIT; 
//		txt_len=tmp_len_16 * 2;  
//		need_decode_flag = TRUE;
//	}
//	
//	tmp_index += txt_len;
//	
//	if(need_decode_flag)
//	{
//		tmp_len = SmsTextDecode(p_src,txt_len,p_dst,dcs_flag);
//	}
//	else
//	{
//		tmp_len = 0;
//	}
//	p_dst[tmp_len] = '\0';
//	*plen_dst = tmp_len ;
//
//RETURN_LAB: 
//	return tmp_index;   
//}

uint8 SmsDecode7bit(uint8 *p_src,uint8 src_len,uint8 *p_dst)   
{
	uint8 i,j,byte_num,left_num;  

	i = 0;   
	j = 0;   
	byte_num = 0;   
	left_num = 0;   

	while(i<src_len)   
	{
		*p_dst = ((*p_src << byte_num) | left_num) & 0x7f;    
		left_num = *p_src >> (7-byte_num);     
		p_dst++;   
		j++;      
		byte_num++;   

		if(byte_num == 7)   
		{   
			*p_dst = left_num;      
			p_dst++;   
			j++;   
			byte_num = 0;   
			left_num = 0;   
		}   
		p_src++;   
		i++;   
	}   
	*p_dst = 0;  
	
	return j;     
}

uint8 AsciiToHexVal(uint8 h_b,uint8 l_b)
{
	uint8 hex_val;
	
	if((h_b >= '0')&&(h_b <= '9'))
	{
		hex_val = h_b - '0';
	}
	else if((h_b >= 'A')&&(h_b <= 'F'))
	{
		hex_val = h_b - 'A' + 10;
	}
	else if((h_b >= 'a')&&(h_b <= 'f'))
	{
		hex_val = h_b - 'a' + 10;
	}
	
	hex_val <<= 4;
	
	if((l_b >= '0')&&(l_b <= '9'))
	{
		hex_val |= l_b - '0';
	}
	else if((l_b >= 'A')&&(l_b <= 'F'))
	{
		hex_val |= l_b - 'A' + 10;
	}
	else if((l_b >= 'a')&&(l_b <= 'f'))
	{
		hex_val |= l_b - 'a' + 10;
	}

	return hex_val;
}

uint16 AsciiToHex(uint8 *p_src,uint16 len,uint8 *p_dst)   
{   
    uint16 i,ret_len;

    ret_len = len >> 1;	
    for(i=0;i<ret_len;i++)   
    {   
      p_dst[i] = AsciiToHexVal(p_src[2*i],p_src[2*i+1]);
    }   
        
    return ret_len;       
}

uint8 StrLen(const uint8 *str,uint8 max_count)
{
	uint8 len;
	
	len = 0x00;
	while(*str != '\0')
	{
		str ++;
		len ++;
		if(max_count != 0)
		{
			if(len > max_count)
			{
				len = 0;
				break;
			}
		}
	}
	return len;
}

uint8 SmsEncode7bit(uint8 *p_src,uint16 src_len,uint8 *p_dst)   
{   
	uint8 i,j,byte_num,right_num;

	i = 0;   
	j = 0;   
	byte_num = 0;   
	right_num = 0;     

	while(i<src_len+1)   
	{    
		byte_num = i & 7;   

		if(byte_num == 0)   
		{     
			right_num = *p_src;   
		}   
		else   
		{       
			*p_dst = (*p_src << (8-byte_num)) | right_num;      
			right_num = *p_src >> byte_num;       
			p_dst++;   
			j++;    
		}  

		p_src++; 
		i++;   
	} 

	return j;        
}

uint16 HexToAscii(uint8 *p_src,uint16 len,uint8 *p_dst)   
{     
    uint8 tab[]="0123456789ABCDEF";    /// 0x0-0xf的字符查找表    
    uint16 i;
	
    for(i=0;i<len;i++)   
    {    
        *p_dst++ = tab[(*p_src) >> 4];      
        *p_dst++ = tab[(*p_src) & 0x0f];   
        p_src++;   
    }   
        
    return (len << 1);   
}

uint8 SmsTextEncode(uint8 *p_src,uint8 len,uint8 *p_dst,uint8 dcs_flag)
{
	uint16 tmp_len;
	uint8 tmp_buf[256];

	if(dcs_flag == GSM_7BIT)
	{
		tmp_buf[0] = len;
		tmp_len = SmsEncode7bit(p_src,len,tmp_buf+1);
	}
//	else if(dcs_flag == GSM_UCS2)
//	{
//		tmp_buf[0] = 2*len;
//		tmp_len = SmsEncodeUcs2(p_src,len,tmp_buf+1);     
//	}
//	else
//	{
//		tmp_buf[0] = len;
//		tmp_len = SmsEncode8bit(p_src,len,tmp_buf+1);  
//	}
	tmp_len = HexToAscii(tmp_buf,tmp_len+1,p_dst);
	p_dst[tmp_len] = '\0';
	return tmp_len;
}

uint8 SmsEncodePdu(uint8 *p_src,uint8 len,uint8 *p_dst,uint16 *dst_len)  
{ 
	uint8 *p_tmp = p_dst;
	uint8 dcs_flag;
	uint16 i;
	
	/// 7-bit编码方式 
	dcs_flag = GSM_7BIT;     
		
	i = SmsTextEncode(p_src,len,p_tmp,dcs_flag);
}

#if 0
int main(int argc, char **argv)
{
    int i;
    uint16 tmp_len;
	uint8 tmp_buf[256];
	//-uint8 *p_src="2A331DAE7BBDAC65747ACC2EEBAC65747ACC2E8F5400194CE68AC1642E580EE68AD96C3A59EC454D6A83112315EA12A7DD23";
	//-uint16 len=0x39;
	uint8 *p_src="2A331DAE7BBDAC65747ACC2EEBAC65747ACC2E8F5440194CE68AC1642E580EE68ADD723A59EC454D6A835F2674F8B5C65CB06F8C069BC56A2E71DA3D02";
	uint16 len=0x45;
	uint8 p_dst[256];

	/// 7-bit解码    
	len = (len % 8)?(len * 7 / 8 + 1):(len * 7 / 8);   
	len *= 2;   
	tmp_len = AsciiToHex(p_src, len, tmp_buf);
	
	tmp_len = SmsDecode7bit(tmp_buf,tmp_len,p_dst);
	printf("SMS Decode: \n%s\n",p_dst);
	
	for (i=0; i<tmp_len; i++) {
        printf("%c",p_dst[i]);
    }
	
    for (i=0; i<5; i++) {
        printf("\nHelloWorld!\n");
    }
    
    return 0;
}
#else
int main(int argc, char **argv)
{
    int i;
    uint16 tmp_len;
	uint8 tmp_buf[256];
	uint8 upgrade_success[] = "*ftp://Vehicle:Vehicle#*@202.102.090.166:21/TIZA_FTP.bin#";
	uint8 *p_in;
	uint8 tx_data[512];
	uint16 tx_len = 0;
	
	p_in = upgrade_success;
	/// 7-bit解码    
	tmp_len = SmsEncodePdu(p_in,StrLen(p_in,255),tx_data,&tx_len);
	//-str_len = sprintf(str_ch,"%d\r",tmp_len);
	
	printf("SMS Encode: \n%s\n",tx_data);
//	for (i=0; i<tmp_len; i++) {
//        printf("%c",p_dst[i]);
//    }
	
    for (i=0; i<5; i++) {
        printf("\nHelloWorld!\n");
    }
    
    return 0;
}
#endif