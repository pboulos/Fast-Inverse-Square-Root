#include <stdio.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <iostream>
#include <fstream>

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    printf("QueryPerformanceFrequency failed!\n");
     
    PCFreq = double(li.QuadPart)/1000000.0;
     
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

float Q_rsqrt(float number) {
        long i;
        float x2, y;
        const float threehalfs = 1.5F;
     
        x2 = number * 0.5F;
        y = number;
        i = *(long *) &y;
        i = 0x5f3759df - (i >> 1);
        y = *(float *) &i;
        y = y * (threehalfs - (x2 * y * y));
        // y = y * (threehalfs - (x2 * y * y));
           
        return y;
}
     
double testQ_rsqrt() {
        int i;
        float f, g;
		double c;
		StartCounter();
		// for (i = 1; i < 10000; i++) {
        for (i = 0x00800000; i < 0x7f800000; i++) {
                f = *(float *) &i;
                g = Q_rsqrt(f);
        }
		c = GetCounter();
		return c;
}
     
double testSqrtf() {
        int i;
        float f, g;
		double c;
		StartCounter();
		// for (i = 1; i < 10000; i++) {
        for (i = 0x00800000; i < 0x7f800000; i++) {
                f = *(float *) &i;
                g = 1.0f / sqrtf(f);
        }
		c = GetCounter();
		return c;
}
     
void main() {
        int i;
		double a, b;
        float d, f1;
		std::ofstream datafile;
		datafile.open("VarianceLog.txt");

        for (i = 0; i < 1000; i++) {
                a = testQ_rsqrt();
				datafile << i << "," << a << ",";
                printf("%f", a);
                b = testSqrtf();
				datafile << b << "\n";
                printf("\t%f", b);
                d = a / b;
				f1 += d;
                printf("\t%f\n", d);
        }

		f1 /= 1000;

		printf("%f", f1);

		datafile.close();

}
