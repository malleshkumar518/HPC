#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main(){
	printf("============================ Sum of all elements divisible by K ============================\n");
	int n,k,sum=0;
	int i,j;
	int a[100][100];
	printf("enter the rows and columns size :");
	scanf("%d",&n);
	printf("enter the matrix elements :\n");
	for (i=0;i<n;i++){
		for(j=0;j<n;j++){
			scanf("%d",&a[i][j]);
		}
	}
	printf("enter the k value :");
	scanf("%d",&k);
	#pragma omp parallel for reduction(+:sum)
		for (i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(a[i][j]%k==0){
					sum+=a[i][j];
				}
			}
		}
	printf("sum of all elements divisible by %d in a matrix :%d\n",k,sum);
	return 0;
}
