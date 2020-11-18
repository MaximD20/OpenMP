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
		//cout << "a: "<<a[i]<<"b"<<b[i]<<endl;
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
		cout << total;
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
				cout << total;
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
			cout << total;
		}
	}
}
int main()
{
	setlocale(LC_ALL, "ru");
	int N[6] = { 50000, 60000,70000,80000,1000000 };
	for (int k = 0; k < 5; k++)
	{
		int* a = new int[N[k]];
		int* b = new int[N[k]];
		init(a, b, N[k]);
		cout << "===== LINEAR =====" << endl;
		double start, end, start2, end2, start3, end3;
		start = omp_get_wtime();
		linearCalc(a, b, N[k]);
		end = omp_get_wtime() - start;
		cout << endl;
		cout << "===== PARALLEL Atomic =====" << endl;
		start2 = omp_get_wtime();
		parallelCalcA(a, b, N[k]);
		end2 = omp_get_wtime() - start2;
		cout << "===== PARALLEL Critical =====" << endl;
		start3 = omp_get_wtime();
		parallelCalcB(a, b, N[k]);
		end3 = omp_get_wtime() - start3;
		cout << endl << "Result for N = " << N[k] << endl;
		cout << "===== Time linear: " << end << endl;
		cout << "Time parallel (atomic): " << end2 << endl << endl;
		cout << "Time parallel (critical): " << end3<< endl << endl;
		system("PAUSE");
	}
	system("PAUSE");
	return 0;
}
