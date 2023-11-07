#include"solver.h"

//a, b, c, phi: n - 2; result: n;
std::vector<double> run(int n, double ae1, double mu1, double ae2, double mu2, double x_0, double x_n) {
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> phi;

    double h = (x_n - x_0) / (double)n;
    for (int i = 1; i < n; i++) {
        double x_i = x_0 + i * h;
        a.push_back(Run_Coeffs::A_i(x_i, h));
        b.push_back(Run_Coeffs::B_i(x_i, h));
        c.push_back(Run_Coeffs::C_i(x_i, h));
        phi.push_back(Run_Coeffs::phi_i(x_i, h));
    }


    std::vector<double> alpha;
    std::vector<double> beta;
    std::vector<double> result;

    alpha.push_back(ae1);
    beta.push_back(mu1);

    for (int i = 0; i < phi.size(); i++) {
        alpha.push_back(b[i] / (c[i] - a[i] * alpha[i]));
        beta.push_back((phi[i] + a[i]*beta[i]) / (c[i] - alpha[i] * a[i]));
    }
    n = alpha.size();
    result.resize(n + 1);
    result[n] = (mu2 + ae2 * beta[n]) / (1.0 - alpha[n]);
    for (int i = n - 1; i >= 0; i--) {
        result[i] = alpha[i] * result[i + 1] + beta[i];
    }
    return result;
}

void buildLES(int n, double x_0, double x_n, std::vector<double>& a, std::vector<double>& b, std::vector<double>& c, std::vector<double>& phi) {
    double h = (x_n - x_0) / (double)n;
    for (int i = 1; i < n; i++) {
        double x_i = x_0 + i * h;
        a.push_back(Run_Coeffs::A_i(x_i, h));
        b.push_back(Run_Coeffs::B_i(x_i, h));
        c.push_back(Run_Coeffs::C_i(x_i, h));
        phi.push_back(Run_Coeffs::phi_i(x_i, h));
    }
}