#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <complex.h>

#define N 100
#define lambda 1
#define nc 20
#define tmax 200
const double PI = 3.14159265358979323846;

void Norma(double complex phi[], int n, double* norma) {
    int j;
    *norma = 0.0; // Inicializo la norma a cero
    for (j = 0; j < N; j++) {
        (*norma) += pow(cabs(phi[j]), 2);
    }
    *norma=pow(*norma,0.5);
}

int main() {
    int  j,n;
    n = 0;
    double t;
    t=0.0;

    double k0, s, h;
    double V[N];
    double complex phi[N], b[N], A0[N], beta[N], xhi[N];
    double complex alpha[N], gamma[N];

    h = 1.0 / (N * 1.0); //espaciado espacial
    k0 = (2 * PI * nc * 1.0) / (N * 1.0); 
    s = 1.0 / (4.0 * pow(k0, 2)); // espaciado temporal

    for (j = 0; j < N; j++) {
        if (j > (0.4 * (N-1) ) && j < (0.6 * (N-1) )) V[j] = lambda * pow(k0, 2);
        else V[j] = 0.0;

        if (j == 0 || j == N-1) phi[j] =0.0 + 0.0*I; // Condicion de contorno inicial
        else phi[j] = cexp(1* I * k0 * j) * exp(-8 * pow((4 * j - N * 1.0), 2) / (N * N * 1.0));

        b[j] = 4 * I * phi[j];
        A0[j] = -2 + 2 * I / s - V[j];
    }

    for (j = N - 2; j >= 0; j--) {
        if (j == N - 2) {
            alpha[j] = 0;
        } else {
            alpha[j] = -1 / (A0[j + 1] + alpha[j + 1]);
        }
    }

    for (j = 0; j < N - 1; j++) {
        gamma[j] = 1 / (A0[j] + alpha[j]);
    }

    FILE *fichonda, *fichnorma;
    fichonda = fopen("schrodinger_data.dat", "w");
    fichnorma = fopen("norma.dat", "w");

    if (!fichonda || !fichnorma) {
        perror("Failed to open file");
        exit(EXIT_FAILURE); 
    }

    while (t < tmax) {
        beta[N - 1] = 0.0 + 0.0*I;
        
        for (j = 0; j < N; j++) {
            b[j] = 4 * I * phi[j] / s;
        }

        for (j = N - 2; j >= 0; j--) {
            if (j == N - 2) {
                beta[j] = 0;
            } else {
                beta[j] = (gamma[j + 1]) * (b[j + 1] - beta[j + 1]);
            }
        }

        xhi[0] = 0.0 + 0.0*I;
        xhi[N - 1] = 0.0 + 0.0*I; // Condiciones de contorno

        for (j = 1; j < N - 1; j++) { 
            xhi[j] = alpha[j - 1] * xhi[j - 1] + beta[j - 1];
        }

        for (j = 0; j < N; j++) {
           // if (j == 0 || j == N-1) phi[j] = 0;
             phi[j] = xhi[j] - phi[j];
        }

        double norma = 0;
        Norma(phi, N, &norma);
        fprintf(fichnorma, "%d\t%lf\n", n, norma);

        for (j = 0; j < N; j++) {
            fprintf(fichonda, "%.2f,\t%.2f,\t%.2f\n", h * j, cabs(phi[j]), cabs(V[j]));
        }
        fprintf(fichonda, "\n"); 

        n += 1;
        t = n * s;
    }

    fclose(fichonda);
    fclose(fichnorma);

    return 0;
}
