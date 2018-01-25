#include <stdio.h>

/*
a label can only be part of a statement and a declaration is not a statement
上面编译出错,原因如下:
由于我在label之后进行变量的声明而导致的错误，label只能是语句的一部分，而变量的声明并不是一个语句。
对变量的声明移到label之前即可
从这个问题可以做一个引申，即在写代码的时候，变量的声明不应该出现在label之后，比如switch语句中的case结构也可能会遇到类似的问题。
 
PS:从一个#if...#endif宏块goto到宏块以外，会有一个编译告警。

特别注意:
在label之后增加{}可以消除编译错误,但是定义的变量后面不能正常赋值,所以不能使用这种方法,必须把定义放到label之前.

下面的例程实际输出如下:
i = 0;j = 5
i = 1;j = 5
i = 2;j = 5
i = 0;j = 5
i = 1;j = 5
i = 2;j = 5
i = 0;j = 5
i = 1;j = 5
i = 2;j = 5

但是我们理想的输出如下:
i = 0;j = 5
i = 1;j = 5
i = 2;j = 5
i = 0;j = 6
i = 1;j = 6
i = 2;j = 6
i = 0;j = 7
i = 1;j = 7
i = 2;j = 7
*/

int main(int argc, char **argv)
{
    int i,research=0;
    //-int j=5;
    
label:
    {
	    int j=5;
	    for (i=0; i<3; i++) {
	        printf("i = %d;j = %d\n", i, j);
	    }
	    
	    if(research < 2)
	    {
	    	research++;
	    	j++;	//-如果这个变量定义在label之后,这里对他的赋值是无效的,内部被转化为了NOP指令即什么也没有做
	        goto label;
	    }
    }
    
    return 0;
}
