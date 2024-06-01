#include "solver.h"
#include <QObject>
#include <iostream>

//constexpr type_d pi = 3.14159265358979323846264338327950288Q;

constexpr type_d pi = 3.14159265358979323846264338327950288L;

type_d u_test::u(type_d x, type_d y) {
    return exp(sin(pi * x * y) * sin(pi * x * y));
}

type_d u_test::uy0(type_d y) {
    return 1;
}
type_d u_test::uy1(type_d y) {
    return exp(sin(pi * y) * sin(pi * y));
}
type_d u_test::ux0(type_d x) {
    return 1;
}
type_d u_test::ux1(type_d x) {
    return exp(sin(pi * x) * sin(pi * x));
}
/*
type_d u_test::ux_border(type_d x) {
    type_d y = (x >= solver::xCut) ? solver::yCut : 1;
    return exp(sin(pi * x * y) * sin(pi * x * y));
}
type_d u_test::uy_border(type_d y) {
    type_d x = (y >= solver::yCut) ? solver::xCut : 1;
    return exp(sin(pi * x * y) * sin(pi * x * y));
}
*/
type_d u_test::f(type_d x, type_d y) {
    return u(x, y) * pi * pi * (x * x + y * y) * (- 1 - 4 * cos(2 * pi * x * y) + cos(4 * pi * x * y)) / 2;
}

type_d u_main::uy0(type_d y) {
    return sin(pi * y);
}

type_d u_main::uy1(type_d y) {
    return sin(pi * y);
}

type_d u_main::ux0(type_d x) {
    return x - x * x;
}

type_d u_main::ux1(type_d x) {
    return x - x * x;
}

type_d u_main::f(type_d x, type_d y) {
    return sin(pi * x * y) * sin(pi * x * y);
}

void solver::constructor() {
    k = 0;
    h = 0;
    N = 0;
    M = 0;
    x0 = 0;
    X = 0;
    y0 = 0;
    Y = 0;
    task = Functions::test;

    uy0 = u_test::uy0;
    uy1 = u_test::uy1;
    ux0 = u_test::ux0;
    ux1 = u_test::ux1;
    u = u_test::u;
    f = u_test::f;
}

void solver::constructor(int function) {
    k = 0;
    h = 0;
    N = 0;
    M = 0;
    x0 = 0;
    X = 0;
    y0 = 0;
    Y = 0;

    if (function == Functions::test) {
        uy0 = u_test::uy0;
        uy1 = u_test::uy1;
        ux0 = u_test::ux0;
        ux1 = u_test::ux1;
        u = u_test::u;
        f = u_test::f;
        task = Functions::test;
    }
    else if (function == Functions::tmain) {
        uy0 = u_main::uy0;
        uy1 = u_main::uy1;
        ux0 = u_main::ux0;
        ux1 = u_main::ux1;
        f = u_main::f;
        task = Functions::tmain;
    }
}


