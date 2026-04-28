#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main()
{
   printf("----------------------------------------------------------------\nMirror Matix\n");
   printf("----------------------------------------------------------------\n");
   int a[3][3],res[3][3],i,j,r,c;
   printf("Enter the size:");
   scanf("%d",&r);
   for(i=0;i<r;i++)
   {
      for(j=0;j<r;j++){
	 scanf("%d",&a[i][j]);
      }
   }
   c=r-1;
   #pragma omp parallel
      for(i=0;i<r;i++)
      {
	 for(j=0;j<r;j++)
	 {
	    res[i][j]=a[i][c];
	    c=c-1;
	 }
	 c=r-1;
      }
   printf("Result:\n");
   for (i=0;i<r;i++)
   {
      for(j=0;j<r;j++)
      {
	 printf("%d\t",res[i][j]);
      }
      printf("\n");
   }
   return 0;
}
