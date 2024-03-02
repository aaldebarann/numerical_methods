#include "solver.h"



double u_real::u(double x, double y) {
    return pow(x, 2.0) + pow(y, 3.0) + 1;
}

double u_real::u0y(double y) {
    return pow(y, 3.0) + 1;
}

double u_real::u1y(double y) {
    return pow(y, 3.0) + 2;
}

double u_real::ux0(double x) {
    return pow(x, 2.0) + 1;
}

double u_real::ux1(double x) {
    return pow(x, 2.0) + 2;
}

double u_real::f(double x, double y) {
    return -6 * y - 2;
}


double u_kapkaev::u(double x, double y) {
    return 1.0 - pow(x, 2.0) - pow(y, 2.0);
}

double u_kapkaev::u0y(double y) {
    return -pow(y, 2.0);
}

double u_kapkaev::u1y(double y) {
    return -pow(y, 2.0);
}

double u_kapkaev::ux0(double x) {
    return -pow(x, 2.0);
}

double u_kapkaev::ux1(double x) {
    return -pow(x, 2.0);
}

double u_kapkaev::f(double x, double y) {
    return 4.0;
}

solver::solver() {
    k = 0;
    h = 0;
    N = 0;
    M = 0;
    x0 = 0;
    X = 0;
    y0 = 0;
    Y = 0;
    task = Functions::STRONGINA;

    u0y = u_real::u0y;
    u1y = u_real::u1y;
    ux0 = u_real::ux0;
    ux1 = u_real::ux1;
    u = u_real::u;
    f = u_real::f;
}

solver::solver(int function) {
    k = 0;
    h = 0;
    N = 0;
    M = 0;
    x0 = 0;
    X = 0;
    y0 = 0;
    Y = 0;

    if (function == Functions::STRONGINA) {
        u0y = u_real::u0y;
        u1y = u_real::u1y;
        ux0 = u_real::ux0;
        ux1 = u_real::ux1;
        u = u_real::u;
        f = u_real::f;
        task = Functions::STRONGINA;
    }
    else if (function == Functions::KAPKAEV) {
        u0y = u_kapkaev::u0y;
        u1y = u_kapkaev::u1y;
        ux0 = u_kapkaev::ux0;
        ux1 = u_kapkaev::ux1;
        u = u_kapkaev::u;
        f = u_kapkaev::f;
        task = Functions::KAPKAEV;
    }
}

void solver::prepare(std::vector<std::vector<double>>& v, std::vector<std::vector<double>>& z, double a, double c) {

    //map[x][y]

    v.resize(N + 1);
    for (int i = 0; i < N + 1; i++)
        v[i].resize(M + 1);

    z.resize(N + 1);
    for (int i = 0; i < N + 1; i++)
        z[i].resize(M + 1);


    for (int i = 0; i < N + 1; i++) {
        v[i][0] = ux0(a + h * i);
        v[i][M] = ux1(a + h * i);
    }

    for (int i = 1; i < M; i++) {
        v[0][i] = u0y(c + k * i);
        v[M][i] = u1y(c + k * i);
    }



    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++)
            v[i][j] = 0;

    for (int i = 0; i < N + 1; i++)
        for (int j = 0; j < M + 1; j++)
            z[i][j] = 0;
}

void solver::step(std::vector<std::vector<double>>& v, std::vector<std::vector<double>>& z, double a, double c, double& mz, double& acc) {
    //zeidel

    it++;
    double max_z = 0;
    double accuracy = 0;
    double last_v;
    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++) {
            last_v = v[i][j];
            v[i][j] = (-f(a + h * i, c + k * j)
                       - hor * v[i - 1][j]
                       - hor * v[i + 1][j]
                       - ver * v[i][j - 1]
                       - ver * v[i][j + 1] ) / A;
            if (abs(last_v - v[i][j]) > accuracy)
                accuracy = abs(last_v - v[i][j]);
            z[i][j] = abs(v[i][j] - u(a + i * h, c + j * k));
            if (z[i][j] > max_z)
                max_z = z[i][j];
        }

    mz = max_z;
    acc = accuracy;
}

