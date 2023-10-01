#include "rk.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>


void calculateTask1(double a, double b, double u, int Nmax, double epsilon, double epsilonb, double h, std::ofstream& outputFile, std::ofstream& summary, bool conststep) {
    double x = a;
    double u_0 = u;
    double parameter = 0;
    double u1 = u;
    double u2 = u;
    double ue = u;
    double ele = 0;
    int C1 = 0;
    int C2 = 0;
    double maxele = 0;
    double maxh = h;
    double xmaxh = a;
    double minh = h;
    double xminh = a;
    int i = 0;
    double maxdev = 0;
    double xmaxdev = 0;
    if (conststep == 1) {
        for (; i < Nmax; i++) {
            u1 = rk(parameter, x, u, h, f0);
            u2 = rkh(parameter, x, u, h, f0);
            ue = f0sol(x + h, u_0);

            ele = 16 * (abs(u1 - u2) / 15);
            if (maxele < ele) { maxele = ele; }
            if (maxdev < abs(ue - u1)) { maxdev = abs(ue - u1); xmaxdev = x; }
            if (x + h > b  - epsilonb) {
                h = b - x - epsilonb / 2;
                u1 = rk(parameter, x, u, h, f0);
                u2 = rkh(parameter, x, u, h, f0);
                ue = f0sol(x + h, u_0);
                x += h;
                outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "," << ue << "," << abs(ue - u1) << "\n";
                break;
            }
            x += h;
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "," << ue << "," << abs(ue - u1) << "\n";
        }
        summary << i + 1 << "," << b - x << "," << maxele << "," << maxdev << "," << xmaxdev << "\n";
    }
    else {
        for (; i < Nmax; i++) {
            double lasth = h;
            int c1 = C1;
            int c2 = C2;
            u1 = rkv(parameter, x, u, h, ele, epsilon, C1, C2, f0);

            if (maxele < abs(ele)) { maxele = abs(ele); }
            if (maxh < h) { maxh = h; xmaxh = x; }
            if (minh > h) { minh = h; xminh = x; }
            if (x + h > b - epsilonb) {
                h = b - x - epsilonb / 2;
                u1 = rk(parameter, x, u, h, f0);
                u2 = rkh(parameter, x, u, h, f0);
                ue = f0sol(x + h, u_0);
                x += h;
                outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "," << ue << "," << abs(ue - u1) << "\n";
                break;
            } else if (c1 == (C1 - 1) && c2 == (C2 - 1)) {
                lasth = h / 2;
                u2 = rkh(parameter, x, u, lasth, f0); 
                ue = f0sol(x + lasth, u_0);
                x += lasth;
            } else if (lasth < h) {
                ue = f0sol(x + lasth, u_0);
                u2 = rkh(parameter, x, u, lasth, f0);
                x += lasth;
            } else  {
                ue = f0sol(x + h, u_0);
                u2 = rkh(parameter, x, u, h, f0); 
                x += h;
            }
            if (maxdev < abs(ue - u1)) { maxdev = abs(ue - u1); xmaxdev = x; }
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "," << ue << "," << abs(ue - u1) << "\n";
            
            if (h < 0.0000001) break;
        }
        summary << i + 1 << "," << b - x << "," << maxele << "," << C1 << "," << C2 << "," << maxh << "," << xmaxh << "," << minh << "," << xminh << "," << maxdev << "," << xmaxdev << "\n";
    }
}

void calculateTask2(double a, double b, double u, int Nmax, double epsilon, double epsilonb, double h, std::ofstream& outputFile, std::ofstream& summary, bool conststep) {
    double x = a;
    double parameter = 3;
    double u1 = u;
    double u2 = u;
    double ele = 0;
    int C1 = 0;
    int C2 = 0;
    double maxele = 0;
    double maxh = h;
    double xmaxh = a;
    double minh = h;
    double xminh = a;
    int i = 0;
    if (conststep == 1) {
        for (; i < Nmax; i++) {
            u1 = rk(parameter, x, u, h, f1);
            u2 = rkh(parameter, x, u, h, f1);

            ele = 16 * (abs(u1 - u2) / 15);
            if (maxele < ele) { maxele = ele; }
            if (x + h > b - epsilonb) {
                h = b - x - epsilonb / 2;
                u1 = rk(parameter, x, u, h, f1);
                u2 = rkh(parameter, x, u, h, f1);  
                x += h;
                outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
                break;
            }
            x += h;
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
        }
        summary << i + 1 << "," << b - x << "," << maxele << "\n";
    }
    else {
        for (; i < Nmax; i++) {
            double lasth = h;
            int c1 = C1;
            int c2 = C2;
            u1 = rkv(parameter, x, u, h, ele, epsilon, C1, C2, f1);
            
            if (maxele < ele) { maxele = ele; }
            if (maxh < h) { maxh = h; xmaxh = x; }
            if (minh > h) { minh = h; xminh = x; }
            if (x + h > b - epsilonb) {
                h = b - x - epsilonb / 2 ;
                u1 = rk(parameter, x, u, h, f1);
                u2 = rkh(parameter, x, u, h, f1);
                x += h;
                outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
                break;
            } else if (c1 == C1 - 1 && c2 == C2 - 1) {
                lasth = h / 2;
                u2 = rkh(parameter, x, u, lasth, f1);
                x += lasth;
            }
            else if (lasth < h){ 
                u2 = rkh(parameter, x, u, lasth, f1);
                x += lasth;
            }
            else { 
                u2 = rkh(parameter, x, u, h, f1);
                x += h;
            }
            u = u1;

            outputFile << i + 1 << "," << x << "," << u1 << "," << u2 << "," << u1 - u2 << "," << ele << "," << h << "," << C1 << "," << C2 << "\n";
            
            if (h < 0.0000001) break;
        }
        summary << i + 1 << "," << b - x << "," << maxele << "," << C1 << "," << C2 << "," << maxh << "," << xmaxh << "," << minh << "," << xminh << "\n";
    }
}

