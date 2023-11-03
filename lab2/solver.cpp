#include"solver.h"

//a, b, c, phi: n - 2; result: n;
void run(double ae1, double mu1, double ae2, double mu2, std::vector<double>& a, std::vector<double>& b, std::vector<double>& c, std::vector<double>& phi, std::vector<double>& result) {
    std::vector<double> alpha;
    std::vector<double> beta;

    alpha.push_back(ae1);
    beta.push_back(mu1);

    for (int i = 0; i < phi.size(); i++) {
        alpha.push_back(b[i] / (c[i] - a[i] * alpha[i]));
        beta.push_back((phi[i] + a[i]) / (c[i] - alpha[i] * a[i]));
    }
    int n = alpha.size();

    result[n] = (mu2 + ae2 * beta[n - 1]) / (1.0 - alpha[n - 1]);
    for (int i = n - 1; i >= 0; i--) {
        result[i] = alpha[i] * result[i + 1] + beta[i];
    }
}

void buildLES(int n, double& ae1, double& mu1, double& ae2, double& mu2, std::vector<double>& a, std::vector<double>& b, std::vector<double>& c, std::vector<double>& phi) {
    /*
    составление схемы
    */
}