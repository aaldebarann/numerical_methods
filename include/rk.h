#include <iostream>
#include <vector>
#include <cmath>


std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size()) {
        throw std::runtime_error("Vector sizes do not match for addition.");
    }

    std::vector<double> result(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

std::vector<double> operator+(const std::vector<double>& v1, const double& r) {
    std::vector<double> result(v1.size());
    for (size_t i = 0; i < v1.size(); i++) {
        result[i] = v1[i] + r;
    }
    return result;
}

std::vector<double> operator*(const std::vector<double>& v, double scalar) {
    std::vector<double> result(v.size());
    for (size_t i = 0; i < v.size(); i++) {
        result[i] = v[i] * scalar;
    }
    return result;
}

double f0(double p, double x, double u) {
    return -u / 2.0;
}

double f0sol(double x, double u_0) {
    return exp(-x / 2 + log(u_0));
}

double f1(double p, double x, double u) {
    return u * u / (1.0 + pow(x, 4))  + u - u * u * u * sin(10 * x);
}

std::vector<double> f2(double p, double x, std::vector<double> u) {
    std::vector<double> result(2);
    result[0] =  - p * sin(u[1]);
    result[1] =  u[0];
    return result;
}

double rk(double& p, double& x, double& u, double& h, double (*f)(double, double, double)) {
    double k1, k2, k3, k4;

    k1 = f(p, x, u);
    k2 = f(p, x + h / 2.0, u + h * k1 / 2.0);
    k3 = f(p, x + h / 2.0, u + h * k2 / 2.0);
    k4 = f(p, x + h, u + h * k3);

    return u + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
}

double rkh(double& p, double& x, double& u, double& h, double (*f)(double, double, double)) {
    double hh = h / 2.0;
    double u1 = rk(p, x, u, hh, f);
    double x1 = x + hh;
    return rk(p, x1, u1, hh, f);
}

double rkv(double& p, double& x, double& u, double& h, double& ele, double& epsilon, int& C1, int& C2, double (*f)(double, double, double)) {
    double u1 = rk(p, x, u, h, f);
    double u2 = rkh(p, x, u, h, f);
    ele = abs(u2 - u1) / 15;
    if (ele > epsilon) {
        h /= 2;
        C1++;
        return rkv(p, x, u, h, ele, epsilon, C1, C2, f);
    }
    else if (ele < epsilon / 32) {
        h *= 2;
        C2++;
        ele *= 16;
        return u1;
    }
    else {
        ele *= 16;
        return u1;
    }
}

std::vector<double> rk(double& p, double& x, std::vector<double>& u, double& h, std::vector<double> (*f)(double, double, std::vector<double>)) {
    std::vector<double> k1, k2, k3, k4;

    k1 = f(p, x, u);
    k2 = f(p, x + h / 2, u + k1 * (h / 2.0));
    k3 = f(p, x + h / 2, u + k2 * (h / 2.0));
    k4 = f(p, x + h, u + k3 * h);

    return u + (k1 + k2 * 2 + k3 * 2 + k4) * (h / 6.0); 
}

std::vector<double> rkh(double& p, double& x, std::vector<double>& u, double& h, std::vector<double>(*f)(double, double, std::vector<double>)) {
    double hh = h / 2;
    std::vector<double> u1 = rk(p, x, u, hh, f);
    double x1 = x + hh;
    return rk(p, x1, u1, hh, f);
}

std::vector<double> rkv(double& p, double& x, std::vector<double>& u, double& h, std::vector<double>& ele, double& epsilon, int& C1, int& C2, std::vector<double>(*f)(double, double, std::vector<double>)) {
    std::vector<double> u1 = rk(p, x, u, h, f);
    std::vector<double> u2 = rkh(p, x, u, h, f);
    double dele = 0;
    for (int i = 0; i < u1.size(); i++) {
        ele[i] = abs(u2[i] - u1[i]) / 15;
        if (dele < ele[i]) dele = ele[i];
    }
    if (dele > epsilon) {
        h /= 2;
        C1++;
        return rkv(p, x, u, h, ele, epsilon, C1, C2, f);
    }
    else if (dele < epsilon / 32) {
        h *= 2;
        C2++;
        ele = ele * 16;
        return u1;
    }
    else {
        ele = ele * 16;
        return u1;
    }
}