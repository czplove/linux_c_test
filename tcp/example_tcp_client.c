#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 

#define portnumber 3333

int main(int argc, char *argv[]) 
{ 
    int sockfd; 
    char buffer[1024]; 
    struct sockaddr_in server_addr; 
    struct hostent *host; 
    struct sockaddr_in remote_addr; //�������������ַ�ṹ��  

    if(argc!=2) 
    { 
        fprintf(stderr,"Usage:%s hostname \a\n",argv[0]); 
        exit(1); 
    } 

		memset(&remote_addr,0,sizeof(remote_addr)); //���ݳ�ʼ��--����  
    remote_addr.sin_family=AF_INET; //����ΪIPͨ��  
    remote_addr.sin_addr.s_addr=inet_addr("192.168.10.23");//������IP��ַ 
    remote_addr.sin_port=htons(9904); //�������˿ں�  

		/*�����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��*/  
    if((sockfd=socket(PF_INET,SOCK_STREAM,0))<0)  
    {  
        perror("server socket");  
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno)); 
        exit(1);  //-�����������е���������
    }  
    
			/*���׽��ְ󶨵��������������ַ��*/  
	    if((connect(sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr)))<0)
	    {  
	        perror("server connect");  
	        fprintf(stderr,"Connect Error:%s\a\n",strerror(errno)); 
	        exit(1); 
	    }

    printf("Please input char:\n");
    

    fgets(buffer,1024,stdin); 
    write(sockfd,buffer,strlen(buffer)); 


    close(sockfd); 
    exit(0); 
} 
