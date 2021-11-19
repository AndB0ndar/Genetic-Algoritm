#include <iostream>
#include <locale>
#include <cmath>

#define LEN 100

using namespace std;

long double function(double arr[3])
{
    //return -pow(arr[0], 2) - 5 * pow(arr[1], 2) - 3 * pow(arr[2], 2) + arr[0] * arr[1] - 2 * arr[0] * arr[2] + 2 * arr[1] * arr[2] + 11 * arr[0] + 2 * arr[1] + 18 * arr[2] + 10; // 17
    return 11 / (1 + pow(arr[0] - 8.04, 2)) + 1 / (1 + pow(arr[1] + 0.455557, 2)) + 1 / (1 + pow(arr[2] + 10.1087, 2));
}

double random()
{
    int n = 6;
    int n1 = 4;
    return (double)((int)pow(rand(), 2) % (int)pow(10, (rand() % n + 1))) * (pow(10, -(n1)+(rand() % (n1)+1)));
}

void sorted(long double arr0[], double arr1[][3], int multiplier = 1)
{
    if (multiplier >= 0)
        multiplier = 1;
    else
        multiplier = -1;
    double factor = 1.2473309;
    int step = LEN - 1;
    while (step > 0) {
        for (int i = 0; i + step < LEN; i++) {
            if (multiplier * arr0[i] < multiplier * arr0[i + step]) {
                swap(arr0[i], arr0[i + step]);
                swap(arr1[i][0], arr1[i + step][0]);
                swap(arr1[i][1], arr1[i + step][1]);
                swap(arr1[i][2], arr1[i + step][2]);
            }
        }
        step /= factor;
    }
}

void crossing(double arr[][3], int k[3], int k_parence, int p, int multiplier = 1)
{
    if (multiplier >= 0)
        multiplier = 1;
    else
        multiplier = -1;
    if (rand() % 3) {
        for (int u = 0; u < 3; u++) {
            for (int i = 0; i < k_parence - 1; i++) {
                for (int j = i + 1; j < k_parence; j++) {
                    if (p >= rand() % 100 + 1) {
                        double a = -0.25 + ((double)(rand() % 125) / 100.0);
                        // arr[k[u]][u] = (arr[i][u] + arr[j][u]) / 2;  // среднее арифметическое
                        arr[k[u]][u] = arr[i][u] + a * (arr[j][u] - arr[i][u]);  // Промежуточная рекомбинация (Intermediate recombination)
                        k[u]--;
                    }
                }
            }
        }
    }
    else {
        int cstop = 15;
        double* a[2];
        while (cstop) {
            for (int e = 0; e < 2; e++) {  // турнирный выбор родителей
                double* b, * c;
                b = arr[rand() % LEN];
                c = arr[rand() % LEN];
                if (multiplier * function(c) < multiplier * function(b))
                    a[e] = b;
                else
                    a[e] = c;
            }
            for (int u = 0; u < 3; u++) {
                arr[k[u]][u] = a[(rand() % 2)][u];  // дискретная рекомбинация (Discrete recombination)
                k[u]--;
                arr[k[u]][u] = a[(rand() % 2)][u];
                k[u]--;
            }
            cstop--;
        }
    }
}

void mutation(double arr[][3], int k[3], int k_parence, int p)
{
    for (int i = 0; i < k_parence; i++) {  // Real valued mutation
        if (rand() % 3) {
            double eps = pow(10, -2 + rand() % 4);
            for (int j = 0; j < 3; j++) {
                arr[k[j]][j] = arr[i][j] + eps;
                k[j]--;
                arr[k[j]][j] = arr[i][j] - eps;
                k[j]--;
                if (k[j] <= 10)
                    return;
            }
        } else {
            for (int i = 0; i < k_parence; i++) {
                for (int u = 0; u < 3; u++) {
                    arr[k[u]][u] = arr[i][u];
                    if (rand() % 100 + 1 <= p)
                        arr[k[u]][u] = random();
                    k[u]--;
                    if (k[u] <= 10)
                        return;
                }
            }
        }
    }
    for (int u = 0; u < 3; u++) {
        for (int i = k_parence; i <= k[u]; i++) {
            if (p >= rand() % 100 + 1) {
                for (int j = 0; j < 3; j++) {
                    arr[i][j] = random();
                }
            }
        }
    }
}

double** change_gaps(double** arr0, double arr1[3], int& n)
{
    double** arr;
    arr = new double* [n];
    int i = 0;
    for (i; i < n - 1; i++) {
        arr[i] = new double[3];
        for (int j = 0; j < 3; j++)
            arr[i][j] = arr0[i][j];
    }
    arr[i] = new double[3];
    for (int j = 0; j < 3; j++)
        arr[i][j] = arr1[j];

    for (i = 0; i < n - 1; i++)
        delete[] arr0[i];
    delete[] arr0;
    n++;
    return arr;
}

