#include <vector>
#include <cmath>
#include <string>
#include <iostream>

class Functions {
public:
    Functions() = default;
    static inline std::string type{ "test" };
    static inline long double c1 = std::strtold("0.060557222866650545786967205659300L", nullptr);
    static inline long double c2 = std::strtold("-1.06055722286665054578696720565930L", nullptr);
    static inline long double c1_ = std::strtold("-0.472024550734437060202137920065678L", nullptr);
    static inline long double c2_ = std::strtold("-4.33108482358005844728696970435680L", nullptr);


    static void set_task(std::string t) {
        type = t;
    }
    static long double k1(long double x) {
        if (type == "test") {
            return 1.4L;
        }
        else {
            return x + 1;
        }
    }

    static long double k1_(long double x) {
        if (type == "test") {
            return 1 / 1.4L;
        }
        else {
            return 1 / (x + 1);
        }
    }

    static long double k2(long double x) {
        if (type == "test") {
            return 0.4L;
        }
        else {
            return x;
        }
    }

    static long double k2_(long double x) {
        if (type == "test") {
            return 1 / 0.4L;
        }
        else {
            return 1 / x;
        }
    }

    static long double q1(long double x) {
        if (type == "test") {
            return 0.4L;
        }
        else {
            return x;
        }
    }

    static long double q2(long double x) {
        if (type == "test") {
            return 0.16L;
        }
        else {
            return x * x;
        }
    }

    static long double f1(long double x) {
        if (type == "test") {
            return 0.4L;
        }
        else {
            return x;
        }
    }

    static long double f2(long double x) {
        if (type == "test") {
            return std::exp(-0.4L);
        }
        else {
            return std::exp(-x);
        }
    }

    static long double tr_f(long double x) {
        if (x == 0) {
            return 0.0L;
        }
        else if (x <= 0.4L && x >= 0) {
            return (c1 * std::exp(std::sqrt(2.0L / 7.0L) * x) + c2 * std::exp(-std::sqrt(2.0L / 7.0L) * x) + 1.0L);
        }
        else if (x > 0.4L && x <= 1.0L) {
            return (c1_ * std::exp(2.0L * x / std::sqrt(10.0L)) + c2_ * std::exp(-2.0L * x / std::sqrt(10.0L)) + std::exp(-0.4L) / 0.16L);
        }
    }
};


class IntegralSolver {
public:
    IntegralSolver() = default;

    constexpr static long double gap = 0.4L;

    static long double trapezoid_method(long double (*func)(long double x), const long double& x_i, const long double& x_i_1) {
        return (x_i_1 - x_i) * (func(x_i) + func(x_i_1)) / 2.0L;
    }


};

class Run_Coeffs {
public:
    Run_Coeffs() {};

    static long double a_i(long double x_i, long double h) {
        if (x_i <= IntegralSolver::gap) {
            return 1.0L / ((1.0L / h) * IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, x_i));
        }
        else if ((x_i - h < IntegralSolver::gap) && (IntegralSolver::gap < x_i)) {
            return 1.0L / ((1.0L / h) * (IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, IntegralSolver::gap) +
                                       IntegralSolver::trapezoid_method(Functions::k2_, IntegralSolver::gap, x_i)));
        }
        else if (x_i - h >= IntegralSolver::gap) {
            return 1.0L / ((1.0L / h) * IntegralSolver::trapezoid_method(Functions::k2_, x_i - h, x_i));
        }
    }

    static long double phi_i(long double x_i, long double h) {
        if (x_i + h / 2.0L <= IntegralSolver::gap) {
            return (1.0L / h) * IntegralSolver::trapezoid_method(Functions::f1, x_i - h / 2.0L, x_i + h / 2.0L);
        }
        else if ((x_i - h / 2.0L < IntegralSolver::gap) && (IntegralSolver::gap < x_i + h / 2.0L)) {
            return (1.0L / h) * (IntegralSolver::trapezoid_method(Functions::f1, x_i - h / 2.0L, IntegralSolver::gap) +
                                IntegralSolver::trapezoid_method(Functions::f2, IntegralSolver::gap, x_i + h / 2.0L));
        }
        else if (x_i - h / 2.0L >= IntegralSolver::gap) {
            return (1.0L / h) * IntegralSolver::trapezoid_method(Functions::f2, x_i - h / 2.0L, x_i + h / 2.0L);
        }
    }

    static long double d_i(long double x_i, long double h) {
        if (x_i + h / 2.0L <= IntegralSolver::gap) {
            return (1.0L / h) * IntegralSolver::trapezoid_method(Functions::q1, x_i - h / 2.0L, x_i + h / 2.0L);
        }
        else if ((x_i - h / 2.0L < IntegralSolver::gap) && (IntegralSolver::gap < x_i + h / 2.0L)) {
            return (1.0L / h) * (IntegralSolver::trapezoid_method(Functions::q1, x_i - h / 2.0L, IntegralSolver::gap) +
                                IntegralSolver::trapezoid_method(Functions::q2, IntegralSolver::gap, x_i + h / 2.0L));
        }
        else if (x_i - h / 2.0L >= IntegralSolver::gap) {
            return (1.0L / h) * IntegralSolver::trapezoid_method(Functions::q2, x_i - h / 2.0L, x_i + h / 2.0L);
        }
    }

    static long double A_i(long double x_i, long double h) {
        return a_i(x_i, h) / (h * h);
    }

    static long double B_i(long double x_i, long double h) {
        return a_i(x_i + h, h) / (h * h);
    }

    static long double C_i(long double x_i, long double h) {
        return ((a_i(x_i, h) + a_i(x_i + h, h)) / (h * h)) + d_i(x_i, h);
    }
};

void run(long double ae1, long double mu1, long double ae2, long double mu2, long double x_0, long double x_n, std::vector<long double>& a, std::vector<long double>& b, std::vector<long double>& c, std::vector<long double>& phi, std::vector<long double>& result);

void buildLES(int n, long double x_0, long double x_n, std::vector<long double>& a, std::vector<long double>& b, std::vector<long double>& c, std::vector<long double>& phi);
