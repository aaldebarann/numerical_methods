#include"solver.h"

//a, b, c, phi: n - 2; result: n;
void run(long double ae1, long double mu1, long double ae2, long double mu2, long double x_0, long double x_n, std::vector<long double>& a, std::vector<long double>& b, std::vector<long double>& c, std::vector<long double>& phi, std::vector<long double>& result) {

    std::vector<long double> alpha;
    std::vector<long double> beta;

    alpha.push_back(ae1);
    beta.push_back(mu1);

    for (int i = 0; i < phi.size(); i++) {
        alpha.push_back(b[i] / (c[i] - a[i] * alpha[i]));
        beta.push_back((phi[i] + a[i] * beta[i]) / (c[i] - alpha[i] * a[i]));
    }

    int n = alpha.size();

    result[n] = (mu2 + ae2 * beta[n - 1]) / (1.0 - alpha[n - 1] * ae2);

    for (int i = n - 1; i >= 0; i--) {
        result[i] = alpha[i] * result[i + 1] + beta[i];
    }
}

void buildLES(int n, long double x_0, long double x_n, std::vector<long double>& a, std::vector<long double>& b, std::vector<long double>& c, std::vector<long double>& phi) {
    long double h = (x_n - x_0) / (long double)(n - 1);
    for (int i = 1; i < n - 1; i++) {
        long double x_i = x_0 + i * h;
        a.push_back(Run_Coeffs::A_i(x_i, h));
        b.push_back(Run_Coeffs::B_i(x_i, h));
        c.push_back(Run_Coeffs::C_i(x_i, h));
        phi.push_back(Run_Coeffs::phi_i(x_i, h));
    }
}