void solver::prepare(Matrix& v, Matrix& z, type_d a, type_d c) {
    v.resize(N + 1, M + 1);
    z.resize(N + 1, M + 1);

    for (int i = 0; i < N + 1; i++) {
        v(i, 0) = ux0(a + h * i);
        v(i, M) = ux1(a + h * i);
    }

    for (int i = 0; i < M + 1; i++) {
        v(0, i) = uy0(c + k * i);
        v(N, i) = uy1(c + k * i);
    }

    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++)
            v(i, j) = ux0(a + h * i) + k * j * (ux1(a + h * i) - ux0(a + h * i));

    for (int i = 0; i < N + 1; i++)
        for (int j = 0; j < M + 1; j++)
            z(i, j) = 0;
}
void solver::prepare(Matrix& v, Matrix& z, type_d a, type_d b, type_d c, type_d d) {
    v.resize(N + 1, M + 1);
    z.resize(N + 1, M + 1);

    for (int i = 0; i < N + 1; i++) {
        v(i, 0) = u(a + h * i, c);
        v(i, M) = u(a + h * i, d);
    }

    for (int i = 0; i < M + 1; i++) {
        v(0, i) = u(a, c + k * i);
        v(N, i) = u(b, c + k * i);
    }

    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++)
            v(i, j) = u(a + h * i, c) + k * j * (u(a + h * i, d) - u(a + h * i, c));

    for (int i = 0; i < N + 1; i++)
        for (int j = 0; j < M + 1; j++)
            z(i, j) = 0;
}
void solver::prepareP(Matrix& v, Matrix& z, type_d a, type_d b, type_d c, type_d d) {
    v.resize(N + 1, M + 1);
    z.resize(N + 1, M + 1);

    for(int i = 0; i < N + 1; i++) {
        for(int j = 0; j < M + 1; j++) {
            v(i, j) = 1;
            z(i, j) = 0;
        }
    }

    for (int i = 1; i < P; i++) {
        for (int j = 1; j < M; j++) {
            v(i, j) = ux0(a + h * i) + k * j * (u(a + h * i, c + k * M) - ux0(a + h * i));
        }
    }
    for (int i = P; i < N; i++) {
        for (int j = 1; j < Q; j++) {
            v(i, j) = ux0(a + h * i) + 2 * k * j * (u(a + h * i, c + k * Q) - ux0(a + h * i));
        }
    }
/*
    for (int j = 1; j < Q; j++) {
        for (int i = 1; i < N; i++) {
            v(i, j) = (v(i, j) + uy0(c + k * j) + h * i * (u(a + h * N, c + k * j) - uy0(c + k * j) ) ) / 2;
        }
    }
    for (int j = Q; j < M; j++) {
        for (int i = 1; i < P; i++) {
            v(i, j) = (v(i, j) + uy0(c + k * j) + 2 * h * i * (u(a + h * P, c + k * j) - uy0(c + k * j) ) ) / 2;
        }
    }


// wtf bro>?
    int Lx = P / 2;
    int Ly = Q / 2;
    for (int i = Lx; i < P; i++) {
        for (int j = Ly; j < M - Ly; j++) {
            v(i, j) = v(i, Ly) + k * (j - Ly) * (v(i, M - Ly) - v(i, Ly));
        }
    }
    for (int i = P; i < N - Lx; i++) {
        for (int j = Ly; j < Q; j++) {
            v(i, j) = v(i, Ly) + 2 * k * (j - Ly) * (v(i, Q) - v(i, Ly));
        }
    }

    for (int j = Ly; j < Q; j++) {
        for (int i = Lx; i < N - Lx; i++) {
            v(i, j) = (v(i, j) + v(Lx, j) + k * (i - Lx) * (v(N - Lx, j) - v(Lx, j))) / 2;
        }
    }
    for (int j = Q; j < M - Ly; j++) {
        for (int i = Lx; i < P; i++) {
            v(i, j) = (v(i, j) + v(Lx, j) + 2 * k * (i - Lx) * (v(P, j) - v(Lx, j))) / 2;
        }
    }
*/

    for (int i = 0; i < N + 1; i++) {
        int M__ = (i >= P) ? Q: M;
        v(i, 0) = ux0(a + h * i);
        v(i, M__) = u(a + h * i, c + k * M__);
    }
    for (int j = 0; j < M + 1; j++) {
        int N__ = (j >= Q) ? P: N;
        v(0, j) = uy0(c + k * j);
        v(N__, j) = u(a + h * N__, c + k * j);
    }


    v(P, Q) = u(a + h * P, c + k * Q);

    /*
    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++) {
            if(!((i == P && j >= Q) || (j == Q && i >= P))) {
                int M__ = (i > P) ? Q: M;
                int tmp = (j > Q) ? 2*k : k;
                v(i, j) = ux0(a + h * i) + k * j * (u(a + h * i, c + k * M__) - ux0(a + h * i));
            }
                //v(i, j) = ux0(a + h * i) + k * j * (u(a + h * i, c + k * M__) - ux0(a + h * i));
    }
    */
/*
    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++) {
            if(i != P && j != Q)
                v(j, j) = 0;
            //v(i, j) = ux0(a + h * i) + k * j * (u(a + h * i, c + k * M__) - ux0(a + h * i));
    }
*/
    for (int i = 0; i < N + 1; i++)
        for (int j = 0; j < M + 1; j++)
            z(i, j) = abs(u(a + h * i, c + k * j) - v(i, j));
}

void solver::prepare(Matrix& v, type_d a, type_d c) {
    v.resize(N + 1, M + 1);

    for (int i = 0; i < N + 1; i++) {
        v(i, 0) = ux0(a + h * i);
        v(i, M) = ux1(a + h * i);
    }

    for (int i = 1; i < M + 1; i++) {
        v(0, i) = uy0(c + k * i);
        v(N, i) = uy1(c + k * i);
    }

    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++)
            v(i, j) = ux0(a + h * i) + k * j * (ux1(a+ h * i) - ux0(a + h * i));
}

