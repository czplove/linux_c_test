#define SMS_GLOBAL

#include <stdio.h>
#include "sample.h"

/*
[14:07:26.263]�ա���+++
[14:07:26.793]�ա���
OK
AT+CMGL=0
[14:07:27.587]�ա���
+CMGL: 3,0,,70
0891683108200505F0240D91683185157052F7000081300331855523392A331DAE7BBDAC65747ACC2EEBAC65747ACC2E8F5400194CE68AC1642E580EE68AD96C3A59EC454D6A83112315EA12A7DD23

OK

*/

/**********************************************************************
* �������ƣ� uint8 SmsDecodePdu( uint8 *p_src, uint8 *p_dst, uint8 *plen_dst ) 
* ���������� �Զ���Դ����н���
* ��������� p_src ���յ��Ķ���Դ������;p_dst ָ�������������ݵ�ַ
* ��������� 
* �� �� ֵ�� 
* ����˵���� 
* �޸�����        �汾��     �޸���	      �޸�����
* -----------------------------------------------
* 2018/03/02	    V1.1.0	    zdj	          XXXX
***********************************************************************/
uint8 SmsDecodePdu( uint8 *p_src, uint8 *p_dst, uint8 *plen_dst )   
{
	uint8 tmp_len,need_decode_flag,dcs_flag;
	uint16 tmp_len_16,txt_len,tmp_index=0;   
	
	/// SMSC��ַ��Ϣ��    
	AsciiToHex(p_src,2,&tmp_len);    /// ȡ����    
	if(tmp_len > 12)
	{   
		tmp_len = 0;
		goto RETURN_LAB;
	}
	
	if(tmp_len == 0)///�޶����������
	{
//-		MemCpy(sms_struct.sms_center_num,(uint8*)sys_const_para_struct.sms_city_center_num,StrLen((uint8*)sys_const_para_struct.sms_city_center_num,LEN_32));
		p_src += 2;
		tmp_index += 2;
	}
	else
	{
		tmp_len = (tmp_len - 1) << 1;						  /// SMSC���봮����,ȥ����91;    
		p_src += 4;              							  /// ָ�����,���������ֽڣ�91�����ֽڡ���4���ֽ�    
		tmp_index += 4; 
		SmsNumDecode(p_src,tmp_len,sms_struct.sms_center_num);    /// ת��SMSC���뵽Ŀ��PDU��      
		p_src += tmp_len;        /// ָ�����,��ʱ����PDUType    
		tmp_index += tmp_len;   
	}
	/// TPDU�λ����������ظ���ַ��    
	///--PDUType     
	p_src += 2;        					/// ָ�����    
	tmp_index += 2;   
	///--OA-- 
	///�����ظ���ַ��ȡ�ظ���ַ��Ϣ    
	AsciiToHex(p_src,2,&tmp_len);    /// ȡ����,OA�ĳ�����ָ����ĺ��볤��
	if(tmp_len & 1) 
	{
		tmp_len += 1;         				/// ������ż��
	}
	p_src += 4;          			  /// ָ����ƣ���ȥ���ȣ���91,��4���ֽ�    
	tmp_index += 4;   
	if((tmp_len < 2*2)||(tmp_len > 12*2))   
	{
		tmp_len = 0;
		goto RETURN_LAB;
	}	
	SmsNumDecode(p_src,tmp_len,sms_struct.sms_tx_num);
	p_src += tmp_len;        		 /// ָ�����    
	tmp_index += tmp_len;   

	/// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��    
	AsciiToHex(p_src,2,&sms_struct.pid);    /// ȡЭ���ʶ(TP-PID)    
	p_src += 2;   
	tmp_index += 2;        
	AsciiToHex(p_src,2,&sms_struct.dcs);    /// ȡ���뷽ʽ(TP-DCS),�������ʱ�ͻظ�ʱ��    
	p_src += 2;       
	tmp_index += 2;   

	p_src += 14;       				/// ָ�����,ʱ���14�ֽ�
	tmp_index+=14;   
	AsciiToHex(p_src,2,&tmp_len);    /// �û���Ϣ����(TP-UDL)    
	p_src += 2;         
	tmp_index += 2;   
	tmp_len_16 = tmp_len;
	
	need_decode_flag = FALSE;
	if((sms_struct.dcs & 0x0c) == GSM_7BIT)       
	{
		/// 7-bit����    
		txt_len = (tmp_len_16 % 8)?(tmp_len_16 * 7 / 8 + 1):(tmp_len_16 * 7 / 8);   
		txt_len *= 2;   
		need_decode_flag = TRUE;
		dcs_flag = GSM_7BIT;   
	}   
	else if((sms_struct.dcs & 0x0c) == GSM_UCS2)   
	{
		/// UCS2����    
		txt_len = tmp_len_16 * 2;   
		need_decode_flag = TRUE;
		dcs_flag = GSM_UCS2;
	}   
	else   
	{
		/// 8-bit���� 
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
