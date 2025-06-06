#include <iostream>
#include <omp.h>
#include <climits>

using namespace std;

const int rows = 10000;
const int cols = 10000;

int arr[rows][cols];

void init_arr();

long long ElementsSum(int numOfThreads);
pair<int, int> getMinSum(int numOfThreads);

int main() {
    init_arr();

    omp_set_nested(1);
    double t1 = omp_get_wtime();

#pragma omp parallel sections
    {
    #pragma omp section
        {
            cout << "total sum 1 = " << ElementsSum(1) << endl;

            cout << "total sum 2 = " << ElementsSum(2) << endl;

            cout << "total sum 4 = " << ElementsSum(4) << endl;

            cout << "total sum 8 = " << ElementsSum(8) << endl;

            cout << "total sum 16 = " << ElementsSum(16) << endl;

            cout << "total sum 32 = " << ElementsSum(32) << endl;
        }

    #pragma omp section
        {
            auto result = getMinSum(1);
            cout << "min sum = " << result.first << " in row = " << result.second << endl;

            result = getMinSum(2);
            cout << "min sum = " << result.first << " in row = " << result.second << endl;

            result = getMinSum(4);
            cout << "min sum = " << result.first << " in row = " << result.second << endl;

            result = getMinSum(8);
            cout << "min sum = " << result.first << " in row = " << result.second << endl;

            result = getMinSum(16);
            cout << "min sum = " << result.first << " in row = " << result.second << endl;

            result = getMinSum(32);
            cout << "min sum = " << result.first << " in row = " << result.second << endl;
        }
    }

    double t2 = omp_get_wtime();
    cout << "Total time - " << t2 - t1 << " seconds" << endl;

    return 0;
}

void init_arr() {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            arr[i][j] = 1;

    arr[2][2] = -100;
}

//обрахунок суми
long long ElementsSum(int numOfThreads) {
    long long sum = 0;
    double t1 = omp_get_wtime();

#pragma omp parallel for reduction(+:sum) num_threads(numOfThreads)
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            sum += arr[i][j];

    double t2 = omp_get_wtime();
    cout << "total sum " << numOfThreads << " threads worked - " << t2 - t1 << " seconds" << endl;

    return sum;
}


//знаходження мінімальної суми
pair<int, int> getMinSum(int numOfThreads) {
    pair<int, int> result = { INT_MAX, -1 };
    double t1 = omp_get_wtime();

#pragma omp parallel num_threads(numOfThreads)
    {
        pair<int, int> localMin = { INT_MAX, -1 };

#pragma omp for
        for (int i = 0; i < rows; i++) {
            long long rowSum = 0;
            for (int j = 0; j < cols; j++)
                rowSum += arr[i][j];

            if (rowSum < localMin.first) {
                localMin.first = rowSum;
                localMin.second = i;
            }
        }

#pragma omp critical
        {
            if (localMin.first < result.first) {
                result = localMin;
            }
        }
    }

    double t2 = omp_get_wtime();
    cout << "min sum " << numOfThreads << " threads worked - " << t2 - t1 << " seconds" << endl;

    return result;
}