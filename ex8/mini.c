#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main()
{
   printf("----------------------------------------------------------------\nRunning minimum in the array\n");
   printf("----------------------------------------------------------------\n");
   int i,j,r,c,n;
   printf("Enter the size of an array:");
   scanf("%d",&n);
   int a[n];
   int min;
   for(i=0;i<n;i++)
   {
      scanf("%d",&a[i]);
   }
   min=a[0];
   int res[n];
   res[0]=a[0];
   #pragma omp parallel
      for(i=0;i<n;i++)
      {
	 if(a[i]<min)
	 {
	    min=a[i];
	 }
	 res[i]=min;
      }
   printf("Result Array:\n");
   for (i=0;i<n;i++)
   {
      printf("%d\t",res[i]);
      
   }
   printf("\n-----------------------------------------------------------------\n");
   return 0;
}