bool check_gap(double arr[3])
{
    double arr1[3], arr2[3];
    for (int i = 0; i < 3; i++) {
        arr1[i] = arr[i];
        arr2[i] = arr[i];
    }
    for (int i = 0; i < 3; i++) {
        arr1[i]++;
        arr2[i]--;
        if (isinf(function(arr1)) || isinf(function(arr2)))
            return true;
    }
    return false;
}

bool check_stop(long double arr1[], long double arr2[])
{
    int k[2][3] = { 0 };
    for (int j = 0; j < 3; j++) {
        for (int i = 1; i < (int)(LEN * 0.45); i++) {
            if (arr1[i] == arr1[0])
                k[0][j]++;
            if (arr2[i] == arr2[0])
                k[1][j]++;
        }
        if (k[0][j] >= (int)(LEN * 0.4))
            return false;
        if (k[1][j] >= (int)(LEN * 0.4))
            return false;
    }
    return true;
}

int main()
{
    setlocale(LC_ALL, "rus");
    system("chcp 1251");
    system("cls");

    int probability_m = 60, probability_c = 80;
    int iter_max = 1000;
    int k_parence = 10, iter = 0;
    int k_individuals_max[3], k_individuals_min[3];

    double population_max[LEN][3], population_min[LEN][3];
    long double func_min[LEN], func_max[LEN];
    int k_gaps = 1;
    double** gaps;
    gaps = new double* [k_gaps];
    gaps[0] = new double[k_gaps];
    bool f_gap = false;

    for (int i = 0; i < LEN; i++) {  // Заполнение массивов
        for (int j = 0; j < 3; j++) {
            population_max[i][j] = random();
            population_min[i][j] = random();
        }
        func_max[i] = function(population_max[i]);
        func_min[i] = function(population_min[i]);  // Вычисление значения функции
    }
    sorted(func_max, population_max);
    sorted(func_min, population_min, -1);  // Сортировка массивов по значениям функции

    do
    {
        iter++;
        k_individuals_max[0] = k_individuals_max[1] = k_individuals_max[2] = LEN - 1;
        k_individuals_min[0] = k_individuals_min[1] = k_individuals_min[2] = LEN - 1;

        crossing(population_max, k_individuals_max, k_parence, probability_c);   // Нахождение родителей и скрещивание следующего поколения
        mutation(population_max, k_individuals_max, k_parence, probability_m);  // Мутация

        crossing(population_min, k_individuals_min, k_parence, probability_c, -1);
        mutation(population_min, k_individuals_min, k_parence, probability_m);

        for (int i = 0; i < LEN; i++) {
            func_max[i] = function(population_max[i]);
            while (isnan(func_max[i]) || isinf(func_max[i])) {
                if (check_gap(population_max[i])) {
                    gaps = change_gaps(gaps, population_max[i], k_gaps);
                    f_gap = true;
                }
                for (int j = 0; j < 3; j++)
                    population_max[i][j] = random();
                func_max[i] = function(population_max[i]);
            }
            func_min[i] = function(population_min[i]);
            while (isnan(func_min[i]) || isinf(func_min[i])) {
                if (check_gap(population_min[i])) {
                    gaps = change_gaps(gaps, population_min[i], k_gaps);
                    f_gap = true;
                }
                for (int j = 0; j < 3; j++)
                    population_min[i][j] = random();
                func_min[i] = function(population_min[i]);
            }
        }
        sorted(func_max, population_max);
        sorted(func_min, population_min, -1);
    } while (iter < iter_max && check_stop(func_max, func_min));

    cout << iter << " Iterations" << endl;
    cout << "Найденный максимум: " << endl;
    cout << "f(x, y, z) = " << func_max[0] << endl;
    cout << population_max[0][0] << " " << population_max[0][1] << " " << population_max[0][2] << endl;
    cout << "Найденный минимум: " << endl;
    cout << "f(x, y, z) = " << func_min[0] << endl;
    cout << population_min[0][0] << " " << population_min[0][1] << " " << population_min[0][2] << endl;

    cout << "Точки разрывов: " << endl;
    if (!f_gap)
        cout << "Нет точек разрыва" << endl;
    else
        for (int i = 0; i < k_gaps - 1; i++) {
            cout << i + 1 << ":";
            cout.width(3);
            for (int j = 0; j < 3; j++) {
                cout << gaps[i][j] << " ";
            }
            cout << endl;
        }
    for (int i = 0; i < k_gaps - 1; i++) {
        delete[] gaps[i];
    }
    delete[] gaps;

    return 0;
}