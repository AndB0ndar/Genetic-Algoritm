#include <iostream>
#include <locale>
#include <cmath>

using namespace std;

long double function(double arr[3])
{
    return pow(arr[0]-81.127, 2) + pow(arr[1]+61.47, 2) + pow(arr[2]-731.07, 2);
    /*return pow(arr[2], 3) - pow(arr[0], 2) + 3 * pow(arr[1], 2) - 1.5 * pow(arr[2], 2) - 4 * arr[0] + 6 * arr[1] + 2;*/
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

void crossing(double arr[][3], int k[3], int k_parence, int p, int LEN, int q = 1)
{
    if (0) {
        for (int u = 0; u < 3; u++) {
            for (int i = 0; i < k_parence; i++) {
                for (int j = i + 1; j < k_parence; j++) {
                    if (p >= rand() % 100 + 1) {
                        double a = -0.25 + (double)(rand() % 125) / 100.0;
                        /*arr[k[u]][u] = (arr[i][u] + arr[j][u]) / 2;*/
                        arr[k[u]][u] = arr[i][u] + a * (arr[j][u] - arr[i][u]);  // Промежуточная рекомбинация (Intermediate recombination)
                        k[u]--;
                    }
                }
            }
        }
    } else {
        int cstop = 30;
        double *a[2];
        for (int i = 0; i < LEN && (cstop > 0); i++) {
            for (int j = i + 1; j < LEN && (cstop > 0); j++) {
                for (int e = 0; e < 2; e++) {  // элитарный выбор
                    double *b, *c;
                    b = arr[rand() % LEN];
                    c = arr[rand() % LEN];
                    if (q * function(c) < q * function(b))
                        a[e] = b;
                    else
                        a[e] = c;
                }
                for (int u = 0; u < 3; u++) {
                    arr[k[u]][u] = a[(rand() % 2)][u];
                    k[u]--;
                }
                cstop--;
            }
        }
    }
}

void mutation(double arr[][3], int k[3], int k_parence, int p)
{
    for (int i = 0; i < k_parence; i++)  // Real valued mutation
        if (rand() % 2) {
            double eps = pow(10, -2 + rand() % 4);
            for (int j = 0; j < 3; j++) {
                arr[k[j]][j] = arr[i][j] + eps;
                k[j]--;
                arr[k[j]][j] = arr[i][j] - eps;
                k[j]--;
                if (k[j] == 0)
                    return;
            }
        }
        else {
            for (int i = 0; i < k_parence; i++)  // Real valued mutation
                for (int u = 0; u < 3; u++) {
                    arr[k[u]][u] = arr[i][u];
                    if (rand() % 100 <= p)
                        arr[k[u]][u] = random();
                    k[u]--;
                    if (k[u] == 0)
                        return;
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

    /*double eps = 0.1;*/
    double probability_m = 60, probability_c = 80;
    const int LEN = 100;
    int k_parence = 10, iter = 0;
    int k_individuals_max[3], k_individuals_min[3];

    /*cout << "Введите точность: " << endl;
    cin >> eps;*/

    double population_max[LEN][3], population_min[LEN][3];
    long double func_min[LEN], func_max[LEN];

    for (int i = 0; i < LEN; i++) {  // Заполнение массивов
        for (int j = 0; j < 3; j++) {
            population_max[i][j] = random();
            population_min[i][j] = random();
            func_max[i] = function(population_max[i]);
            func_min[i] = function(population_min[i]);  // Вычисление значения функции
        }
    }
    sorted(func_max, population_max, LEN, -1);
    sorted(func_min, population_min, LEN);  // Сортировка массивов по значениям функции
    
    do
    {
        k_individuals_max[0] = k_individuals_max[1] = k_individuals_max[2] = LEN - 1;
        k_individuals_min[0] = k_individuals_min[1] = k_individuals_min[2] = LEN - 1;
        iter++;

        crossing(population_max, k_individuals_max, k_parence, probability_c, LEN);   // Нахождение следующего поколения (среднее арифметическое)
        mutation(population_max, k_individuals_max, k_parence, probability_m);  // Нахождение окрестности + Заполнение остатка массива случайными числами

        crossing(population_min, k_individuals_min, k_parence, probability_c, LEN, -1);
        mutation(population_min, k_individuals_min, k_parence, probability_m);

        for (int i = 0; i < LEN; i++) {
            func_max[i] = function(population_max[i]);
            func_min[i] = function(population_min[i]);
        }
        sorted(func_max, population_max, LEN, -1);
        sorted(func_min, population_min, LEN);

    } while (iter < 2000);
    cout << iter << " Iterations" << endl;

    cout << "Максимум: " << endl;
    cout << "f(x, y, z) = " << func_max[0] << endl;
    cout << population_max[0][0] << " " << population_max[0][1] << " " << population_max[0][2] << endl;
    cout << "Минимум: " << endl;
    cout << "f(x, y, z) = " << func_min[0] << endl;
    cout << population_min[0][0] << " " << population_min[0][1] << " " << population_min[0][2] << endl;

    return 0;
}