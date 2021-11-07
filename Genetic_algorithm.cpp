#include <iostream>
#include <locale>
#include <cmath>

using namespace std;

long double function(double x, double y)
{
    return pow((x - 1), 4) + pow((y + 2), 4);
}

double random()
{
    return rand() % (int)pow(10, rand() % 4 + 1);
    return (int)pow(rand(), 2) % (int)pow(10, rand() % 4 + 1);
}

void sorted(long double* arr0, double* arr1, double* arr2, int LEN, int k = 1)
{
    if (k >= 0)
        k = 1;
    else
        k = -1;
    for (int i = 0; i < LEN - 1; i++) {
        for (int j = i + 1; j < LEN; j++) {
            if (k * arr0[i] > k * arr0[j]) {
                swap(arr0[i], arr0[j]);
                swap(arr1[i], arr1[j]);
                swap(arr2[i], arr2[j]);
            }
        }
    }
}

void crossing(double* arr, int& k, int k_parence, int p)
{
    if (p < rand() % 100 + 1)
        for (int i = 0; i < k_parence; i++) {
            for (int j = i + 1; j < k_parence; j++) {
                double a = -0.25 + (double)(rand() % 125) / 100.0;
                /*arr[k] = (arr[i] + arr[j]) / 2;*/
                arr[k] = arr[i] + a * (arr[j] - arr[i]);  // Промежуточная рекомбинация (Intermediate recombination)
                k--;
            }
        }
}

void mutation1(double* arr, int& k, int k_parence, double eps)
{
    for (int i = 0; i < k_parence; i++) {
        arr[k] = arr[i] + eps;
        k--;
        arr[k] = arr[i] - eps;
        k--;
    }
}

void mutation2(double* arr, int& k, int k_parence, int p)
{
    if (p < rand() % 100 + 1)
        for (int i = k_parence; i <= k; i++)
            arr[i] = random();;
}

int main() // функция номер 20
{
    setlocale(LC_ALL, "rus");

    double eps = 1e-2;
    double probability_m = 30, probability_c = 80;
    const int LEN = 100;
    int k_x, k_y, k_parence = 10, iter = 0;

    /*cout << "Введите точность: " << endl;
    cin >> eps;*/

    int k_gaps = 0;
    double x_population[LEN], x1_population[LEN];
    double y_population[LEN], y1_population[LEN];
    long double func_min[LEN], func_max[LEN];

    for (int i = 0; i < LEN; i++) {  // Заполнение массивов
        x_population[i] = random();
        x1_population[i] = random();
        y_population[i] = random();
        y1_population[i] = random();
        func_min[i] = function(x_population[i], y_population[i]);  // Вычисление значения функции
        func_max[i] = function(x1_population[i], y1_population[i]);
    }
    sorted(func_min, x_population, y_population, LEN);  // Сортировка массивов по значениям функции
    sorted(func_max, x1_population, y1_population, LEN, -1);

    while (iter < 10000) {
        k_x = k_y = LEN - 1;
        iter++;

        crossing(x_population, k_x, k_parence, probability_c);   // Нахождение следующего поколения (среднее арифметическое)
        mutation1(x_population, k_x, k_parence, eps);  // Нахождение окрестности
        mutation2(x_population, k_x, k_parence, probability_m);  // Заполнение остатка массива случайными числами

        crossing(y_population, k_y, k_parence, probability_c);
        mutation1(y_population, k_y, k_parence, eps);
        mutation2(y_population, k_y, k_parence, probability_m);

        for (int i = 0; i < LEN; i++) {
            func_min[i] = function(x_population[i], y_population[i]);
            func_max[i] = function(x1_population[i], y1_population[i]);
       }
        sorted(func_min, x_population, y_population, LEN);
        sorted(func_max, x1_population, y1_population, LEN, -1);
    }
    cout << iter << " Iterations" << endl;

    cout << "Максимум: " << endl;
    cout << "f(x, y, z) = " << func_max[0] << endl;
    cout << x1_population[0] << " " << y1_population[0] << endl;
    cout << "Минимум: " << endl;
    cout << "f(x, y, z) = " << func_min[0] << endl;
    cout << x_population[0] << " " << y_population[0] << endl;

    return 0;
}