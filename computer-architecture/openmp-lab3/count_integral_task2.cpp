#include <omp.h>

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


double h(double x)
{
    return x * x;
}

double GetElapsedMilliseconds(double start_time)
{
    return (omp_get_wtime() - start_time) * 1000;
}

double CountIntegral(double a, double b, unsigned int n, double h(double))
{
    auto start_time = omp_get_wtime();

    double result_sum = 0;
    double delta_x = (b - a) / n;

    #pragma omp parallel for reduction(+ : result_sum)
    for (unsigned int i = 1; i < n; ++i)
    {
        double x1 = a + i * delta_x;
        double x3 = x1 + delta_x;
        double x2 = (x1 + x3) / 2;

        result_sum += ((x3 - x1) * (h(x1) + 4 * h(x2) + h(x3))) / 6;
    }

    auto ms = GetElapsedMilliseconds(start_time);
    ofstream fout("..\\time-results\\time_results.txt", ios::app);
    fout << "CountIntegral(a=" << a << ", b=" << b << ", n=" << n <<
         ", num_of_threads= " << omp_get_max_threads() << "): " << ms << " ms" << endl;

    return result_sum;
}


int main(int argc, char** argv)
{
    // <number of threads> <a> <b> <n>

    if (argc != 5)
    {
        cout << "usage: <number of threads> <a> <b> <n>" << endl;
    }
    else
    {
        omp_set_num_threads(strtol(argv[1], nullptr, 10));
        cout << fixed << setprecision(2) << CountIntegral(strtod(argv[2], nullptr), strtod(argv[3], nullptr), strtoul(argv[4], nullptr, 10), h) << endl;
    }
}