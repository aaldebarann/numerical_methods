#include <iostream>
#include <vector>
#include <cmath>


double f0(double x, double u) {
    return -u / 2.0;
}

double f0sol(double x, double u_0) {
    return exp(-x / 2 + log(u_0));
}

double f1(double x, double u) {
    return u * u / (1.0 + pow(x, 4))  + u - u * u * u * sin(10 * x);
}

double fv(double x, double u) {
    return 2 * sin(u);
}

double rk(double x, double u, double h, double (*f)(double, double)) {
    double k1, k2, k3, k4;

    k1 = f(x, u);
    k2 = f(x + h/2, u + h * k1 / 2.0);
    k3 = f(x + h/2, u + h * k2 / 2.0);
    k4 = f(x + h, u + h * k3);

    return u + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
}

double f2(double x, double u) {
    return rk(x, u, 0.01, fv);
}

double rkh(double x, double u, double h, double (*f)(double, double)) {
    double k1, k2, k3, k4;
    double hh = h / 2;

    k1 = f(x, u);
    k2 = f(x + hh / 2, u + hh * k1 / 2.0);
    k3 = f(x + hh / 2, u + hh * k2 / 2.0);
    k4 = f(x + hh, u + hh * k3);

    double u2 = u + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
    double x2 = x + hh;

    k1 = f(x2, u2);
    k2 = f(x2 + hh / 2, u2 + hh * k1 / 2.0);
    k3 = f(x2 + hh / 2, u2 + hh * k2 / 2.0);
    k4 = f(x2 + hh, u2 + hh * k3);

    return u2 + hh * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
}

double rkv(double x, double u, double& h, double& ele, double epsilon, int& C1, int& C2, double (*f)(double, double)) {
    double u1 = rk(x, u, h, f);
    double u2 = rkh(x, u, h, f);
    ele = abs(u1 - u2) / 15;
    if (ele > epsilon) {
        h /= 2;
        C1++;
        return rkv(x, u, h, ele, epsilon, C1, C2, f);
    }
    else if (ele < epsilon / 32) {
        h *= 2;
        C2++;
        return u1;
    }
    else return u1;
}