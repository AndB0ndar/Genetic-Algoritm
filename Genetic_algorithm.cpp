#include <iostream>
#include <locale>
#include <cmath>

using namespace std;

long double function(double arr[3])
{
    /*return pow(arr[0]-3*9801.127, 2) + pow(arr[1]+17.47, 2) + pow(arr[2]+1.013, 2);*/
    return pow(arr[2], 3) - pow(arr[0], 2) + 3 * pow(arr[1], 2) - 1.5 * pow(arr[2], 2) - 4 * arr[0] + 6 * arr[1] + 2;
}

double random()
{
    int n = 5;
    /*return rand() % (int)pow(10, rand() % 4 + 1);*/
    return (double)((int)pow(rand(), 2) % (int)pow(10, (rand() % n + 1))) * (pow(10, -(n - 1) + (rand() % (n - 1) + 1)));

}

void sorted(long double* arr0, double arr1[][3], int LEN, int k = 1)
{
    if (k >= 0)
        k = 1;
    else
        k = -1;
    for (int i = 0; i < LEN - 1; i++) {
        for (int j = i + 1; j < LEN; j++) {
            if (k * arr0[i] > k * arr0[j]) {
                swap(arr0[i], arr0[j]);
                swap(arr1[i][0], arr1[j][0]);
                swap(arr1[i][1], arr1[j][1]);
                swap(arr1[i][2], arr1[j][2]);
            }
        }
    }
}

void crossing(double arr[][3], int k[3], int k_parence, double sum, int p, int LEN)
{
    if (rand()%2) {
        for (int u = 0; u < 3; u++)
            for (int i = 0; i < k_parence; i++)
                for (int j = i + 1; j < k_parence; j++)
                    if (p >= rand() % 100 + 1) {
                        double a = -0.25 + (double)(rand() % 125) / 100.0;
                        /*arr[k[u]][u] = (arr[i][u] + arr[j][u]) / 2;*/
                        arr[k[u]][u] = arr[i][u] + a * (arr[j][u] - arr[i][u]);  // Промежуточная рекомбинация (Intermediate recombination)
                        k[u]--;
                    }
    } else {
        int cstop = 20;
        for (int i = 0; i < LEN && (cstop>0); i++)
            if (rand() % 100 <= 100 * (function(arr[i]) / sum))
                for (int j = i + 1; j < LEN && (cstop>0); j++)
                    if (rand() % 100 <= 100 * (function(arr[i]) / sum))
                        for (int u = 0; u < 3; u++) {
                            arr[i][u] = arr[(rand() % 3) ? i : j][u];
                            cstop--;
                            k[u]--;
                        }

    }
}

void mutation(double arr[][3], int k[3], int k_parence, double eps, int p)
{
    if (rand()%2) {
            for (int i = 0; i < k_parence; i++)  // Real valued mutation
                for (int j = 0; j < 3; j++) {
                    arr[k[j]][j] = arr[i][j] + eps;
                    k[j]--;
                    arr[k[j]][j] = arr[i][j] - eps;
                    k[j]--;
                }
    } else {
        for (int i = 0; i < k_parence; i++)  // Real valued mutation
            for (int u = 0; u < 3; u++)
                if (rand() % 100 <= 10) {
                    arr[k[u]][u] = arr[i][u];
                    k[u]--;
                }
    }
    for (int j = 0; j < 3; j++) {
        if (p >= rand() % 100 + 1)
            for (int i = k_parence; i <= k[j]; i++)
                arr[i][j] = random();
    }
}

bool check_stop(double arr1[][3], double arr2[][3], int LEN)
{
    int x = 10;
    int k[2][3] = { 0 };
    for (int j = 0; j < 3; j++) {
        for (int i = 1; i < LEN; i++) {
            if (arr1[i][j] + x > arr1[0][j])
                k[0][j]++;
        }
    }
    if ((k[0][0] > (int)LEN * 0.5) && (k[0][1] > (int)LEN * 0.5) && (k[0][2] > (int)LEN * 0.5))
        return false;
    for (int j = 0; j < 3; j++) {
        for (int i = 1; i < LEN; i++) {
            if (arr2[i][j] + x > arr2[0][j])
                k[1][j]++;
        }
    }
    if ((k[1][0] > (int)LEN * 0.5) && (k[1][1] > (int)LEN * 0.5) && (k[1][2] > (int)LEN * 0.5))
        return false;
    return true;
}

int main() // функция номер 20
{
    setlocale(LC_ALL, "rus");

    double eps = 1e-2;
    double probability_m = 30, probability_c = 80;
    const int LEN = 100;
    int k_parence = 10, iter = 0;
    double sum_max = 0, sum_min = 0;
    int k_individuals_max[3], k_individuals_min[3];

    /*cout << "Введите точность: " << endl;
    cin >> eps;*/

    int k_gaps = 0;
    double population_max[LEN][3], population_min[LEN][3];
    long double func_min[LEN], func_max[LEN];

    for (int i = 0; i < LEN; i++) {  // Заполнение массивов
        for (int j = 0; j < 3; j++) {
            population_max[i][j] = random();
            population_min[i][j] = random();
            func_max[i] = function(population_max[i]);
            sum_max = func_max[i];
            func_min[i] = function(population_min[i]);  // Вычисление значения функции
            sum_min = func_min[i];
        }
    }
    sorted(func_max, population_max, LEN, -1);
    sorted(func_min, population_min, LEN);  // Сортировка массивов по значениям функции
    
    do
    {
        k_individuals_max[0] = k_individuals_max[1] = k_individuals_max[2] = LEN - 1;
        k_individuals_min[0] = k_individuals_min[1] = k_individuals_min[2] = LEN - 1;
        iter++;

        crossing(population_max, k_individuals_max, k_parence, sum_max, probability_c, LEN);   // Нахождение следующего поколения (среднее арифметическое)
        mutation(population_max, k_individuals_max, k_parence, eps, probability_m);  // Нахождение окрестности + Заполнение остатка массива случайными числами

        crossing(population_min, k_individuals_min, k_parence, sum_min, probability_c, LEN);
        mutation(population_min, k_individuals_min, k_parence, eps, probability_m);

        sum_max = 0;
        sum_min = 0;
        for (int i = 0; i < LEN; i++) {
            func_max[i] = function(population_max[i]);
            sum_max = func_max[i];
            func_min[i] = function(population_min[i]);
            sum_min = func_min[i];
        }
        sorted(func_max, population_max, LEN, -1);
        sorted(func_min, population_min, LEN);

    } while (iter < 10000);
    cout << iter << " Iterations" << endl;

    cout << "Максимум: " << endl;
    cout << "f(x, y, z) = " << func_max[0] << endl;
    cout << population_max[0][0] << " " << population_max[0][1] << " " << population_max[0][2] << endl;
    cout << "Минимум: " << endl;
    cout << "f(x, y, z) = " << func_min[0] << endl;
    cout << population_min[0][0] << " " << population_min[0][1] << " " << population_min[0][2] << endl;

    return 0;
}