void calculateTask3(double parameter, double a, double b, double u, double du, int Nmax, double epsilon, double epsilonb, double h, std::ofstream& outputFile, std::ofstream& outputFile2, std::ofstream& summary, bool conststep) {
    double x = a;
    std::vector<double> ele = { 0, 0 };
    int C1 = 0;
    int C2 = 0;
    double maxele = 0;
    double maxh = h;
    double xmaxh = a;
    double minh = h;
    double xminh = a;
    int i = 0;
    std::vector<double> uv = { u, du };
    std::vector<double> uv1;
    if (conststep == 1) {
        for (; i < Nmax; i++) {
            uv1 = rk(parameter, x, uv, h, f2);
            std::vector<double> u1d = rkh(parameter, x, uv, h, f2);

            for (int i = 0; i < uv1.size(); i++) {
                ele[i] = 16 * (abs(uv1[i] - u1d[i]) / 15);
                if (maxele < ele[i]) { maxele = ele[i]; }
            }
            if (x + h > b - epsilonb) {
                h = b - x - epsilonb / 2;
                uv1 = rk(parameter, x, uv, h, f2);
                u1d = rkh(parameter, x, uv, h, f2);
                x += h;
                outputFile2 << i + 1 << "," << x << "," << uv1[0] << "," << u1d[0] << "," << uv1[0] - u1d[0] << "," << ele[0] << "," << h << "," << C1 << "," << C2 << "\n";
                outputFile << i + 1 << "," << x << "," << uv1[1] << "," << u1d[1] << "," << uv1[1] - u1d[1] << "," << ele[1] << "," << h << "," << C1 << "," << C2 << "\n";
                break;
            }
            x += h;
            uv = uv1;
            
            outputFile2 << i + 1 << "," << x << "," << uv1[0] << "," << u1d[0] << "," << uv1[0] - u1d[0] << "," << ele[0] << "," << h << "," << C1 << "," << C2 << "\n";
            outputFile << i + 1 << "," << x << "," << uv1[1] << "," << u1d[1] << "," << uv1[1] - u1d[1] << "," << ele[1] << "," << h << "," << C1 << "," << C2 << "\n";
        }
        summary << i + 1 << "," << b - x << "," << maxele << "\n";
    }
    else {
        for (; i < Nmax; i++) {
            double lasth = h;  
            int c1 = C1;
            int c2 = C2;
            uv1 = rkv(parameter, x, uv, h, ele, epsilon, C1, C2, f2);
            std::vector<double> u1d(uv1.size());
            for (int i = 0; i < uv1.size(); i++) {
                if (maxele < ele[i]) { maxele = ele[i]; }
            }
            if (maxh < h) { maxh = h; xmaxh = x; }
            if (minh > h) { minh = h; xminh = x; }
            if (x + h > b - epsilonb) {
                h = b - x - epsilonb / 2;
                uv1 = rk(parameter, x, uv, h, f2);
                u1d = rkh(parameter, x, uv, h, f2);
                x += h;
                outputFile2 << i + 1 << "," << x << "," << uv1[0] << "," << u1d[0] << "," << uv1[0] - u1d[0] << "," << ele[0] << "," << h << "," << C1 << "," << C2 << "\n";
                outputFile << i + 1 << "," << x << "," << uv1[1] << "," << u1d[1] << "," << uv1[1] - u1d[1] << "," << ele[1] << "," << h << "," << C1 << "," << C2 << "\n";
                break;
            } else if (c1 == C1 - 1 && c2 == C2 - 1) {
                lasth = h / 2;
                u1d = rkh(parameter, x, uv, lasth, f2);
                x += lasth;
            }
            if (lasth < h) {
                u1d = rkh(parameter, x, uv, lasth, f2);
                x += lasth;
            }
            else {
                u1d = rkh(parameter, x, uv, h, f2);
                x += h;
            }
            uv = uv1;

            outputFile2 << i + 1 << "," << x << "," << uv1[0] << "," << u1d[0] << "," << uv1[0] - u1d[0] << "," << ele[0] << "," << h << "," << C1 << "," << C2 << "\n";
            outputFile << i + 1 << "," << x << "," << uv1[1] << "," << u1d[1] << "," << uv1[1] - u1d[1] << "," << ele[1] << "," << h << "," << C1 << "," << C2 << "\n";
            
            if (h < 0.0000001) break;
        }
        summary << i + 1 << "," << b - x << "," << maxele << "," << C1 << "," << C2 << "," << maxh<< "," << xmaxh << "," << minh << "," << xminh << "\n";
    }
}

int main()
{
    double a, b, epsilon, h, u, du, parameter, epsilonb;
    int Nmax;
    int task;
    bool conststep;

    std::ifstream paramsFile("input_params.txt");
    if (!paramsFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    paramsFile >> a >> b >> u >> epsilon >> epsilonb >> h >> Nmax >> task >> conststep >> du >> parameter;

    std::ofstream outputFile("results.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    std::ofstream outputFile2("results2.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    std::ofstream summary("summary.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    if (task == 1) {
        calculateTask1(a, b, u, Nmax, epsilon, epsilonb, h, outputFile, summary, conststep);
    }
    else if (task == 2) {
        calculateTask2(a, b, u, Nmax, epsilon, epsilonb, h, outputFile, summary, conststep);
    }
    else if (task == 3) {
        calculateTask3(parameter, a, b, u, du, Nmax, epsilon, epsilonb, h, outputFile, outputFile2, summary, conststep);
    }
    outputFile.close();
    return 0;
}