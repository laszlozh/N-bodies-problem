#include <iostream>
#include <omp.h>

#define G 6.67e-11

using namespace std;

void count(int countOfBodies, double* X, double* Y, double* Vx, double* Vy, double* M, double* countedX, double* countedY, double* countedVx, double* countedVy, double h, double T);
double ax(int countOfBodies, int bodyInd, double* X, double* Y, double* M);
double ay(int countOfBodies, int bodyInd, double* X, double* Y, double* M);
void addVector(int countOfBodies, double* result, double* X1, double* X2);
void copyVector(int countOfBodies, double* result, double* X);
void multVector(int countOfBodies, double* result, double* X, double coef);

int main()
{
    int countOfBodies = 200;
    double* X = (double*)malloc(sizeof(double) * countOfBodies);
    double* Y = (double*)malloc(sizeof(double) * countOfBodies);
    double* Vx = (double*)malloc(sizeof(double) * countOfBodies);
    double* Vy = (double*)malloc(sizeof(double) * countOfBodies);
    double* M = (double*)malloc(sizeof(double) * countOfBodies);

    for (int i = 0; i < countOfBodies; i++) {
        X[i] = (rand() / (double)RAND_MAX * 384400000 * 20) - 10 * 384400000;
        Y[i] = (rand() / (double)RAND_MAX * 384400000 * 20) - 10 * 384400000;;
        Vx[i] = (rand() / (double)RAND_MAX * 100 * 20) - 10 * 100;
        Vy[i] = (rand() / (double)RAND_MAX * 100 * 20) - 10 * 100;
        M[i] = (rand() / (double)RAND_MAX * 5.972e24 * 5);
    }

    double* countedX = (double*)malloc(sizeof(double) * countOfBodies);
    double* countedY = (double*)malloc(sizeof(double) * countOfBodies);
    double* countedVx = (double*)malloc(sizeof(double) * countOfBodies);
    double* countedVy = (double*)malloc(sizeof(double) * countOfBodies);

 
    omp_set_num_threads(14);

    double start_time, end_time;

    start_time = omp_get_wtime();
    count(countOfBodies, X, Y, Vx, Vy, M, countedX, countedY, countedVx, countedVy, 3600, 273.0 * 86400.0);
    end_time = omp_get_wtime();

    printf("Time: %.6f seconds\n", end_time - start_time);

    free(X); free(Y); free(Vx); free(Vy); free(M);
    free(countedX); free(countedY); free(countedVx); free(countedVy);
}

