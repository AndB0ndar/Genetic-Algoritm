#include <iostream>
#include <locale>
#include <cmath>

using namespace std;

long double function(double arr[3])
{
    /*return pow(arr[1],4) + 2 * arr[0] *arr[1] + pow(arr[0], 2) + 2*arr[1] + pow(arr[1],2);*/
    return -pow(arr[0],2) - 5 * pow(arr[1],2) - 3 * pow(arr[2], 2) + arr[0] * arr[1] - 2 * arr[0] * arr[2] + 2 * arr[1] * arr[2] + 11 * arr[0] + 2 * arr[1] + 18 * arr[2] + 10;
    return 11/(1+pow(arr[0]-8.04, 2)) + 1/(1+pow(arr[1]+0.455557, 2)) + 1/(1+pow(arr[2]+10.1087, 2));
    return pow(arr[2], 3) - pow(arr[0], 2) + 3 * pow(arr[1], 2) - 1.5 * pow(arr[2], 2) - 4 * arr[0] + 6 * arr[1] + 2;
}

double random()
{
    int n = 6;
    int n1 = 4;
    /*return rand() % (int)pow(10, rand() % 4 + 1);*/
    return (double)((int)pow(rand(), 2) % (int)pow(10, (rand() % n + 1))) * (pow(10, -(n1) + (rand() % (n1) + 1)));
}

void sorted(long double* arr0, double arr1[][3], int LEN, int k = 1)
{
    if (k >= 0)
        k = 1;
    else
        k = -1;
    for (int i = 0; i < LEN - 1; i++) {
        for (int j = i + 1; j < LEN; j++) {
            if (k * arr0[i] < k * arr0[j]) {
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
    if (rand()%3) {
        for (int u = 0; u < 3; u++) {
            for (int i = 0; i < k_parence; i++) {
                for (int j = i + 1; j < k_parence; j++) {
                    if (p >= rand() % 100 + 1) {
                        double a = -0.25 + (double)(rand() % 125) / 100.0;
                        //arr[k[u]][u] = (arr[i][u] + arr[j][u]) / 2;  // ������� ��������������
                        arr[k[u]][u] = arr[i][u] + a * (arr[j][u] - arr[i][u]);  // ������������� ������������ (Intermediate recombination)
                        k[u]--;
                    }
                }
            }
        }
    } else {
        int cstop = 10;
        double *a[2];
        for (int i = 0; i < LEN && (cstop > 0); i++) {
            for (int j = i + 1; j < LEN && (cstop > 0); j++) {
                for (int e = 0; e < 2; e++) {  // ��������� ����� ���������
                    double *b, *c;
                    b = arr[rand() % LEN];
                    c = arr[rand() % LEN];
                    if (q * function(c) < q * function(b))
                        a[e] = b;
                    else
                        a[e] = c;
                }
                for (int u = 0; u < 3; u++) {
                    arr[k[u]][u] = a[(rand() % 2)][u];  // ���������� ������������ (Discrete recombination)
                    k[u]--;
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
        if (rand()%3) {
            double eps = pow(10, -2 + rand() % 4);
            for (int j = 0; j < 3; j++) {
                arr[k[j]][j] = arr[i][j] + eps;
                k[j]--;
                arr[k[j]][j] = arr[i][j] - eps;
                k[j]--;
                if (k[j] <= 10)
                    return;
            }
        }
        else {
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

bool check_stop(long double arr1[], long double arr2[], int LEN)
{
    int k[2][3] = { 0 };
    for (int j = 0; j < 3; j++) {
        for (int i = 1; i < LEN*0.45; i++) {  // a == b
            if (arr1[i] == arr1[0])
                k[0][j]++;
            if (arr2[i] == arr2[0])
                k[1][j]++;
        }
        if (k[0][j] >= 40)
            return false;
        if (k[1][j] >= 40)
            return false;
    }
    return true;
}

int main() // ������� ����� 20
{
    setlocale(LC_ALL, "rus");
    system("chcp 1251");
    system("cls");

    double probability_m = 60, probability_c = 80;
    const int LEN = 100;
    int iter_max = 1000;
    int k_parence = 10, iter = 0;
    int k_individuals_max[3], k_individuals_min[3];

    int q;
    /*cout << "������ ������� ����������(1-��): " << endl;
    cin >> q;*/
    q = 0;
    switch (q)
    {
    case 1:
        cout << "������� ����������� ����������� (������������) [0 - 100]: " << endl;
        cin >> probability_c;
        if (probability_c < 0 || probability_c > 100) {
            cout << "����������� ������" << endl;
            return 0;
        }
        cout << "������� ����������� ������� [0 - 100]: " << endl;
        cin >> probability_m;
        if (probability_m < 0 || probability_m > 100) {
            cout << "����������� ������" << endl;
            return 0;
        }
        cout << "������� ������������ ���������� �������� [>0]: " << endl;
        cin >> iter_max;
        if (iter_max < 0) {
            cout << "����������� ������" << endl;
            return 0;
        }
        break;
    default:
        break;
    }

    double population_max[LEN][3], population_min[LEN][3];
    long double func_min[LEN], func_max[LEN];

    for (int i = 0; i < LEN; i++) {  // ���������� ��������
        for (int j = 0; j < 3; j++) {
            population_max[i][j] = random();
            population_min[i][j] = random();
        }
        func_max[i] = function(population_max[i]);
        func_min[i] = function(population_min[i]);  // ���������� �������� �������
    }
    sorted(func_max, population_max, LEN);
    sorted(func_min, population_min, LEN, -1);  // ���������� �������� �� ��������� �������
    
    do
    {
        iter++;
        k_individuals_max[0] = k_individuals_max[1] = k_individuals_max[2] = LEN - 1;
        k_individuals_min[0] = k_individuals_min[1] = k_individuals_min[2] = LEN - 1;

        crossing(population_max, k_individuals_max, k_parence, probability_c, LEN);   // ���������� � ����������� ���������� ���������
        mutation(population_max, k_individuals_max, k_parence, probability_m);  // ���������� ����������� + ���������� ������� ������� ���������� �������

        crossing(population_min, k_individuals_min, k_parence, probability_c, LEN, -1);
        mutation(population_min, k_individuals_min, k_parence, probability_m);

        for (int i = 0; i < LEN; i++) {
            func_max[i] = function(population_max[i]);
            while (isnan(func_max[i]) || isinf(func_max[i])) {
                for (int j = 0; j < 3; j++)
                    population_max[i][j] = random();
                func_max[i] = function(population_max[i]);
            }
            func_min[i] = function(population_min[i]);
            while (isnan(func_min[i]) || isinf(func_min[i])) {
                for (int j = 0; j < 3; j++)
                    population_min[i][j] = random();
                func_min[i] = function(population_min[i]);
            }
        }
        sorted(func_max, population_max, LEN);
        sorted(func_min, population_min, LEN, -1);
    } while (iter < iter_max && check_stop(func_max, func_min, LEN));
    
    cout << iter << " Iterations" << endl;
    cout << "��������: " << endl;
    cout << "f(x, y, z) = " << func_max[0] << endl;
    cout << population_max[0][0] << " " << population_max[0][1] << " " << population_max[0][2] << endl;
    cout << "�������: " << endl;
    cout << "f(x, y, z) = " << func_min[0] << endl;
    cout << population_min[0][0] << " " << population_min[0][1] << " " << population_min[0][2] << endl;

    return 0;
}