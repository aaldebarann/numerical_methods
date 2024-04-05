#include "solver.h"
#define type_d high_precision_type
using high_precision_type = boost::multiprecision::cpp_dec_float_50;

type_d u_test::u(type_d x, type_d y) {
    return exp(sin(boost::math::constants::pi<type_d>() * x * y) * sin(boost::math::constants::pi<type_d>() * x * y));
}

type_d u_test::u0y(type_d y) {
    return (1);
}

type_d u_test::u1y(type_d y) {
    return exp(sin(boost::math::constants::pi<type_d>() * y) * sin(boost::math::constants::pi<type_d>() * y));
}

type_d u_test::ux0(type_d x) {
    return (1);
}

type_d u_test::ux1(type_d x) {
    return exp(sin(boost::math::constants::pi<type_d>() * x) * sin(boost::math::constants::pi<type_d>() * x));
}

type_d u_test::f(type_d x, type_d y) {
    return u(x, y) * boost::math::constants::pi<type_d>() * boost::math::constants::pi<type_d>() * (x * x + y * y) * (- (1) - (4) * cos((2) * boost::math::constants::pi<type_d>() * x * y) + cos((4) * boost::math::constants::pi<type_d>() * x * y)) / 2;
}

type_d u_main::u0y(type_d y) {
    return sin(boost::math::constants::pi<type_d>() * y);
}

type_d u_main::u1y(type_d y) {
    return sin(boost::math::constants::pi<type_d>() * y);
}

type_d u_main::ux0(type_d x) {
    return x - x * x;
}

type_d u_main::ux1(type_d x) {
    return x - x * x;
}

type_d u_main::f(type_d x, type_d y) {
    return sin(boost::math::constants::pi<type_d>() * x * y) * sin(boost::math::constants::pi<type_d>() * x * y);
}

solver::solver() {
    k = type_d(0);
    h = type_d(0);
    N = 0;
    M = 0;
    x0 = type_d(0);
    X = type_d(0);
    y0 = type_d(0);
    Y = type_d(0);
    task = Functions::test;

    u0y = u_test::u0y;
    u1y = u_test::u1y;
    ux0 = u_test::ux0;
    ux1 = u_test::ux1;
    u = u_test::u;
    f = u_test::f;
}

solver::solver(int function) {
    k = type_d(0);
    h = type_d(0);
    N = 0;
    M = 0;
    x0 = type_d(0);
    X = type_d(0);
    y0 = type_d(0);
    Y = type_d(0);

    if (function == Functions::test) {
        u0y = u_test::u0y;
        u1y = u_test::u1y;
        ux0 = u_test::ux0;
        ux1 = u_test::ux1;
        u = u_test::u;
        f = u_test::f;
        task = Functions::test;
    }
    else if (function == Functions::tmain) {
        u0y = u_main::u0y;
        u1y = u_main::u1y;
        ux0 = u_main::ux0;
        ux1 = u_main::ux1;
        f = u_main::f;
        task = Functions::tmain;
    }
}


void solver::prepare(std::vector<std::vector<type_d>>& v, std::vector<std::vector<type_d>>& z, type_d a, type_d c) {
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
            v[i][j] = ux0(a + h * i) + k * j * (ux1(a+ h * i) - ux0(a + h * i));

    for (int i = 0; i < N + 1; i++)
        for (int j = 0; j < M + 1; j++)
            z[i][j] = 0;
}

void solver::prepare(std::vector<std::vector<type_d>>& v, type_d a, type_d c) {
    v.resize(N + 1);
    for (int i = 0; i < N + 1; i++)
        v[i].resize(M + 1);

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
            v[i][j] = ux0(a + h * i) + k * j * (ux1(a+ h * i) - ux0(a + h * i));
}

void solver::step(std::vector<std::vector<type_d>>& v, std::vector<std::vector<type_d>>& z, type_d a, type_d c, type_d& mz, type_d& acc) {
    //zeidel

    it++;
    type_d max_z = type_d(0);
    type_d accuracy = type_d(0);
    type_d last_v;
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

void solver::step(std::vector<std::vector<type_d>>& v, type_d a, type_d c, type_d& acc) {
    //zeidel

    it++;
    type_d accuracy = 0;
    type_d last_v;
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
        }
    acc = accuracy;
}

