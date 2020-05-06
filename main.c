#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OUTPUT_FILE "../output/output.txt"
#define INPUT_FILE "../input/input.txt"

#define SIZE 3 // розмір масива структур

struct ResistancePower {
    double T;
    double t;
    double p;
    double A;
    double v0;
    double u;
    double d;
};

struct ResistancePower *getResistancePower();

double getS(struct ResistancePower rp);

double *getV(struct ResistancePower rp);

double *getR(struct ResistancePower rp);

double *getQ(struct ResistancePower rp);

double *getF(struct ResistancePower rp);

void writeResistancePowerToFile(struct ResistancePower rp, int structPos);

int main() {
    struct ResistancePower *rp = getResistancePower();
    for (int i = 0; i < SIZE; ++i) {
        writeResistancePowerToFile(rp[i], i + 1);
    }
    return 0;
}

struct ResistancePower *getResistancePower() {
    struct ResistancePower *rp;
    rp = calloc(SIZE, sizeof(struct ResistancePower));
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

double getS(struct ResistancePower rp) {
    return M_PI * pow((rp.d / 2), 2);
}

double *getV(struct ResistancePower rp) {
    int arraySize = (int) rp.T / rp.t + 1;
    double *V = calloc(arraySize, sizeof(double));
    double tPos = 0;
    int i = 0;
    while (tPos <= rp.T) {
        if (tPos >= 0 && tPos < rp.T / 4) {
            V[i] = rp.v0 + 4 * rp.A * tPos / rp.T;
        }
        if (tPos >= rp.T / 4 && tPos < 3 * rp.T / 4) {
            V[i] = rp.v0 + rp.A;
        }
        if (tPos >= 3 * rp.T / 4 && tPos <= rp.T) {
            V[i] = rp.v0 + rp.A - (tPos - 3. / 4 * rp.T) * (4 * rp.A) / rp.T;
        }
        tPos += rp.t;
        i++;
    }
    return V;
}

double *getR(struct ResistancePower rp) {
    double *V = getV(rp);
    int arraySize = (int) rp.T / rp.t + 1;
    double *R = calloc(arraySize, sizeof(double));
    for (int i = 0; i < arraySize; ++i) {
        R[i] = V[i] * rp.d * rp.p / rp.u;
    }
    return R;
}

double *getQ(struct ResistancePower rp) {
    double *R = getR(rp);
    int arraySize = (int) rp.T / rp.t + 1;
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

double *getF(struct ResistancePower rp) {
    double *Q = getQ(rp);
    double *V = getV(rp);
    double S = getS(rp);
    int arraySize = (int) rp.T / rp.t + 1;
    double *F = calloc(arraySize, sizeof(double));
    for (int i = 0; i < arraySize; ++i) {
        F[i] = Q[i] * S * rp.p * pow(V[i], 2) / 2;
    }
    return F;
}

void writeResistancePowerToFile(struct ResistancePower rp, int structPos) {
    int arraySize = (int) rp.T / rp.t + 1;
    double *F = getF(rp);
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