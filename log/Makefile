iov_Log : iov_Log.o iotSemaphore.o atoi.o Thread.o 	#-helloword 就是我们要生成的目标,sample.o是生成此目标的先决条件
	$(CC) -pthread -o iov_Log iov_Log.o iotSemaphore.o atoi.o Thread.o
iov_Log.o : iov_Log.c iov_Log.h
	$(CC) -c iov_Log.c
iotSemaphore.o : iotSemaphore.c
	$(CC) -c iotSemaphore.c
atoi.o : atoi.c
	$(CC) -c atoi.c
Thread.o : Thread.c
	$(CC) -c Thread.c

clean :
	rm iov_Log iov_Log.o iotSemaphore.o atoi.o


#gcc sample.c -o sample
