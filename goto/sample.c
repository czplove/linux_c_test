#include <stdio.h>

/*
a label can only be part of a statement and a declaration is not a statement
����������,ԭ������:
��������label֮����б��������������µĴ���labelֻ��������һ���֣�������������������һ����䡣
�Ա����������Ƶ�label֮ǰ����
��������������һ�����꣬����д�����ʱ�򣬱�����������Ӧ�ó�����label֮�󣬱���switch����е�case�ṹҲ���ܻ��������Ƶ����⡣
 
PS:��һ��#if...#endif���goto��������⣬����һ������澯��

�ر�ע��:
��label֮������{}���������������,���Ƕ���ı������治��������ֵ,���Բ���ʹ�����ַ���,����Ѷ���ŵ�label֮ǰ.

���������ʵ���������:
i = 0;j = 5
i = 1;j = 5
i = 2;j = 5
i = 0;j = 5
i = 1;j = 5
i = 2;j = 5
i = 0;j = 5
i = 1;j = 5
i = 2;j = 5

��������������������:
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
	    	j++;	//-����������������label֮��,��������ĸ�ֵ����Ч��,�ڲ���ת��Ϊ��NOPָ�ʲôҲû����
	        goto label;
	    }
    }
    
    return 0;
}
