#include"solver.h"

//a, b, c, phi: n - 2; result: n;
void run(high_precision_type ae1, high_precision_type mu1, high_precision_type ae2, high_precision_type mu2, high_precision_type x_0, high_precision_type x_n, std::vector<high_precision_type>& a, std::vector<high_precision_type>& b, std::vector<high_precision_type>& c, std::vector<high_precision_type>& phi, std::vector<high_precision_type>& result) {

    std::vector<high_precision_type> alpha;
    std::vector<high_precision_type> beta;

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

void buildLES(int n, high_precision_type x_0, high_precision_type x_n, std::vector<high_precision_type>& a, std::vector<high_precision_type>& b, std::vector<high_precision_type>& c, std::vector<high_precision_type>& phi) {
    high_precision_type h = (x_n - x_0) / (high_precision_type)(n - 1);
    for (int i = 1; i < n - 1; i++) {
        high_precision_type x_i = x_0 + i * h;
        a.push_back(Run_Coeffs::A_i(x_i, h));
        b.push_back(Run_Coeffs::B_i(x_i, h));
        c.push_back(Run_Coeffs::C_i(x_i, h));
        phi.push_back(Run_Coeffs::phi_i(x_i, h));
    }
}
