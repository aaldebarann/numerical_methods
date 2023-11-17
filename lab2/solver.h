#pragma once

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <boost/multiprecision/cpp_dec_float.hpp>


namespace mp = boost::multiprecision;
using high_precision_type = mp::cpp_dec_float_50;

class Functions {
public:
    Functions() = default;
    static inline std::string type{ "test" };
    static inline high_precision_type c1 = high_precision_type("0.06055722286665054578696720565930001500930596148855");
    static inline high_precision_type c2 = high_precision_type("-1.0605572228666505457869672056593000150093059614886");
    static inline high_precision_type c1_ = high_precision_type("-0.4720245507344370602021379200656781375924107069915");
    static inline high_precision_type c2_ = high_precision_type("-4.3310848235800584472869697043568015372999456023843");

    static void set_task(std::string t) {
        type = t;
    }
    static high_precision_type k1(high_precision_type x) {
        if (type == "test") {
            return high_precision_type("1.4");
        }
        else {
            return x + high_precision_type("1.0");
        }
    }

    static high_precision_type k1_(high_precision_type x) {
        if (type == "test") {
            return high_precision_type("1.0") / high_precision_type("1.4");
        }
        else {
            return high_precision_type("1.0") / (x + high_precision_type("1.0"));
        }
    }

    static high_precision_type k2(high_precision_type x) {
        if (type == "test") {
            return high_precision_type("0.4");
        }
        else {
            return x;
        }
    }

    static high_precision_type k2_(high_precision_type x) {
        if (type == "test") {
            return high_precision_type("1") / high_precision_type("0.4");
        }
        else {
            return high_precision_type("1.0") / x;
        }
    }

    static high_precision_type q1(high_precision_type x) {
        if (type == "test") {
            return high_precision_type("0.4");
        }
        else {
            return x;
        }
    }

    static high_precision_type q2(high_precision_type x) {
        if (type == "test") {
            return high_precision_type("0.16");
        }
        else {
            return x * x;
        }
    }

    static high_precision_type f1(high_precision_type x) {
        if (type == "test") {
            return high_precision_type("0.4");
        }
        else {
            return x;
        }
    }

    static high_precision_type f2(high_precision_type x) {
        if (type == "test") {
            return mp::exp(mp::cpp_dec_float_50("-0.4"));
        } else {
            return mp::exp(x);
        }
    }

    static high_precision_type tr_f(high_precision_type x) {
        if (x == 0) {
            return high_precision_type("0.0");
        }
        else if (x <= high_precision_type("0.4") && x >= 0) {
            return (c1 * mp::exp(mp::sqrt(high_precision_type(high_precision_type("2.0") / high_precision_type("7.0"))) * x) +
                   c2 * mp::exp(-mp::sqrt(high_precision_type(high_precision_type("2.0") / high_precision_type("7.0"))) * x) + high_precision_type("1.0"));
        }
        else if (x > high_precision_type("0.4") && x <= high_precision_type("1.0")) {
            return (c1_ * mp::exp(high_precision_type(high_precision_type("2.0") * x / mp::sqrt(high_precision_type("10.0")))) +
                    c2_ * mp::exp(high_precision_type(high_precision_type("-2.0") * x / mp::sqrt(high_precision_type("10.0")))) +
                    mp::exp(high_precision_type(high_precision_type("-0.4"))) / high_precision_type("0.16"));
        }
    }
};

class IntegralSolver {
public:
    IntegralSolver() = default;


    static high_precision_type trapezoid_method(high_precision_type (*func)(high_precision_type x), const high_precision_type& x_i, const high_precision_type& x_i_1) {
        return (x_i_1 - x_i) * (func(x_i) + func(x_i_1)) / high_precision_type(2.0);
    }
};

class Run_Coeffs {
public:
    Run_Coeffs() {};

