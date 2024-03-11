#ifndef SOLVER_H
#define SOLVER_H
#include "functions.h"
#include <vector>


class solver {
public:
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> d;
    int n;
    bool valid;
    double prev_bord_a;
    double prev_bord_b;

    solver() {
        valid = false;
    }

    void solve(double mu1, double mu2, int N) {
        valid = true;
        n = N;
        int next_n = n;
        prev_bord_a = mu1;
        prev_bord_b = mu2;
        n = next_n;
        double h = (Functions::b - Functions::a) / n;
        std::vector<double> phi(n);
        a = std::vector<double>(n + 1);
        b = std::vector<double>(n + 1);
        c = std::vector<double>(n + 1);
        d = std::vector<double>(n + 1);

        for (int i = 1; i < n; i++) {
            phi[i] = -6.0 * (Functions::f(Functions::a + h * (double)(i + 1)) - 2.0 * Functions::f(Functions::a + h * (double)(i)) + Functions::f(Functions::a + h * (double)(i - 1))) / h;
        }

        run(mu1, mu2, phi);

        for (int i = 1; i < n + 1; i++) {
            a[i] = Functions::f(Functions::a + h * (double)i);
            d[i] = (c[i] - c[i - 1]) / h;
            b[i] = ((Functions::f(Functions::a + h * (double)i) - Functions::f(Functions::a + h * (double)(i - 1))) / h)
                   + c[i] * h / 3.0
                   + c[i - 1] * h / 6.0;
        }
        next_n *= 2;
    }

    double get_error() {
        int k = 2 * n + 1;
        double max_sub = 0;
        double max_sub_x = 0;
        double h = (Functions::b - Functions::a) / k;
        double x = Functions::a;
        for (int i = 1; i <= k; i++) {
            x += h;
            if (abs(Functions::f(x) - S(x)) > max_sub) {
                max_sub = abs(Functions::f(x) - S(x));
                max_sub_x = x;
            }
        }
        return max_sub;
    }

    double get_der_error() {
        int k = 2 * n + 1;
        double max_der_sub = 0;
        double max_der_sub_x = 0;
        double h = (Functions::b - Functions::a) / k;
        double x = Functions::a;
        for (int i = 1; i <= k; i++) {
            x += h;
            if (abs(Functions::f_der(x) - S_der(x)) > max_der_sub) {
                max_der_sub = abs(Functions::f_der(x) - S_der(x));
                max_der_sub_x = x;
            }
        }
        return max_der_sub;
    }

    double get_der_der_error() {
        int k = 2 * n + 1;
        double max_der_der_sub = 0;
        double max_der_der_sub_x = 0;
        double h = (Functions::b - Functions::a) / k;
        double x = Functions::a;
        for (int i = 1; i <= k; i++) {
            x += h;
            if (abs(Functions::f_der_der(x) -S_der_der(x)) > max_der_der_sub) {
                max_der_der_sub = abs(Functions::f_der_der(x) - S_der_der(x));
                max_der_der_sub_x = x;
            }
        }
        return max_der_der_sub;
    }

    void run(double mu1, double mu2, std::vector<double>& phi) {
        std::vector<double> alpha;
        std::vector<double> beta;

        double h = (Functions::b - Functions::a) / n;

        alpha.push_back(0);
        beta.push_back(mu1);

        for (int i = 0; i < phi.size() - 1; i++) {
            alpha.push_back(1.0 / (-4.0 - alpha[i]));
            beta.push_back((phi[i + 1] / h + beta[i]) / (-4.0 - alpha[i]));
        }

        int N = alpha.size();

        c[N] = mu2;

        for (int i = N - 1; i >= 0; i--) {
            c[i] = alpha[i] * c[i + 1] + beta[i];
        }
    }

    double S(double x) {
        int i;
        double h = (Functions::b - Functions::a) / (double)n;
        i = (int)((x - Functions::a) / h) + 1;
        if (i == n + 1)
            i = n;
        return a[i] + b[i] * (x - (Functions::a + i * h)) + c[i] * pow(x - (Functions::a + i * h), 2.0) / 2.0 + d[i] * pow(x - (Functions::a + i * h), 3.0) / 6.0;
    }

    double S_der(double x) {
        int i;
        double h = (Functions::b - Functions::a) / (double)n;
        i = (int)((x - Functions::a) / h) + 1;
        if (i == n + 1)
            i = n;
        return b[i] + c[i] * (x - (Functions::a + i * h)) + d[i] * pow(x - (Functions::a + i * h), 2.0) / 2.0;
    }

    double S_der_der(double x) {
        int i;
        double h = (Functions::b - Functions::a) / (double)n;
        i = (int)((x - Functions::a) / h) + 1;
        if (i == n + 1)
            i = n;
        return c[i] + d[i] * (x - (Functions::a + i * h));
    }

};
#endif // SOLVER_H