void solver::step(Matrix& v, Matrix& z, type_d a, type_d c, type_d& mz, type_d& acc) {
    //zeidel

    it++;
    type_d max_z = 0;
    type_d accuracy = 0;
    type_d last_v;
    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++) {
            last_v = v(i, j);
            v(i, j) = (-f(a + h * i, c + k * j)
                       - hor * v(i - 1, j)
                       - hor * v(i + 1, j)
                       - ver * v(i, j - 1)
                       - ver * v(i, j + 1) ) / A;
            if (abs(last_v - v(i, j)) > accuracy)
                accuracy = abs(last_v - v(i, j));
            z(i, j) = abs(v(i, j) - u(a + i * h, c + j * k));
            if (z(i, j) > max_z)
                max_z = z(i, j);
        }
    mz = max_z;
    acc = accuracy;
}

void solver::step(Matrix& v, type_d a, type_d c, type_d& acc) {
    //zeidel

    it++;
    type_d accuracy = 0;
    type_d last_v;
    for (int i = 1; i < N; i++)
        for (int j = 1; j < M; j++) {
            last_v = v(i, j);
            v(i, j) = (-f(a + h * i, c + k * j)
                       - hor * v(i - 1, j)
                       - hor * v(i + 1, j)
                       - ver * v(i, j - 1)
                       - ver * v(i, j + 1) ) / A;
            if (abs(last_v - v(i, j)) > accuracy)
                accuracy = abs(last_v - v(i, j));
        }
    acc = accuracy;
}

void solver::step_mvr(Matrix& v, Matrix& z, type_d a, type_d c, type_d& mz, type_d& acc) {
    //mvr

    omp_lock_t accuracyLock;
    omp_init_lock(&accuracyLock);

    it++;
    type_d max_z = 0;
    type_d accuracy = 0;
    type_d last_v;
    type_d localAcc, localMaxZ;
    int i, j;
#pragma omp parallel for private(i, j, last_v, localAcc, localMaxZ)
    for (i = 1; i < P; i++) {
        localAcc = 0;
        localMaxZ = 0;
        for (j = 1; j < Q; j++) {
            last_v = v(i, j);
            v(i, j) = (- w * f(a + h * i, c + k * j)
                       - w * hor * v(i - 1, j)
                       - w * hor * v(i + 1, j)
                       - w * ver * v(i, j - 1)
                       - w * ver * v(i, j + 1)
                       +(1 - w) * A * last_v) / A;
            if (abs(last_v - v(i, j)) > localAcc)
                localAcc = abs(last_v - v(i, j));
            z(i, j) = abs(v(i, j) - u(a + i * h, c + j * k));
            if (z(i, j) > localMaxZ)
                localMaxZ = z(i, j);
        }
        omp_set_lock(&accuracyLock);
        if(accuracy < localAcc)
            accuracy = localAcc;
        if(max_z < localMaxZ)
            max_z = localMaxZ;
        omp_unset_lock(&accuracyLock);
    }
#pragma omp parallel for private(i, j, last_v, localAcc, localMaxZ)
    for (i = P; i < N; i++) {
        localAcc = 0;
        localMaxZ = 0;
        for (j = 1; j < Q; j++) {
            last_v = v(i, j);
            v(i, j) = (- w * f(a + h * i, c + k * j)
                       - w * hor * v(i - 1, j)
                       - w * hor * v(i + 1, j)
                       - w * ver * v(i, j - 1)
                       - w * ver * v(i, j + 1)
                       +(1 - w) * A * last_v) / A;
            if (abs(last_v - v(i, j)) > localAcc)
                localAcc = abs(last_v - v(i, j));
            z(i, j) = abs(v(i, j) - u(a + i * h, c + j * k));
            if (z(i, j) > localMaxZ)
                localMaxZ = z(i, j);
        }
        omp_set_lock(&accuracyLock);
        if(accuracy < localAcc)
            accuracy = localAcc;
        if(max_z < localMaxZ)
            max_z = localMaxZ;
        omp_unset_lock(&accuracyLock);
    }
#pragma omp parallel for private(i, j, last_v, localAcc, localMaxZ)
    for (i = 1; i < P; i++) {
        localAcc = 0;
        localMaxZ = 0;
        for (j = Q; j < M; j++) {
            last_v = v(i, j);
            v(i, j) = (- w * f(a + h * i, c + k * j)
                       - w * hor * v(i - 1, j)
                       - w * hor * v(i + 1, j)
                       - w * ver * v(i, j - 1)
                       - w * ver * v(i, j + 1)
                       +(1 - w) * A * last_v) / A;
            if (abs(last_v - v(i, j)) > localAcc)
                localAcc = abs(last_v - v(i, j));
            z(i, j) = abs(v(i, j) - u(a + i * h, c + j * k));
            if (z(i, j) > localMaxZ)
                localMaxZ = z(i, j);
        }
        omp_set_lock(&accuracyLock);
        if(accuracy < localAcc)
            accuracy = localAcc;
        if(max_z < localMaxZ)
            max_z = localMaxZ;
        omp_unset_lock(&accuracyLock);
    }
    mz = max_z;
    acc = accuracy;
}

