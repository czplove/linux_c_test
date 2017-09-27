helloworld : hello.o #-helloword 就是我们要生成的目标,hello.o是生成此目标的先决条件
	gcc -o helloworld hello.o
hello.o : hello.c
	gcc -c hello.c

clean :
	rm helloworld hello.o


#gcc hello.c -o hello
