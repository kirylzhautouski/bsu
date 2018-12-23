#include <omp.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

double GetElapsedMilliseconds(double start_time)
{
    return (omp_get_wtime() - start_time) * 1000;
}

double f(double x)
{
    return 0.5 * x * x;
}

double g(double x)
{
    return 2 * x + 7.8;
}

void FindMaxMin(double a, double b, unsigned n, double f(double), double g(double))
{
    auto start_time = omp_get_wtime();
    double result_min = numeric_limits<double>::max(), result_max = numeric_limits<double>::min();
    double delta_x = (b - a) / n;

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma	omp section
            {
                #pragma omp parallel for reduction(min : result_min)
                for (int i = 0; i < n; i++)
                {
                    auto x = a + delta_x * i;
                    auto x_next = x + delta_x;
                    auto result = (f(x_next) - f(x)) / delta_x;

                    if (result < result_min)
                        result_min = result;

                }
            }

            #pragma	omp section
            {
                #pragma omp parallel for reduction(max : result_max)
                for (int i = 1; i < n; i++)
                {
                    auto x = a + delta_x * i;
                    auto x_next = x + delta_x;
                    auto result = (g(x_next) - g(x)) / delta_x;

                    if (result > result_max)
                        result_max = result;
                }
            }
        }
    }

    auto ms = GetElapsedMilliseconds(start_time);

    cout << "min[f']=" << fixed << setprecision(2) << result_min << endl;
    cout << "max[g']=" << fixed << setprecision(2) << result_max << endl;

    ofstream fout("..\\time-results\\time_results.txt", ios::app);
    fout << "FindMaxMin(a=" << a << ", b=" << b << ", n=" << n <<
         ", num_of_threads=" << omp_get_max_threads() << "): " << ms << " ms" << endl;
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
        FindMaxMin(strtod(argv[2], nullptr), strtod(argv[3], nullptr), strtoul(argv[4], nullptr, 10), f, g);
    }
}