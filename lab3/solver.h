#pragma once
#include <math.h>
#include <vector>
#include <utility>


namespace u_real {
double u(double x, double y);

double u0y(double y);

double u1y(double y);

double ux0(double x);

double ux1(double x);

double f(double x, double y);
}

namespace u_kapkaev {
double u(double x, double y);

double u0y(double y);

double u1y(double y);

double ux0(double x);

double ux1(double x);

double f(double x, double y);
}

enum Functions {
    STRONGINA, KAPKAEV
};

class solver {

    double (*u0y)(double);
    double (*u1y)(double);
    double (*ux0)(double);
    double (*ux1)(double);
    double (*f)(double x, double y);
    double (*u)(double x, double y);

    inline int is_border(int i, int j);

    void fill_right_side(std::vector<std::vector<double>>& v, double a, double c);

    void calc_r(std::vector<std::vector<double>>& v);

    void prepare(std::vector<std::vector<double>>& v, std::vector<std::vector<double>>& z, double a, double c);

    void step(std::vector<std::vector<double>>& v, std::vector<std::vector<double>>& z, double a, double c, double& mz, double& acc) ;

    void copy(std::vector<std::vector<double>>& v1, std::vector<std::vector<double>>& z1, std::vector<std::vector<double>>& v2, std::vector<std::vector<double>>& z2);

public:

    int N, M, max_it, it, task;
    double x0, X, y0, Y, k, h, hor, ver, A, max_z, achieved_accuracy, max_r, epsilon;
    bool valid = 0;
    std::vector<double> right_side;

    solver();

    solver(int function);

    void solve(int n, int m, double a, double b, double c, double d, double eps, int m_it, std::vector<std::vector<std::vector<double>>>& v, std::vector<std::vector<std::vector<double>>>& z);
};

