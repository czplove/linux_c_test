iov_Log : iov_Log.o iotSemaphore.o atoi.o 	#-helloword 就是我们要生成的目标,sample.o是生成此目标的先决条件
	gcc -o iov_Log iov_Log.o iotSemaphore.o atoi.o
iov_Log.o : iov_Log.c iov_Log.h
	gcc -c iov_Log.c
iotSemaphore.o : iotSemaphore.c
	gcc -c iotSemaphore.c
atoi.o : atoi.c
	gcc -c atoi.c

clean :
	rm iov_Log iov_Log.o iotSemaphore.o atoi.o


#gcc sample.c -o sample
