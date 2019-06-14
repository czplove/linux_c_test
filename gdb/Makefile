test : sample.o #-helloword 就是我们要生成的目标,sample.o是生成此目标的先决条件
	cc -o test sample.o
sample.o : sample.c
	cc -g -c sample.c

clean :
	rm test sample.o


#gcc sample.c -o sample
