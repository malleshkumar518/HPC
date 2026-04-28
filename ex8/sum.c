#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main()
{
   printf("Sum of the two array\n");
   printf("\n");
   int i,j,r,c,n;
   printf("Enter the size of an array:");
   scanf("%d",&n);
   int a[n];
   int b[n];
   printf("Enter the array 1 elements:\n");
   for(i=0;i<n;i++)
   {
      scanf("%d",&a[i]);
   }
   printf("Enter the array 2 elements:\n");
   for (i=0;i<n;i++)
   {
      scanf("%d",&b[i]);
   }
   int res[n];
   #pragma omp parallel
      for(i=0;i<n;i++)
      {
	 res[i]=a[i]+b[i];
      }
   printf("Result Array:\n");
   for (i=0;i<n;i++)
   {
      printf("%d\t",res[i]);
      
   }
   printf("\n\n");
   return 0;
}

