sample : sample.o #-helloword 就是我们要生成的目标,sample.o是生成此目标的先决条件
	gcc -o sample sample.o
sample.o : sample.c
	gcc -c sample.c

clean :
	rm sample sample.o


#gcc sample.c -o sample