void solver::step_mvr(Matrix& v, type_d a, type_d c, type_d& acc) {
    //mvr

    omp_lock_t accuracyLock;
    omp_init_lock(&accuracyLock);

    it++;
    type_d accuracy = 0;
    type_d last_v;
    int i, j;
    type_d localAcc = 0;
#pragma omp parallel for private(i, j, last_v, localAcc)
    for (i = 1; i < N; i++) {
        localAcc = 0;
        for (j = 1; j < M; j++) {
            last_v = v(i, j);
            v(i, j) = (- w * f(a + h * i, c + k * j)
                       - w * hor * v(i - 1, j)
                       - w * hor * v(i + 1, j)
                       - w * ver * v(i, j - 1)
                       - w * ver * v(i, j + 1)
                       + (1 - w) * A * last_v) / A;
            if (abs(last_v - v(i, j)) > localAcc)
                localAcc = abs(last_v - v(i, j));
        }
        omp_set_lock(&accuracyLock);
        if(accuracy < localAcc)
            accuracy = localAcc;
        omp_unset_lock(&accuracyLock);
    }
    acc = accuracy;
}


void solver::copy(Matrix& v1, Matrix& z1, Matrix& v2, Matrix& z2) {
    for (int i = 0; i <= N; i++)
        for (int j = 0; j <= M; j++) {
            v2(i, j) = v1(i, j);
            z2(i, j) = z1(i, j);
        }
}

void solver::copy(Matrix& v1,Matrix& v2) {
    for (int i = 0; i <= N; i++)
        for (int j = 0; j <= M; j++) {
            v2(i, j) = v1(i, j);
        }
}