    static high_precision_type a_i(high_precision_type x_i, high_precision_type h) {
        if (x_i <= high_precision_type("0.4")) {
            return high_precision_type("1.0") / ((high_precision_type("1.0") / h) * IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, x_i));
        }
        else if ((x_i - h < high_precision_type("0.4")) && (high_precision_type("0.4") < x_i)) {
            return high_precision_type("1.0") / ((high_precision_type("1.0") / h) * (IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, high_precision_type("0.4")) +
                                       IntegralSolver::trapezoid_method(Functions::k2_, high_precision_type("0.4"), x_i)));
        }
        else if (x_i - h >= high_precision_type("0.4")) {
            return high_precision_type("1.0") / ((high_precision_type("1.0") / h) * IntegralSolver::trapezoid_method(Functions::k2_, x_i - h, x_i));
        }
    }

    static high_precision_type phi_i(high_precision_type x_i, high_precision_type h) {
        if (x_i + h / high_precision_type("2.0") <= high_precision_type("0.4")) {
            return (high_precision_type("1.0") / h) * IntegralSolver::trapezoid_method(Functions::f1, x_i - h / high_precision_type("2.0"), x_i + h / high_precision_type("2.0"));
        }
        else if ((x_i - h / high_precision_type("2.0") < high_precision_type("0.4")) && (high_precision_type("0.4") < x_i + h / high_precision_type("2.0"))) {
            return (high_precision_type("1.0") / h) * (IntegralSolver::trapezoid_method(Functions::f1, x_i - h / high_precision_type("2.0"), high_precision_type("0.4")) +
                                IntegralSolver::trapezoid_method(Functions::f2, high_precision_type("0.4"), x_i + h / high_precision_type("2.0")));
        }
        else if (x_i - h / high_precision_type("2.0") >= high_precision_type("0.4")) {
            return (high_precision_type("1.0") / h) * IntegralSolver::trapezoid_method(Functions::f2, x_i - h / high_precision_type("2.0"), x_i + h / high_precision_type("2.0"));
        }
    }

    static high_precision_type d_i(high_precision_type x_i, high_precision_type h) {
        if (x_i + h / high_precision_type("2.0") <= high_precision_type("0.4")) {
            return (high_precision_type("1.0") / h) * IntegralSolver::trapezoid_method(Functions::q1, x_i - h / high_precision_type("2.0"), x_i + h / high_precision_type("2.0"));
        }
        else if ((x_i - h / high_precision_type("2.0") < high_precision_type("0.4")) && (high_precision_type("0.4") < x_i + h / high_precision_type("2.0"))) {
            return (high_precision_type("1.0") / h) * (IntegralSolver::trapezoid_method(Functions::q1, x_i - h / high_precision_type("2.0"), high_precision_type("0.4")) +
                                IntegralSolver::trapezoid_method(Functions::q2, high_precision_type("0.4"), x_i + h / high_precision_type("2.0")));
        }
        else if (x_i - h / high_precision_type("2.0") >= high_precision_type("0.4")) {
            return (high_precision_type("1.0") / h) * IntegralSolver::trapezoid_method(Functions::q2, x_i - h / high_precision_type("2.0"), x_i + h / high_precision_type("2.0"));
        }
    }

    static high_precision_type A_i(high_precision_type x_i, high_precision_type h) {
        return a_i(x_i, h) / (h * h);
    }

    static high_precision_type B_i(high_precision_type x_i, high_precision_type h) {
        return a_i(x_i + h, h) / (h * h);
    }

    static high_precision_type C_i(high_precision_type x_i, high_precision_type h) {
        return ((a_i(x_i, h) + a_i(x_i + h, h)) / (h * h)) + d_i(x_i, h);
    }
};

void run(high_precision_type ae1, high_precision_type mu1, high_precision_type ae2, high_precision_type mu2, high_precision_type x_0, high_precision_type x_n, std::vector<high_precision_type>& a, std::vector<high_precision_type>& b, std::vector<high_precision_type>& c, std::vector<high_precision_type>& phi, std::vector<high_precision_type>& result);

void buildLES(int n, high_precision_type x_0, high_precision_type x_n, std::vector<high_precision_type>& a, std::vector<high_precision_type>& b, std::vector<high_precision_type>& c, std::vector<high_precision_type>& phi);
