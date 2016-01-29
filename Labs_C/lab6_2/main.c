#include <stdio.h>
#include <stdlib.h>

#define n 5
#define m 4

int main()
{
    srand( time( 0 ) );
    int buf[n];
    int matrix[m][n];
    int result[m];

    int i,j;
    printf("M2[%d]:\n",n);
    for(i=0;i<n;i++){
        buf[i] = rand()%8+1;
        printf("%d ",buf[i]);
        }

    printf("\n\nM1[%d][%d]:\n",m,n);

    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            matrix[i][j] = rand()%8+1;
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }

    int tmp;
    for(i=0;i<m;i++){
        result[i] = 0;
        for(j=0;j<n;j++)
            result[i]+=matrix[i][j] * buf[i];
    }

    printf("\nM1[%d][%d] X M2[%d]:\n",m,n,n);

    for(i=0;i<m;i++)
        printf("%d\n",result[i]);

    return 0;
}
