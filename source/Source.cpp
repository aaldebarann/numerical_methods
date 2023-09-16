#include "rk.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

static double p = 2;
int main()
{
    double a = 0;
    double b = 10;
    int Nmax = 10000;
    double x = a;
    double u = 1.0;
    double u_0 = 1.0;
    double h = 0.01;
    double epsilon = 0.01;
    double u1 = u;
    double u2 = u;
    double ue = 0;
    double ele = 0;
    int C1 = 0;
    int C2 = 0;

    std::ofstream outputFile("results.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    for (int i = 0; x <= b && i < Nmax; i++) {
        u1 = rk(x, u, h, f0);
        u2 = rkh(x, u, h, f0);
        ue = f0sol(x + h, u_0);
        ele = abs(u1 - u2)/15;
        outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << abs(u1 - u2) << "," << ele << "," << h << "," << C1 << "," << C2 << "," << ue << "," << abs(ue-u1) << "\n";
        x += h;
        u = u1;
    }

    x = a;
    ele = C1 = C2 = 0;
    u = 1;
    h = 0.01;

    for (int i = 0; x <= b && i < Nmax; i++) {
        u1 = rkv(x, u, h, ele, epsilon, C1, C2, f1);
        u2 = rkh(x, u, h, f1);
        outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << abs(u1 - u2) << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
        x += h;
        u = u1;
    }

    x = a;
    ele = C1 = C2 = 0;
    u = 1;
    h = 0.01;
    double v = 1;
    double v1 = v;
    double v2 = v;
    double vepsilon = 0.01;
    double vele = 0;
    int vC1 = 0;
    int vC2 = 0;
    double vh = 0.01;

    for (int i = 0; x <= b && i < Nmax; i++) {
        v1 = rkv(x, v, vh, vele, vepsilon, vC1,vC2, fv);
        v2 = rkh(x, v, vh, fv);

        u1 = rkv(x, u, h, ele, epsilon, C1, C2, f2);
        u2 = rkh(x, u, h, f2);

        outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << abs(u1 - u2) << "," << ele << "," << h << "," << C1 << "," << C2 << "," << v1 << "," << v2 << "," << abs(v1-v2) << "," << vele << "," << vh << "," << vC1 << "," << vC2 << "," << "\n";
        x += h;
        v = v1;
        u = u1;
    }

    outputFile.close();
    return 0;
}