#include <omp.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
using namespace std;
void init(int* a, int* b, int N)
{
	srand(time(0));
	cout << "=====Matrix init: N =" << N << " =====" << endl;
	for (int i = 0; i < N; i++)
	{
		a[i] = rand() % 2 + 1;
		b[i] = rand() % 2 + 1;
	}
}
void linearCalc(int* a, int * b, int N)
{
	 int total = 1;
	for (int i = 0; i < N; i++)
	{
		int sp = a[i] + b[i];
		if (sp != 0)
			total *= sp;
	}
}
void parallelCalcA(int* a, int * b, int N)
{
	int  sp, i;
	int total = 1;
#pragma omp parallel shared(a,b,total)
	{
#pragma omp for private(i,sp)
		for (i = 0; i < N; i++)
		{
			sp = a[i] + b[i];
			if (sp != 0)
			{
#pragma omp atomic
				total *= sp;
			}
		}
	}
}
void parallelCalcB(int* a, int * b, int N)
{
	int  sp, i;
	int total = 1;
#pragma omp parallel shared(a,b,total)
	{
#pragma omp for private(i,sp)
		for (i = 0; i < N; i++)
		{
			sp = a[i] + b[i];
#pragma omp critical(update)
			{
				if (sp != 0)
				{
					total *= sp;
				}
			}
		}
	}
}
void parallelRCalc(int* a, int * b, int N)
{
	int  sp, i, total ;
	total = 1;
#pragma omp parallel shared(a,b) 
	{
#pragma omp for private(i,sp) reduction(*:total)
		for (i = 0; i < N; i++)
		{
			sp = a[i] + b[i];
			if (sp != 0)
				total *= sp;
		}
	}
}

void parallelLCalc(int* a, int * b, int N)
{
	omp_lock_t lock;
	omp_init_lock(&lock);
	int  sp, i, total = 1;
#pragma omp parallel shared(a,b) 
	{
#pragma omp for private(i,sp)
		for (i = 0; i < N; i++)
		{
			sp = a[i] + b[i];
			if (sp != 0)
			{
				omp_set_lock(&lock);
				total *= sp;
				omp_unset_lock(&lock);
			}
		}
	}
	omp_destroy_lock(&lock);
}
void parallelBCalc(int* a, int * b, int N)
{
	int  sp, i, total =0;
#pragma omp parallel shared(a,b) 
	{
#pragma omp master
		{
			total = 1;
			//cout << "Мастер поток" << endl;
		}
#pragma omp barrier
#pragma omp for private(i,sp) reduction(*:total)
		for (i = 0; i < N; i++)
		{
			sp = a[i] + b[i];
			if (sp != 0)
				total *= sp;
		}
	}
}
void parallelCalc(int* a, int * b, int N,int n_sect)
{
	int  sp, i;
	int total = 1;
	if(n_sect ==2)
	{
#pragma omp parallel sections reduction(*:total) private(sp)
		{
#pragma omp section
			{
				for (int i = 0; i < N / 2; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i =N/2; i < N; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
		}
	}
	else if (n_sect == 4)
	{
#pragma omp parallel sections reduction(*:total) private(sp)
		{
#pragma omp section
			{
				for (int i = 0; i < N / 4; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = N / 4; i < N/2; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = N / 2; i < (N/4)*3; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = (N / 4) * 3; i < N; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
		}
	}
	else
	{
#pragma omp parallel sections reduction(*:total) private(sp)
		{
#pragma omp section
			{
				for (int i = 0; i < N / 8; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = N / 8; i < N / 4; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = N / 4; i < (N / 8) * 3; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = (N / 8) * 3; i < N/2; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = N/2; i < (N/8)*5; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = (N/8)*5; i < (N / 4)*3; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = (N/4)*3; i < (N / 8) * 7; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
#pragma omp section
			{
				for (int i = (N / 8) * 7; i < N; i++)
				{
					sp = a[i] + b[i];
					if (sp != 0)
					{
						total *= sp;
					}
				}
			}
		}
	}
}
int main()
{
	setlocale(LC_ALL, "ru");
	int N[6] = { 500000, 600000,700000,800000,10000000 };
	int N_s[3] = { 2,4,8 };
	for (int k = 0; k < 5; k++)
	{
		int* a = new int[N[k]];
		int* b = new int[N[k]];
		init(a, b, N[k]);
		double start, end, start2, end2, start3, end3;
		start = omp_get_wtime();
		linearCalc(a, b, N[k]);
		end = omp_get_wtime() - start;
		//cout << "Time linear: "  << end << endl;
		//cout << endl;
		start = omp_get_wtime();
		parallelRCalc(a, b, N[k]);
		end = omp_get_wtime() - start;
		//cout << "Time reduction: " << end << endl;
		//cout << endl;
		start3 = omp_get_wtime();
		parallelCalcA(a, b, N[k]);
		end3 = omp_get_wtime() - start3;
		//cout << "Time parallel atomic: " << end3 << endl;
		//cout << endl;
		start3 = omp_get_wtime();
		parallelCalcB(a, b, N[k]);
		end3 = omp_get_wtime() - start3;
		//cout << "Time parallel critical: " << end3 << endl;
		//cout << endl;
		for (int p = 0; p < 3; p++)
		{
			start2 = omp_get_wtime();
			parallelCalc(a, b, N[k], N_s[p]);
			end2 = omp_get_wtime() - start2;
			//cout << "Time parallel for " << N_s[p] << " sections: " << end2 << endl << endl;
		}
		start3 = omp_get_wtime();
		parallelBCalc(a, b, N[k]);
		end3 = omp_get_wtime() - start3;
		cout << "Time barrier: " << end3 << endl;
		cout << endl;
		start3 = omp_get_wtime();
		parallelLCalc(a, b, N[k]);
		end3 = omp_get_wtime() - start3;
		cout << "Time lock: " << end3 << endl;
		cout << endl;
	}
	system("PAUSE");
	return 0;
}