void solver::copy(std::vector<std::vector<double>>& v1, std::vector<std::vector<double>>& z1, std::vector<std::vector<double>>& v2, std::vector<std::vector<double>>& z2) {
    for (int i = 0; i <= N; i++)
        for (int j = 0; j <= M; j++) {
            v2[i][j] = v1[i][j];
            z2[i][j] = z1[i][j];
        }
}

void solver::solve(int n, int m, double a, double b, double c, double d, double eps, int m_it, std::vector<std::vector<std::vector<double>>>& v, std::vector<std::vector<std::vector<double>>>& z) {
    N = n;
    M = m;
    x0 = a;
    X = b;
    y0 = c;
    Y = d;
    k = (Y - y0) / M;
    h = (X - x0) / N;
    valid = 1;
    epsilon = eps;

    max_it = m_it;
    it = 0;
    hor = 1.0 / pow(h, 2.0);
    ver = 1.0 / pow(k, 2.0);
    A = -2.0 * (1.0 / pow(h, 2) + 1.0 / pow(k, 2));
    v.resize(10);
    z.resize(10);


    for(int i = 0; i < 10; i++){
        prepare(v[i], z[i], a, c);
    }
    fill_right_side(v[9], a, c);
    double last_mz;
    double last_accuracy = 0;
    int cur_photo = 0;

    step(v[9], z[9], a, c, last_mz, last_accuracy);
    copy(v[9], z[9], v[cur_photo], z[cur_photo]);
    cur_photo++;

    step(v[9], z[9], a, c, last_mz, last_accuracy);
    copy(v[9], z[9], v[cur_photo], z[cur_photo]);
    cur_photo++;

    double cur_accuracy = last_accuracy;

    for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
        step(v[9], z[9], a, c, last_mz, cur_accuracy);
        if (cur_accuracy < (last_accuracy / 2.0) && cur_photo < 9) {
            copy(v[9], z[9], v[cur_photo], z[cur_photo]);
            last_accuracy = cur_accuracy;
            cur_photo++;
        }
    }
    for (; cur_photo < 9; cur_photo++) {
        copy(v[9], z[9], v[cur_photo], z[cur_photo]);
    }
    max_z = last_mz;
    achieved_accuracy = cur_accuracy;
    calc_r(v[9]);
}

inline int solver::is_border(int i, int j){
    return ((i == 0) || (j == 0) || (i == N) || (j == M)) ? 1 : 0;
}

void solver::fill_right_side(std::vector<std::vector<double>>& v, double a, double c){
    right_side.clear();
    for (int j = 1; j < M; j++){
        for (int i = 1; i < N; i++){
            right_side.push_back(- f(a + i * h, c + j * k)
                                 - hor * v[i - 1][j] * is_border(i - 1, j)
                                 - hor * v[i + 1][j] * is_border(i + 1, j)
                                 - ver * v[i][j - 1] * is_border(i, j - 1)
                                 - ver * v[i][j + 1] * is_border(i, j + 1));
        }
    }
}

void solver::calc_r(std::vector<std::vector<double>>& v){
    int place = 0;
    double r = 0;
    for (int j = 1; j < M; j++){
        for (int i = 1; i < N; i++){
            r += pow(A  * v[i][j] + (!(is_border(i - 1, j))) * hor * v[i - 1][j]
                    + (!(is_border(i + 1, j))) * hor * v[i + 1][j]
                    + (!(is_border(i, j - 1))) * hor * v[i][j - 1]
                    + (!(is_border(i, j + 1))) * hor * v[i][j + 1] - right_side[place], 2);
            place++;
        }
    }
    max_r = sqrt(r);
}

















