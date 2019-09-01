dload_http : http.o url.o client.o	#-helloword 就是我们要生成的目标,sample.o是生成此目标的先决条件
	$(CC) -pthread -o dload_http http.o url.o client.o -lcurl
http.o : http.c http.h
	$(CC) -g -c http.c
url.o : url.c
	$(CC) -g -c url.c
client.o : client.c
	$(CC) -g -c client.c


clean :
	rm dload_http *.o 


#gcc sample.c -o sample
