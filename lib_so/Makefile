sample : shared_lib sample.o #-helloword 就是我们要生成的目标,sample.o是生成此目标的先决条件
	gcc -o sample sample.o -L. -lmylib
sample.o : sample.c
	gcc -c sample.c
	
shared_lib:
	gcc -c mylib.c -o mylib.o
	gcc -shared -fpic -o libmylib.so mylib.o

clean :
	rm sample sample.o


#gcc sample.c -o sample