void solver::step_mvr(std::vector<std::vector<type_d>>& v, std::vector<std::vector<type_d>>& z, type_d a, type_d c, type_d& mz, type_d& acc) {
    //mvr

    it++;
    type_d max_z = type_d(0);
    type_d accuracy = type_d(0);
    type_d last_v;
    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++) {
            last_v = v[i][j];
            v[i][j] = (- w * f(a + h * i, c + k * j)
                       - w * hor * v[i - 1][j]
                       - w * hor * v[i + 1][j]
                       - w * ver * v[i][j - 1]
                       - w * ver * v[i][j + 1]
                       +(1 - w) * A * last_v) / A;
            if (abs(last_v - v[i][j]) > accuracy)
                accuracy = abs(last_v - v[i][j]);
            z[i][j] = abs(v[i][j] - u(a + i * h, c + j * k));
            if (z[i][j] > max_z)
                max_z = z[i][j];
        }

    mz = max_z;
    acc = accuracy;
}

void solver::step_mvr(std::vector<std::vector<type_d>>& v, type_d a, type_d c, type_d& acc) {
    //mvr

    it++;
    type_d accuracy = type_d(0);
    type_d last_v;
    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++) {
            last_v = v[i][j];
            v[i][j] = (- w * f(a + h * i, c + k * j)
                       - w * hor * v[i - 1][j]
                       - w * hor * v[i + 1][j]
                       - w * ver * v[i][j - 1]
                       - w * ver * v[i][j + 1]
                       +(1 - w) * A * last_v) / A;
            if (abs(last_v - v[i][j]) > accuracy)
                accuracy = abs(last_v - v[i][j]);
        }
    acc = accuracy;
}


void solver::copy(std::vector<std::vector<type_d>>& v1, std::vector<std::vector<type_d>>& z1, std::vector<std::vector<type_d>>& v2, std::vector<std::vector<type_d>>& z2) {
    for (int i = 0; i <= N; i++)
        for (int j = 0; j <= M; j++) {
            v2[i][j] = v1[i][j];
            z2[i][j] = z1[i][j];
        }
}

void solver::copy(std::vector<std::vector<type_d>>& v1,std::vector<std::vector<type_d>>& v2) {
    for (int i = 0; i <= N; i++)
        for (int j = 0; j <= M; j++) {
            v2[i][j] = v1[i][j];
        }
}

