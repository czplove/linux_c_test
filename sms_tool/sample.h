
	typedef   unsigned        char uint8;
	typedef   unsigned short   int uint16;
	typedef   unsigned         int uint32;
//-	typedef   unsigned     __int64 uint64;

		#define LEN_12 12
		#define LEN_16 16
		#define LEN_20 20
		#define LEN_32 32
		#define LEN_64 64
		
		#define FALSE  0
		#define TRUE   1
		#define NULL 0

#ifdef SMS_GLOBAL
			#define SMS_EXTERN
#else
			#define SMS_EXTERN extern
#endif
			
			
#define  SMS_DataOutput						MDM_SMS_DataSend
			

		#define GSM_7BIT 0x00
		#define GSM_UCS2 0x08
		#define GSM_8BIT 0x04
		
		typedef struct 
		{
			uint8 sms_center_num[LEN_32];
			uint8 sms_tx_num[LEN_32];      ///发送方短信号
			uint8 pid;
			uint8 dcs;	
			uint8 change_sms_center_num_flag;
			
			uint8 RecvValidSMS;            // 收到有效短信标志(权限号码),  1：收到  0：初始
		}SMS_STRUCT;
		
		SMS_EXTERN SMS_STRUCT sms_struct;
		