void count(int countOfBodies, double* X, double* Y, double* Vx, double* Vy, double* M, double* countedX, double* countedY, double* countedVx, double* countedVy, double h, double T) {
    int countOfSteps = (int)(T / h);

    copyVector(countOfBodies, countedX, X);
    copyVector(countOfBodies, countedY, Y);
    copyVector(countOfBodies, countedVx, Vx);
    copyVector(countOfBodies, countedVy, Vy);

    double* a1_x = (double*)malloc(sizeof(double) * countOfBodies);
    double* a2_x = (double*)malloc(sizeof(double) * countOfBodies);
    double* a3_x = (double*)malloc(sizeof(double) * countOfBodies);
    double* a4_x = (double*)malloc(sizeof(double) * countOfBodies);

    double* a1_y = (double*)malloc(sizeof(double) * countOfBodies);
    double* a2_y = (double*)malloc(sizeof(double) * countOfBodies);
    double* a3_y = (double*)malloc(sizeof(double) * countOfBodies);
    double* a4_y = (double*)malloc(sizeof(double) * countOfBodies);

    double* buf_x = (double*)malloc(sizeof(double) * countOfBodies);
    double* buf_y = (double*)malloc(sizeof(double) * countOfBodies);

    /*FILE* coordinates;
    string name = "coordinates.txt";
    fopen_s(&coordinates, const_cast<char*>(name.c_str()), "wb+");*/

    /*for (int i = 0; i < countOfBodies; i++) {
        fwrite(&(countedX[i]), sizeof(double), 1, coordinates);
        fwrite(&(countedY[i]), sizeof(double), 1, coordinates);
    }*/

    for (int n = 1; n <= countOfSteps; n++) {
        #pragma omp parallel for
        for (int i = 0; i < countOfBodies; i++) {
            a1_x[i] = ax(countOfBodies, i, countedX, countedY, M);
            a1_y[i] = ay(countOfBodies, i, countedX, countedY, M);
        }
        multVector(countOfBodies, buf_x, a1_x, h / 2);
        multVector(countOfBodies, buf_y, a1_y, h / 2);
        addVector(countOfBodies, buf_x, buf_x, countedX);
        addVector(countOfBodies, buf_y, buf_y, countedY);
        #pragma omp parallel for
        for (int i = 0; i < countOfBodies; i++) {
            a2_x[i] = ax(countOfBodies, i, buf_x, buf_y, M);
            a2_y[i] = ay(countOfBodies, i, buf_x, buf_y, M);
        }
        multVector(countOfBodies, buf_x, a2_x, h / 2);
        multVector(countOfBodies, buf_y, a2_y, h / 2);
        addVector(countOfBodies, buf_x, buf_x, countedX);
        addVector(countOfBodies, buf_y, buf_y, countedY);
        #pragma omp parallel for
        for (int i = 0; i < countOfBodies; i++) {
            a3_x[i] = ax(countOfBodies, i, buf_x, buf_y, M);
            a3_y[i] = ay(countOfBodies, i, buf_x, buf_y, M);
        }
        multVector(countOfBodies, buf_x, a3_x, h);
        multVector(countOfBodies, buf_y, a3_y, h);
        addVector(countOfBodies, buf_x, buf_x, countedX);
        addVector(countOfBodies, buf_y, buf_y, countedY);
        #pragma omp parallel for
        for (int i = 0; i < countOfBodies; i++) {
            a4_x[i] = ax(countOfBodies, i, buf_x, buf_y, M);
            a4_y[i] = ay(countOfBodies, i, buf_x, buf_y, M);
        }
        #pragma omp parallel for
        for (int i = 0; i < countOfBodies; i++) {
            countedX[i] += (h / 6) * (6 * countedVx[i] + h * (a1_x[i] + a2_x[i] + a3_x[i]));
            countedY[i] += (h / 6) * (6 * countedVy[i] + h * (a1_y[i] + a2_y[i] + a3_y[i]));
        }
        #pragma omp parallel for
        for (int i = 0; i < countOfBodies; i++) {
            countedVx[i] += (h / 6) * (a1_x[i] + 2 * a2_x[i] + 2 * a3_x[i] + a4_x[i]);
            countedVy[i] += (h / 6) * (a1_y[i] + 2 * a2_y[i] + 2 * a3_y[i] + a4_y[i]);
        }
        /*for (int i = 0; i < countOfBodies; i++) {
            fwrite(&(countedX[i]), sizeof(double), 1, coordinates);
            fwrite(&(countedY[i]), sizeof(double), 1, coordinates);
        }*/
    }

    //fclose(coordinates);

    free(a1_x);
    free(a2_x);
    free(a3_x);
    free(a4_x);
    free(a1_y);
    free(a2_y);
    free(a3_y);
    free(a4_y);
    free(buf_x);
    free(buf_y);
}

double ax(int countOfBodies, int bodyInd, double* X, double* Y, double* M) {
    double result = 0;
    for (int i = 0; i < countOfBodies; i++) {
        if (i == bodyInd) continue;
        double dx = X[i] - X[bodyInd];
        double dy = Y[i] - Y[bodyInd];
        double r = sqrt(dx * dx + dy * dy);
        result += M[i] * dx / pow(r, 3);
    }
    return G * result;
}

double ay(int countOfBodies, int bodyInd, double* X, double* Y, double* M) {
    double result = 0;
    for (int i = 0; i < countOfBodies; i++) {
        if (i == bodyInd) continue;
        double dx = X[i] - X[bodyInd];
        double dy = Y[i] - Y[bodyInd];
        double r = sqrt(dx * dx + dy * dy);
        result += M[i] * dy / pow(r, 3);
    }
    return G * result;
}

void addVector(int countOfBodies, double* result, double* X1, double* X2) {
    #pragma omp parallel for
    for (int i = 0; i < countOfBodies; i++) {
        result[i] = X1[i] + X2[i];
    }
}

void copyVector(int countOfBodies, double* result, double* X) {
    #pragma omp parallel for
    for (int i = 0; i < countOfBodies; i++) {
        result[i] = X[i];
    }
}

void multVector(int countOfBodies, double* result, double* X, double coef) {
    #pragma omp parallel for
    for (int i = 0; i < countOfBodies; i++) {
        result[i] = coef * X[i];
    }
}