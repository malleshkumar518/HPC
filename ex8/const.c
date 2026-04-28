#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main()
{
   printf("Add the constant to the array\n");
   printf("\n");
   int i,j,r,c,n;
   printf("Enter the size of an array:");
   scanf("%d",&n);
   int a[n];
   printf("Enter the array 1 elements:\n");
   for(i=0;i<n;i++)
   {
      scanf("%d",&a[i]);
   }
   int res[n];
   #pragma omp parallel
      for(i=0;i<n;i++)
      {
	 res[i]=a[i]+100;
      }
   printf("Result Array after add with constant(10):\n");
   for (i=0;i<n;i++)
   {
      printf("%d\t",res[i]);
      
   }
   printf("\n\n");
   return 0;
}

