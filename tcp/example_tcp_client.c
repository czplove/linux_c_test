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
    struct sockaddr_in remote_addr; //服务器端网络地址结构体  

    if(argc!=2) 
    { 
        fprintf(stderr,"Usage:%s hostname \a\n",argv[0]); 
        exit(1); 
    } 

		memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零  
    remote_addr.sin_family=AF_INET; //设置为IP通信  
    remote_addr.sin_addr.s_addr=inet_addr("192.168.10.23");//服务器IP地址 
    remote_addr.sin_port=htons(9904); //服务器端口号  

		/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/  
    if((sockfd=socket(PF_INET,SOCK_STREAM,0))<0)  
    {  
        perror("server socket");  
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno)); 
        exit(1);  //-结束正在运行的整个程序
    }  
    
			/*将套接字绑定到服务器的网络地址上*/  
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
