#include <vector>
#include <cmath>
#include <string>
#include <iostream>

class Functions{
public:
    Functions()=default;
    static inline std::string type{"test"};
    static inline double c1 = 0.06055722286665054583;
    static inline double c2 = -1.0605572228666505458;
    static inline double c1_ = -0.47202455073443706013;
    static inline double c2_ = -4.3310848235800584475;


    static void set_task(std::string t){
        type = t;
    }
    static double k1(double x) {
        if(type == "test" ){
            return 1.4;
        }
        else{
            return x + 1;
        }
    }

    static double k1_(double x) {
        if(type == "test"){
            return 1 / 1.4;
        }
        else{
            return 1 / (x + 1);
        }
    }

    static double k2(double x) {
        if (type == "test"){
            return 0.4;
        }
        else{
            return x;
        }
    }

    static double k2_(double x) {
        if (type == "test"){
            return 1 / 0.4;
        }
        else{
            return 1 / x;
        }
    }

    static double q1(double x) {
        if (type == "test"){
            return 0.4;
        }
        else{
            return x;
        }
    }

    static double q2(double x) {
        if (type == "test"){
            return 0.16;
        }
        else{
            return x*x;
        }
    }

    static double f1(double x) {
        if (type == "test"){
            return 0.4;
        }
        else{
            return x;
        }
    }

    static double f2(double x) {
        if (type == "test"){
            return std::exp(-0.4);
        }
        else{
            return std::exp(-x);
        }
    }

    static double tr_f(double x){
        if (x == 0){
            return 0.0;
        }
        else if (x <= 0.4 && x >= 0){
            return (c1 * std::exp(std::sqrt(2.0 / 7.0) * x) + c2 * std::exp(-std::sqrt(2.0 / 7.0) * x) + 1.0);
        }
        else if (x > 0.4 && x <= 1.0){
            return (c1_ * std::exp(2.0 * x / std::sqrt(10.0)) + c2_ * std::exp(-2.0 * x / std::sqrt(10.0)) + std::exp(-0.4)/0.16);
        }
    }
};


class IntegralSolver{
public:
    IntegralSolver()=default;

    constexpr static double gap = 0.4;

    static double trapezoid_method(double (*func)(double x), const double& x_i, const double& x_i_1){
        return (x_i_1 - x_i) * (func(x_i) + func(x_i_1)) / 2.0;
    }


};

class Run_Coeffs{
public:
    Run_Coeffs(){};

   static double a_i(double x_i, double h) {
        if (x_i <= IntegralSolver::gap){
            return 1.0 / ((1.0 / h) * IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, x_i));
        }
        else if((x_i - h < IntegralSolver::gap) && (IntegralSolver::gap < x_i)){
            return 1.0 / ((1.0 / h) * (IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, IntegralSolver::gap) +
                                   IntegralSolver::trapezoid_method(Functions::k2_, IntegralSolver::gap, x_i)));
        }
        else if (x_i - h >= IntegralSolver::gap){
            return 1.0 / ((1.0 / h) * IntegralSolver::trapezoid_method(Functions::k2_, x_i - h, x_i));
        }
    }

    static double phi_i(double x_i, double h) {
        if (x_i + h/2.0 <= IntegralSolver::gap){
            return (1.0 / h) * IntegralSolver::trapezoid_method(Functions::f1, x_i - h/2.0, x_i + h/2.0);
        }
        else if((x_i - h/2.0 < IntegralSolver::gap) && (IntegralSolver::gap < x_i+h/2.0)){
            return (1.0 / h) * (IntegralSolver::trapezoid_method(Functions::f1, x_i - h/2.0, IntegralSolver::gap) +
                                   IntegralSolver::trapezoid_method(Functions::f2, IntegralSolver::gap, x_i + h/2.0));
        }
        else if(x_i - h / 2.0 >= IntegralSolver::gap){
            return (1.0 / h) * IntegralSolver::trapezoid_method(Functions::f2, x_i - h/2.0, x_i + h/2.0);
        }
    }

    static double d_i(double x_i, double h) {
        if (x_i + h/2.0 <= IntegralSolver::gap){
            return (1.0 / h) * IntegralSolver::trapezoid_method(Functions::q1, x_i - h / 2.0, x_i + h/2.0);
        }
        else if((x_i - h/2.0 < IntegralSolver::gap) && (IntegralSolver::gap < x_i + h/2.0)){
            return (1.0 / h) * (IntegralSolver::trapezoid_method(Functions::q1, x_i - h/ 2.0, IntegralSolver::gap) +
                              IntegralSolver::trapezoid_method(Functions::q2, IntegralSolver::gap, x_i + h / 2.0));
        }
        else if(x_i - h / 2.0 >= IntegralSolver::gap){
            return (1.0 / h) * IntegralSolver::trapezoid_method(Functions::q2, x_i - h / 2.0, x_i + h / 2.0);
        }
    }

    static double A_i(double x_i, double h) {
        return a_i(x_i, h) / (h * h);
    }

    static double B_i(double x_i, double h){
        return a_i(x_i + h, h) / (h * h);
    }

    static double C_i(double x_i, double h){
        return ((a_i(x_i, h) + a_i(x_i + h, h)) / (h * h)) + d_i(x_i, h);
    }
};

std::vector<double> run(int n, double ae1, double mu1, double ae2, double mu2, double x_0, double x_n);

void buildLES(int n, double x_0, double x_n, std::vector<double>& a, std::vector<double>& b, std::vector<double>& c, std::vector<double>& phi);
