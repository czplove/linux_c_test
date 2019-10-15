#include <stdio.h>

int myplus(int a,int b);
extern int mysub(int a,int b);
int main(int argc, char **argv)
{
    int i;

    for (i=0; i<5; i++) {
        printf("HelloWorld!\n");
    }
    
    int a,b;
	printf("input a and b:");
	scanf("%d%d",&a,&b);
	printf("%d + %d = %d\n",a,b,myplus(a,b));
	printf("%d - %d = %d\n",a,b,mysub(a,b));
    
    return 0;
}
