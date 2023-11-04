#include <vector>
#include <cmath>
#include <string>

class Functions{
public:
    Functions()=default;
    static inline std::string type{"test"};
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
            return 1 / 0.4;
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
};


class IntegralSolver{
public:
    IntegralSolver()=default;

    constexpr static double gap = 0.4;

    static double trapezoid_method(double (*func)(double x), const double& x_i, const double& x_i_1){
        return (x_i_1 - x_i) * (func(x_i) + func(x_i_1)) / 2;
    }


};

class Run_Coeffs{
public:
    Run_Coeffs(){};

   static double a_i(const double& x_i, const double& h) {
        if (x_i + h <= IntegralSolver::gap){
            return 1 / ((1 / h) * IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, x_i + h));
        }
        else if((x_i - h < IntegralSolver::gap) && (IntegralSolver::gap < x_i + h)){
            return 1 / ((1 / h) * (IntegralSolver::trapezoid_method(Functions::k1_, x_i - h, IntegralSolver::gap) +
                                   IntegralSolver::trapezoid_method(Functions::k2_, IntegralSolver::gap, x_i + h)));
        }
        else{
            return 1 / ((1 / h) * IntegralSolver::trapezoid_method(Functions::k2_, x_i - h, x_i + h));
        }
    }

    static double phi_i(const double& x_i, const double& h) {
        if (x_i + h/2 <= IntegralSolver::gap){
            return (1 / h) * IntegralSolver::trapezoid_method(Functions::f1, x_i - h/2, x_i + h/2);
        }
        else if((x_i - h/2 < IntegralSolver::gap) && (IntegralSolver::gap < x_i+h/2)){
            return (1 / h) * (IntegralSolver::trapezoid_method(Functions::f1, x_i - h/2, IntegralSolver::gap) +
                                   IntegralSolver::trapezoid_method(Functions::f2, IntegralSolver::gap, x_i + h/2));
        }
        else{
            return (1 / h) * IntegralSolver::trapezoid_method(Functions::f2, x_i - h/2, x_i + h/2);
        }
    }

    static double d_i(const double& x_i, const double& h) {
        if (x_i + h/2 <= IntegralSolver::gap){
            return (1 / h) * IntegralSolver::trapezoid_method(Functions::q1, x_i - h / 2, x_i + h/2);
        }
        else if((x_i - h/2 < IntegralSolver::gap) && (IntegralSolver::gap < x_i + h/2)){
            return (1 / h) * (IntegralSolver::trapezoid_method(Functions::q1, x_i - h/ 2, IntegralSolver::gap) +
                              IntegralSolver::trapezoid_method(Functions::q2, IntegralSolver::gap, x_i + h / 2));
        }
        else{
            return (1 / h) * IntegralSolver::trapezoid_method(Functions::q2, x_i - h / 2, x_i + h / 2);
        }
    }

    static double A_i(const double& x_i, const double& h) {
        return a_i(x_i, h) / (h * h);
    }

    static double B_i(const double& x_i, const double& h){
        return a_i(x_i + h, h) / (h * h);
    }

    static double C_i(const double& x_i, const double& h){
        return ((a_i(x_i, h) + a_i(x_i + h, h)) / (h * h)) + d_i(x_i, h);
    }
};

void run(double ae1, double mu1, double ae2, double mu2, std::vector<double>& a, std::vector<double>& b, std::vector<double>& c, std::vector<double>& phi, std::vector<double>& result);

void buildLES(int n, double& x_0, double& x_n, std::vector<double>& a, std::vector<double>& b, std::vector<double>& c, std::vector<double>& phi);