Q_INVOKABLE void solver::solve(int n, int m, type_d a, type_d b, type_d c, type_d d, type_d eps, int m_it,
                               Matrix& v, std::vector<Matrix>& vPhotos,
                               Matrix& z, std::vector<Matrix>& zPhotos,
                               int p, int q) {
    timer.start();
    N = n;
    M = m;
    P = p;
    Q = q;
    x0 = a;
    X = b;
    y0 = c;
    Y = d;
    k = (Y - y0) / M;
    h = (X - x0) / N;
    valid = true;
    epsilon = eps;

    max_it = m_it;
    it = 0;
    hor = 1 / (h * h);
    ver = 1 / (k * k);
    A = - 2 / (h * h) - 2 / (k * k);
    vPhotos.resize(10);
    zPhotos.resize(10);
    iter.resize(2 + max_it / interval);
    ACCURACY.resize(2 + max_it / interval);
    MAX_R.resize(2 + max_it / interval);
    MAX_Z.resize(2 + max_it / interval);

    int iter_size = 1;

    prepareP(v, z, a, b, c, d);
    if(max_it == 0)
        return;
    //prepareP(v, z, N, M, a, b, c, d, ux0, uy0, ux1, uy1);
    if (N < 100 && M < 100) {
        for(int i = 0; i < 10; i++) {
            vPhotos[i].resize(N + 1, M + 1);
            zPhotos[i].resize(N + 1, M + 1);
        }
        prepare(vPhotos[0], a, c);
    }
    fill_right_side(v, a, c);
    type_d last_mz;
    type_d last_accuracy = 0;
    int cur_photo = 1;
    if(max_it == 0)
        return;
    emit progressUpdate(0, 66, timer.elapsed(), 1);
    if (meth == Methods::zeidel){
        timer.start();
        step(v, z, a, c, last_mz, last_accuracy);
        emit progressUpdate((0 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
        cur_photo++;

        iter[0] = (it);
        ACCURACY[0] = last_accuracy;
        calc_r(v);
        MAX_R[0] = max_r;
        MAX_Z[0] = last_mz;

        step(v, z, a, c, last_mz, last_accuracy);
        emit progressUpdate((1 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;
        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step(v, z, a, c, last_mz, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (it);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v);
                MAX_R[iter_size] = (max_r);
                MAX_Z[iter_size] = (last_mz);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / 2) && cur_photo < 10 && N < 100 && M < 100) {
                copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
                last_accuracy = cur_accuracy;
                cur_photo++;
            }
            emit progressUpdate((i * 100) / max_it, cur_accuracy, timer.elapsed(), it);
        }
        emit progressUpdate(100, cur_accuracy, timer.elapsed(), it);

        if (N < 100 && M < 100) {
            for (; cur_photo < 10; cur_photo++) {
                copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
            }
        }
        max_z = last_mz;
        achieved_accuracy = cur_accuracy;
        calc_r(v);
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (achieved_accuracy);
        MAX_R[iter_size] = (max_r);
        MAX_Z[iter_size] = (max_z);
        iter.resize(iter_size);
        ACCURACY.resize(iter_size);
        MAX_R.resize(iter_size);
        MAX_Z.resize(iter_size);
    } else if (meth == Methods::mvr){
        timer.start();
        step_mvr(v, z, a, c, last_mz, last_accuracy);
        emit progressUpdate((0 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
        cur_photo++;

        iter[0] = (it);
        ACCURACY[0] = last_accuracy;
        calc_r(v);
        MAX_R[0] = max_r;
        MAX_Z[0] = last_mz;

        step_mvr(v, z, a, c, last_mz, last_accuracy);
        emit progressUpdate((1 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;

        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step_mvr(v, z, a, c, last_mz, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (it);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v);
                MAX_R[iter_size] = (max_r);
                MAX_Z[iter_size] = (last_mz);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / 2) && cur_photo < 10 && N < 100 && M < 100) {
                copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
                last_accuracy = cur_accuracy;
                cur_photo++;
            }
            emit progressUpdate((i * 100) / max_it, cur_accuracy, timer.elapsed(), it);
        }
        emit progressUpdate(100, cur_accuracy, timer.elapsed(), it);

        if (N < 100 && M < 100) {
            for (; cur_photo < 10; cur_photo++) {
                copy(v, z, vPhotos[cur_photo], zPhotos[cur_photo]);
            }
        }
        max_z = last_mz;
        achieved_accuracy = cur_accuracy;
        std::cout << "final" << std::endl;
        calc_r(v);
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (achieved_accuracy);
        MAX_R[iter_size] = (max_r);
        MAX_Z[iter_size] = (max_z);
        iter.resize(iter_size);
        ACCURACY.resize(iter_size);
        MAX_R.resize(iter_size);
        MAX_Z.resize(iter_size);
    }
    duration = timer.elapsed();
    emit solveFinished();
}

Q_INVOKABLE void solver::solve(int n, int m, type_d a, type_d b, type_d c, type_d d, type_d eps, int m_it,
                               Matrix& v, std::vector<Matrix>& vPhotos) {
    timer.start();
    N = n;
    M = m;
    x0 = a;
    X = b;
    y0 = c;
    Y = d;
    k = (Y - y0) / M;
    h = (X - x0) / N;
    epsilon = eps;
    max_it = m_it;
    it = 0;
    hor = 1 / (h * h);
    ver = 1 / (k * k);
    A = - 2 / (h * h) - 2 / (k * k);
    vPhotos.resize(10);
    iter.resize(2 + max_it / interval);
    ACCURACY.resize(2 + max_it / interval);
    MAX_R.resize(2 + max_it / interval);
    int iter_size = 1;

    prepare(v, a, c);
    if (N < 100 && M < 100) {
        for(int i = 0; i < 10; i++){
            vPhotos[i].resize(N + 1, M + 1);
        }
        prepare(vPhotos[0], a, c);
    }
    fill_right_side(v, a, c);
    type_d last_accuracy = 0;
    int cur_photo = 1;

    emit progressUpdate(0, 66, timer.elapsed(), 1);
    meth = Methods::mvr;
    if (meth == Methods::zeidel){
        timer.start();
        step(v, a, c, last_accuracy);
        emit progressUpdate((0 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, vPhotos[cur_photo]);
        cur_photo++;

        iter[0] = (it);
        ACCURACY[0] = last_accuracy;
        calc_r(v);
        MAX_R[0] = max_r;

        step(v, a, c, last_accuracy);
        emit progressUpdate((1 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, vPhotos[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;

        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step(v, a, c, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (it);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v);
                MAX_R[iter_size] = (max_r);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / 2) && cur_photo < 10 && N < 100 && M < 100) {
                copy(v, vPhotos[cur_photo]);
                last_accuracy = cur_accuracy;
                cur_photo++;
            }
            emit progressUpdate((i * 100) / max_it, cur_accuracy, timer.elapsed(), it);
        }
        emit progressUpdate(100, cur_accuracy, timer.elapsed(), it);

        if (N < 100 && M < 100) {
            for (; cur_photo < 10; cur_photo++) {
                copy(v, vPhotos[cur_photo]);
            }
        }
        achieved_accuracy = cur_accuracy;
        calc_r(v);
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (cur_accuracy);
        MAX_R[iter_size] = (max_r);
        iter.resize(iter_size);
        ACCURACY.resize(iter_size);
        MAX_R.resize(iter_size);
    } else if(meth == Methods::mvr){
        timer.start();
        step_mvr(v, a, c, last_accuracy);
        emit progressUpdate((0 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, vPhotos[cur_photo]);
        cur_photo++;

        iter[0] = (it);
        ACCURACY[0] = last_accuracy;
        calc_r(v);
        MAX_R[0] = max_r;

        step_mvr(v, a, c, last_accuracy);
        emit progressUpdate((1 * 100) / max_it, last_accuracy, timer.elapsed(), it);
        if (N < 100 && M < 100) copy(v, vPhotos[cur_photo]);
        cur_photo++;

        type_d cur_accuracy = last_accuracy;

        for (size_t i = 2; i < max_it && cur_accuracy > eps; i++) {
            step_mvr(v, a, c, cur_accuracy);
            if(i % interval == 0){
                iter[iter_size] = (it);
                ACCURACY[iter_size] = (cur_accuracy);
                calc_r(v);
                MAX_R[iter_size] = (max_r);
                iter_size++;
            }
            if (cur_accuracy < (last_accuracy / 2) && cur_photo < 10 && N < 100 && M < 100) {
                copy(v, vPhotos[cur_photo]);
                last_accuracy = cur_accuracy;
                cur_photo++;
            }
            emit progressUpdate((i * 100) / max_it, cur_accuracy, timer.elapsed(), it);
        }
        emit progressUpdate(100, cur_accuracy, timer.elapsed(), it);

        if (N < 100 && M < 100) {
            for (; cur_photo < 10; cur_photo++) {
                copy(v, vPhotos[cur_photo]);
            }
        }
        achieved_accuracy = cur_accuracy;
        calc_r(v);
        iter[iter_size] = (it);
        ACCURACY[iter_size] = (cur_accuracy);
        MAX_R[iter_size] = (max_r);
        iter.resize(iter_size);
        ACCURACY.resize(iter_size);
        MAX_R.resize(iter_size);
    }
    duration = timer.elapsed();
    valid = true;
    emit solveFinished();
}

inline int solver::is_border(int i, int j){
    return ((i == 0) || (j == 0) || (i == N) || (j == M) || (i == P && j >= Q) || (j == Q && i >= P));
}

void solver::fill_right_side(Matrix& v, type_d a, type_d c){
    right_side.clear();
    for (int j = 1; j < M; j++){
        for (int i = 1; i < N; i++){
            if(i < P || j < Q) {
                right_side.push_back(- f(a + i * h, c + j * k)
                                 - hor * v(i - 1, j) * is_border(i - 1, j)
                                 - hor * v(i + 1, j) * is_border(i + 1, j)
                                 - ver * v(i, j - 1) * is_border(i, j - 1)
                                 - ver * v(i, j + 1) * is_border(i, j + 1));
            }
        }
    }
}

void solver::calc_r(Matrix& v){
    int place = 0;
    type_d r = 0;
    for (int j = 1; j < M; j++){
        for (int i = 1; i < N; i++){
            if(i < P || j < Q) {

                r += (A  * v(i, j) + (!(is_border(i - 1, j))) * hor * v(i - 1, j)
                      + (!(is_border(i + 1, j))) * hor * v(i + 1, j)
                      + (!(is_border(i, j - 1))) * hor * v(i, j - 1)
                      + (!(is_border(i, j + 1))) * hor * v(i, j + 1) - right_side[place]) *
                     (A  * v(i, j) + (!(is_border(i - 1, j))) * hor * v(i - 1, j)
                      + (!(is_border(i + 1, j))) * hor * v(i + 1, j)
                      + (!(is_border(i, j - 1))) * hor * v(i, j - 1)
                      + (!(is_border(i, j + 1))) * hor * v(i, j + 1) - right_side[place]);
                place++;
            }
        }            
    }
    max_r = sqrt(r);
}
