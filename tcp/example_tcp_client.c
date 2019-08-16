#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <netinet/tcp.h>

#define portnumber 3333

#define NVRTR_DEBUG

#ifdef NVRTR_DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif /* DB_DEBUG */

///////////////////////////////////////////////////////////////////////////////
int heartbeat(int fd)
{
    int alive,error,idle,cnt,intv,ret;

    /*
     * open keepalive on fd
     */
Restart:
    alive = 1;//set keepalive open
    ret=setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,&alive,sizeof(alive));
    if(ret < 0)
    {
        DEBUG_PRINTF("set socket option error.\n");
        exit(1);
        goto Restart;
    }

    /*
     * 15S without data,send heartbeat package
     */
    idle = 15;
    ret = setsockopt(fd,SOL_TCP,TCP_KEEPIDLE,&idle,sizeof(idle));
    if(ret < 0)
    {
        DEBUG_PRINTF("set keepalive idle error.\n");
        exit(1);
        return -1;
    }

    /*
     * without any respond,10S later resend package
     */
    intv = 10;
    ret = setsockopt(fd,SOL_TCP,TCP_KEEPINTVL,&intv,sizeof(intv));
    if(ret < 0)
    {
        DEBUG_PRINTF("set keepalive intv error.\n");
        exit(1);
        return -2;
    }

    /*
     * send 3 times,without any response,mean connect lose 
     */
    cnt = 3;
    ret = setsockopt(fd,SOL_TCP,TCP_KEEPCNT,&cnt,sizeof(cnt));
    if(ret < 0)
    {
        DEBUG_PRINTF("set keepalive cnt error.\n");
        exit(1);
        return -3;
    }
}

/* check whether socket is connected. Actually, it is not accurate if the client 
 * crashs and the server don't know about it because the socket buffer is ready.
 * So the return value '0' doesn't mean the socket is really valid. Of course,
 * '-1' shows the socket is invalid 
*/
int checksock(int s)
{
        fd_set   fds;
        char buf[2];
        int nbread;
        FD_ZERO(&fds);
        FD_SET(s,&fds);
        if ( select(s+1, &fds, (fd_set *)0, (fd_set *)0, NULL) == -1 ) {
                //log(LOG_ERR,"select(): %s\n",strerror(errno)) ;
                return -1;
        }
        if (!FD_ISSET(s,&fds)) {
                //log(LOG_ERR,"select() returns OK but FD_ISSET not\n") ;
                return -1;
        }       
        /* read one byte from socket */
        nbread = recv(s, buf, 1, MSG_PEEK);
        if (nbread <= 0)
                return -1;
        return 0;
}

int SocketConnected(int sock)
{
	   if(sock<=0) return 0;
     struct tcp_info info;
     int len=sizeof(info);
     getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
     if((info.tcpi_state==1)||(info.tcpi_state==8))  //info.tcpi_state==TCP_ESTABLISHED
     {
         return 1;
     }
     else
     {
         printf("tcpi_state %d\n",info.tcpi_state);
         if((info.tcpi_state==7)) return 0;
         return 1;
     }
}

///////////////////////////////////////////////////////////////////////////////

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
    remote_addr.sin_addr.s_addr=inet_addr("218.94.153.146");//服务器IP地址 218.94.153.146 192.168.10.23
    remote_addr.sin_port=htons(9903); //服务器端口号  

		/*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/  
    if((sockfd=socket(PF_INET,SOCK_STREAM,0))<0)  
    {  
        perror("server socket");  
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno)); 
        exit(1);  //-结束正在运行的整个程序
    }  
    
    heartbeat(sockfd);
    
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

		int len;
    len = read( sockfd, buffer, strlen(buffer) ); 
//    len = recv(sockfd,buffer,strlen(buffer),0);
    if ( len > 0 ) {
        buffer[len] = '\0';
        printf( "SR (%d): %s\n", len, buffer );

    } else {
        printf( "Socket reads %d bytes\n", len );
    }

    close(sockfd); 
    exit(0); 
} 