void solver::solve(int n, int m, type_d a, type_d b, type_d c, type_d d, type_d eps, int m_it, std::vector<std::vector<std::vector<type_d>>>& v, std::vector<std::vector<std::vector<type_d>>>& z) {
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
    hor = type_d(1) / (h * h);
    ver = type_d(1) / (k * k);
    A = type_d(- 2) * (type_d(1) / (h * h) + type_d(1) / (k * k));
    v.resize(10);
    z.resize(10);
    iter.resize(2 + max_it / interval);
    ACCURACY.resize(2 + max_it / interval);
    MAX_R.resize(2 + max_it / interval);
    MAX_Z.resize(2 + max_it / interval);

    int iter_size = 1;

    for(int i = 0; i < 10; i++){
        prepare(v[i], z[i], a, c);
    }
    fill_right_side(v[9], a, c);
    type_d last_mz;
    type_d last_accuracy = type_d(0);
    int cur_photo = 1;

    if (meth == Methods::zeidel){
        step(v[9], z[9], a, c, last_mz, last_accuracy);
        copy(v[9], z[9], v[cur_photo], z[cur_photo]);
        cur_photo++;

        iter[0] = (it);
        ACCURACY[0] = last_accuracy;
        calc_r(v[9]);
        MAX_R[0] = max_r;
        MAX_Z[0] = last_mz;

        step(v[9], z[9], a, c, last_mz, last_accuracy);
        copy(v[9], z[9], v[cur_photo], z[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;

        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step(v[9], z[9], a, c, last_mz, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (it);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v[9]);
                MAX_R[iter_size] = (max_r);
                MAX_Z[iter_size] = (last_mz);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / type_d(2)) && cur_photo < 9) {
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
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (achieved_accuracy);
        MAX_R[iter_size] = (max_r);
        MAX_Z[iter_size] = (max_z);
        iter.resize(iter_size);
        ACCURACY.resize(iter_size);
        MAX_R.resize(iter_size);
        MAX_Z.resize(iter_size);
    } else if (meth == Methods::mvr){
        step_mvr(v[9], z[9], a, c, last_mz, last_accuracy);
        copy(v[9], z[9], v[cur_photo], z[cur_photo]);
        cur_photo++;

        iter[0] = (it);
        ACCURACY[0] = last_accuracy;
        calc_r(v[9]);
        MAX_R[0] = max_r;
        MAX_Z[0] = last_mz;

        step_mvr(v[9], z[9], a, c, last_mz, last_accuracy);
        copy(v[9], z[9], v[cur_photo], z[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;

        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step_mvr(v[9], z[9], a, c, last_mz, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (it);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v[9]);
                MAX_R[iter_size] = (max_r);
                MAX_Z[iter_size] = (last_mz);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / type_d(2)) && cur_photo < 9) {
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
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (achieved_accuracy);
        MAX_R[iter_size] = (max_r);
        MAX_Z[iter_size] = (max_z);
        iter.resize(iter_size + 1);
        ACCURACY.resize(iter_size + 1);
        MAX_R.resize(iter_size + 1);
        MAX_Z.resize(iter_size + 1);
    }
}

void solver::solve(int n, int m, type_d a, type_d b, type_d c, type_d d, type_d eps, int m_it, std::vector<std::vector<std::vector<type_d>>>& v) {
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
    hor = type_d(1) / (h * h);
    ver = type_d(1) / (k * k);
    A = type_d(- 2) * (type_d(1) / (h * h) + type_d(1) / (k * k));
    v.resize(10);
    iter.resize(2 + max_it / interval);
    ACCURACY.resize(2 + max_it / interval);
    MAX_R.resize(2 + max_it / interval);
    int iter_size = 1;

    for(int i = 0; i < 10; i++){
        prepare(v[i], a, c);
    }
    fill_right_side(v[9], a, c);
    type_d last_accuracy = type_d(0);
    int cur_photo = 1;

    if (meth == Methods::zeidel){
        step(v[9], a, c, last_accuracy);
        copy(v[9], v[cur_photo]);
        cur_photo++;

        iter[0] = (1);
        ACCURACY[0] = last_accuracy;
        calc_r(v[9]);
        MAX_R[0] = max_r;

        step(v[9], a, c, last_accuracy);
        copy(v[9], v[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;

        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step(v[9], a, c, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (i);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v[9]);
                MAX_R[iter_size] = (max_r);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / type_d(2)) && cur_photo < 9) {
                copy(v[9], v[cur_photo]);
                last_accuracy = cur_accuracy;
                cur_photo++;
            }
        }

        for (; cur_photo < 9; cur_photo++) {
            copy(v[9], v[cur_photo]);
        }
        achieved_accuracy = cur_accuracy;
        calc_r(v[9]);
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (cur_accuracy);
        MAX_R[iter_size] = (max_r);
        iter.resize(iter_size);
        ACCURACY.resize(iter_size);
        MAX_R.resize(iter_size);
    } else if(meth == Methods::mvr){
        step_mvr(v[9], a, c, last_accuracy);
        copy(v[9], v[cur_photo]);
        cur_photo++;

        iter[0] = (1);
        ACCURACY[0] = last_accuracy;
        calc_r(v[9]);
        MAX_R[0] = max_r;

        step_mvr(v[9], a, c, last_accuracy);
        copy(v[9], v[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;

        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step_mvr(v[9], a, c, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (i);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v[9]);
                MAX_R[iter_size] = (max_r);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / type_d(2)) && cur_photo < 9) {
                copy(v[9], v[cur_photo]);
                last_accuracy = cur_accuracy;
                cur_photo++;
            }
        }

        for (; cur_photo < 9; cur_photo++) {
            copy(v[9], v[cur_photo]);
        }
        achieved_accuracy = cur_accuracy;
        calc_r(v[9]);
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (cur_accuracy);
        MAX_R[iter_size] = (max_r);
        iter.resize(iter_size);
        ACCURACY.resize(iter_size);
        MAX_R.resize(iter_size);
    }
}

inline int solver::is_border(int i, int j){
    return ((i == 0) || (j == 0) || (i == N) || (j == M)) ? 1 : 0;
}

void solver::fill_right_side(std::vector<std::vector<type_d>>& v, type_d a, type_d c){
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

void solver::calc_r(std::vector<std::vector<type_d>>& v){
    int place = 0;
    type_d r = type_d(0);
    for (int j = 1; j < M; j++){
        for (int i = 1; i < N; i++){
            r += (A  * v[i][j] + (!(is_border(i - 1, j))) * hor * v[i - 1][j]
                  + (!(is_border(i + 1, j))) * hor * v[i + 1][j]
                  + (!(is_border(i, j - 1))) * hor * v[i][j - 1]
                  + (!(is_border(i, j + 1))) * hor * v[i][j + 1] - right_side[place]) *
                 (A  * v[i][j] + (!(is_border(i - 1, j))) * hor * v[i - 1][j]
                  + (!(is_border(i + 1, j))) * hor * v[i + 1][j]
                  + (!(is_border(i, j - 1))) * hor * v[i][j - 1]
                  + (!(is_border(i, j + 1))) * hor * v[i][j + 1] - right_side[place]);
            place++;
        }
    }
    max_r = sqrt(r);
}
