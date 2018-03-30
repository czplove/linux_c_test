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

/**********************************************************************
* 函数名称： uint8 SmsDecodePdu( uint8 *p_src, uint8 *p_dst, uint8 *plen_dst ) 
* 功能描述： 对短信源码进行解析
* 输入参数： p_src 接收到的短信源码数据;p_dst 指向解析后待存数据地址
* 输出参数： 
* 返 回 值： 
* 其它说明： 
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2018/03/02	    V1.1.0	    zdj	          XXXX
***********************************************************************/
uint8 SmsDecodePdu( uint8 *p_src, uint8 *p_dst, uint8 *plen_dst )   
{
	uint8 tmp_len,need_decode_flag,dcs_flag;
	uint16 tmp_len_16,txt_len,tmp_index=0;   
	
	/// SMSC地址信息段    
	AsciiToHex(p_src,2,&tmp_len);    /// 取长度    
	if(tmp_len > 12)
	{   
		tmp_len = 0;
		goto RETURN_LAB;
	}
	
	if(tmp_len == 0)///无短信中心情况
	{
//-		MemCpy(sms_struct.sms_center_num,(uint8*)sys_const_para_struct.sms_city_center_num,StrLen((uint8*)sys_const_para_struct.sms_city_center_num,LEN_32));
		p_src += 2;
		tmp_index += 2;
	}
	else
	{
		tmp_len = (tmp_len - 1) << 1;						  /// SMSC号码串长度,去掉了91;    
		p_src += 4;              							  /// 指针后移,长度两个字节，91两个字节。共4个字节    
		tmp_index += 4; 
		SmsNumDecode(p_src,tmp_len,sms_struct.sms_center_num);    /// 转换SMSC号码到目标PDU串      
		p_src += tmp_len;        /// 指针后移,此时到了PDUType    
		tmp_index += tmp_len;   
	}
	/// TPDU段基本参数、回复地址等    
	///--PDUType     
	p_src += 2;        					/// 指针后移    
	tmp_index += 2;   
	///--OA-- 
	///包含回复地址，取回复地址信息    
	AsciiToHex(p_src,2,&tmp_len);    /// 取长度,OA的长度是指具体的号码长度
	if(tmp_len & 1) 
	{
		tmp_len += 1;         				/// 调整奇偶性
	}
	p_src += 4;          			  /// 指针后移，除去长度，和91,共4个字节    
	tmp_index += 4;   
	if((tmp_len < 2*2)||(tmp_len > 12*2))   
	{
		tmp_len = 0;
		goto RETURN_LAB;
	}	
	SmsNumDecode(p_src,tmp_len,sms_struct.sms_tx_num);
	p_src += tmp_len;        		 /// 指针后移    
	tmp_index += tmp_len;   

	/// TPDU段协议标识、编码方式、用户信息等    
	AsciiToHex(p_src,2,&sms_struct.pid);    /// 取协议标识(TP-PID)    
	p_src += 2;   
	tmp_index += 2;        
	AsciiToHex(p_src,2,&sms_struct.dcs);    /// 取编码方式(TP-DCS),这个解码时和回复时用    
	p_src += 2;       
	tmp_index += 2;   

	p_src += 14;       				/// 指针后移,时间戳14字节
	tmp_index+=14;   
	AsciiToHex(p_src,2,&tmp_len);    /// 用户信息长度(TP-UDL)    
	p_src += 2;         
	tmp_index += 2;   
	tmp_len_16 = tmp_len;
	
	need_decode_flag = FALSE;
	if((sms_struct.dcs & 0x0c) == GSM_7BIT)       
	{
		/// 7-bit解码    
		txt_len = (tmp_len_16 % 8)?(tmp_len_16 * 7 / 8 + 1):(tmp_len_16 * 7 / 8);   
		txt_len *= 2;   
		need_decode_flag = TRUE;
		dcs_flag = GSM_7BIT;   
	}   
	else if((sms_struct.dcs & 0x0c) == GSM_UCS2)   
	{
		/// UCS2解码    
		txt_len = tmp_len_16 * 2;   
		need_decode_flag = TRUE;
		dcs_flag = GSM_UCS2;
	}   
	else   
	{
		/// 8-bit解码 
		dcs_flag = GSM_8BIT; 
		txt_len=tmp_len_16 * 2;  
		need_decode_flag = TRUE;
	}
	
	tmp_index += txt_len;
	
	if(need_decode_flag)
	{
		tmp_len = SmsTextDecode(p_src,txt_len,p_dst,dcs_flag);
	}
	else
	{
		tmp_len = 0;
	}
	p_dst[tmp_len] = '\0';
	*plen_dst = tmp_len ;

RETURN_LAB: 
	return tmp_index;   
}


int main(int argc, char **argv)
{
    int i;

	j = 0;
    for (i=0; i<5; i++) {
        printf("HelloWorld!\n");
    }
    
    return 0;
}
