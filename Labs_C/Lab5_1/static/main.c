#include <stdio.h>
#include <stdlib.h>

int main()
{
extern float f1(float a,float b);
extern float f2(float a,float b);

    printf("5.1+4.3=%.1f\n",f1(5.1,4.3));
    printf("5.1-4.4=%.1f\n",f2(5.1,4.3));

    return 0;
}
