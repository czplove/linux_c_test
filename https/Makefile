example_https : example_https_get.o
	$(CC) -o example_https example_https_get.o -lcurl

example_https_get.o : example_https_get.c
	$(CC) -c example_https_get.c

#Thread.o : Thread.c
#	$(CC) -c Thread.c

clean :
	rm example_https example_https_get.o


#gcc sample.c -o sample
