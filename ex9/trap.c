#include <stdio.h>
#include <omp.h>
double f(double x)
{
    return x * x;
}
int main()
{
	double a, b, h, sum = 0.0, result;
	int n;
	printf("Enter a: ");
	scanf("%lf", &a);
	printf("Enter b: ");
	scanf("%lf", &b);
	printf("Enter number of trapezoids: ");
	scanf("%d", &n);
	h = (b - a) / n;
	sum = 0.0;
	#pragma omp parallel for reduction(+:sum)
	for (int i =1;i<n;i++) {
		sum+=f(a+i*h);
	}
	result=h*((f(a)+f(b))/2+sum);
	printf("\nTrapezoid using reduction: %.2lf\n",result);
	int sum_c=0.0;
#pragma omp parallel for
	for(int i=0;i<n;i++)
	{
		double t=f(a+i*h);
#pragma omp critical
		{
			sum_c+=t;
		}
	}
	double res=h*((f(a)+f(b))/2+sum_c);
	printf("\nTrapezoid using critical: %.2lf\n",res); 
	return 0;
}

