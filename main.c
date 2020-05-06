#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OUTPUT_FILE "../output/output.txt"
#define INPUT_FILE "../input/input.txt"

#define SIZE 3 // розмір масива структур

struct resistancePower {
    double T;
    double t;
    double p;
    double A;
    double v0;
    double u;
    double d;
};

struct resistancePower *getResistancePower();

double getS(double d);

double *getV(double T, int arraySize, double t, double A, double v0);

double *getR(double *V, int arraySize, double d, double p, double u);

double *getQ(double *R, int arraySize);

double *getF(double *Q, double *V, int arraySize, double p, double S);

void writeResistancePowerToFile(double *F, int arraySize, int structPos);

int main() {
    struct resistancePower *rp = getResistancePower();
    for (int i = 0; i < SIZE; ++i) {
        int arraySize = (int) rp[i].T / rp[i].t + 1;
        double curS = getS(rp[i].d);
        double *curV = getV(rp[i].T, arraySize, rp[i].t, rp[i].A, rp[i].v0);
        double *curR = getR(curV, arraySize, rp[i].d, rp[i].p, rp[i].u);
        double *curQ = getQ(curR, arraySize);
        double *curF = getF(curQ, curV, arraySize, rp[i].p, curS);
        writeResistancePowerToFile(curF, arraySize, i + 1);
    }
    return 0;
}

struct resistancePower *getResistancePower() {
    struct resistancePower *rp;
    rp = calloc(SIZE, sizeof(struct resistancePower));
    FILE *input;
    input = fopen(INPUT_FILE, "r");
    if (input == 0) {
        printf("Can not find a file\n");
        exit(-1);
    }
    int i = 0;
    while (fscanf(input, "%lf %lf %lf %lf %lf %lf %lf", &rp[i].T, &rp[i].t, &rp[i].p, &rp[i].A, &rp[i].v0, &rp[i].u,
                  &rp[i].d) != EOF) {
        printf("Struct %d", i + 1);
        printf("\n%.0lf %.0lf %.2lf %.0lf %.0lf %.7lf %.3lf\n", rp[i].T, rp[i].t, rp[i].p, rp[i].A, rp[i].v0, rp[i].u,
               rp[i].d);
        i++;
    }
    return rp;
}

double getS(double d) {
    return M_PI * pow((d / 2), 2);
}

double *getV(double T, int arraySize, double t, double A, double v0) {
    double *V = calloc(arraySize, sizeof(double));
    double deltaT = t;
    t = 0;
    int i = 0;
    while (t <= T) {
        if (t >= 0 && t < T / 4) {
            V[i] = v0 + 4 * A * t / T;
        }
        if (t >= T / 4 && t < 3 * T / 4) {
            V[i] = v0 + A;
        }
        if (t >= 3 * T / 4 && t <= T) {
            V[i] = v0 + A - (t - 3. / 4 * T) * (4 * A) / T;
        }
        t += deltaT;
        i++;
    }
    return V;
}

double *getR(double *V, int arraySize, double d, double p, double u) {
    double *R = calloc(arraySize, sizeof(double));
    for (int i = 0; i < arraySize; ++i) {
        R[i] = V[i] * d * p / u;
    }
    return R;
}

double *getQ(double *R, int arraySize) {
    double *Q = calloc(arraySize, sizeof(double));
    for (int i = 0; i < arraySize; ++i) {
        if (R[i] <= 2) {
            Q[i] = 24 / R[i];
        }
        if (R[i] > 2 && R[i] <= 500) {
            Q[i] = 18.5 / pow(R[i], 0.6);
        }
        if (R[i] > 500 && R[i] <= 2 * pow(10, 5)) {
            Q[i] = 0.44;
        }
    }
    return Q;
}

double *getF(double *Q, double *V, int arraySize, double p, double S) {
    double *F = calloc(arraySize, sizeof(double));
    for (int i = 0; i < arraySize; ++i) {
        F[i] = Q[i] * S * p * pow(V[i], 2) / 2;
    }
    return F;
}

void writeResistancePowerToFile(double *F, int arraySize, int structPos) {
    FILE *output;
    output = fopen(OUTPUT_FILE, "a+");
    printf("\n\nResistance power (F) from struct #%d:\n", structPos);
    fprintf(output, "ResistancePower (F) from struct #%d:\n", structPos);
    for (int i = 0; i < arraySize; ++i) {
        printf("%.8lf, ", F[i]);
        fprintf(output, "%.8lf ", F[i]);
    }
    fprintf(output, "\n\n\n");

    fclose(output);
}