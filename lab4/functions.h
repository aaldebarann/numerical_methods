#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <math.h>

class Functions {
public:
  static const int FIRST = 0;
  static const int SECOND = 1;
  static const int THIRD = 2;
  static const int TEST = 3;
  
  static double test(double x) {
    double s;
    if (per)
      s = cos(10.0 * x);
    else
      s = 0;
    if (x <= 0) {
      return pow(x, 3.0) + 3.0 * pow(x, 2.0) + s;
    }
    else {
      return -pow(x, 3.0) + 3.0 * pow(x, 2.0) + s;
    }
  }

  static double test_der(double x) {
    double s;
    if (per)
      s = -10.0 * sin(10.0 * x);
    else
      s = 0;
    if (x <= 0) {
      return 3.0 * pow(x, 2.0) + 6.0 * x + s;
    }
    else {
      return -3.0 * pow(x, 2.0) + 6.0 * x + s;
    }
  }

  static double test_der_der(double x) {
    double s;
    if (per)
      s = -100.0 * cos(10.0 * x);
    else
      s = 0;
    if (x <= 0) {
      return 6.0 * x + 6.0 + s;
    }
    else {
      return -6.0 * x + 6.0 + s;
    }
  }

  static double f1(double x) {
    double s;
    if (per)
      s = cos(10.0 * x);
    else
      s = 0;
    return sqrt(pow(x, 2) - 1) / x + s;
  }

  static double f2(double x) {\
    double s;
    if (per)
      s = cos(10.0 * x);
    else
      s = 0;
    return pow((pow(x, 2) + 1), 1.0 / 3.0) + s;
  }

  static double f3(double x) {
    double s;
    if (per)
      s = cos(10.0 * x);
    else
      s = 0;
    return sin(x + 1) / (x + 1) + s;
  }

  static double f1_der(double x) {
    double s;
    if (per)
      s = -10.0 * sin(10.0 * x);
    else
      s = 0;
    return 1.0 / sqrt(pow(x, 2.0) - 1) - sqrt(pow(x, 2.0) - 1) / pow(x, 2.0) + s;
  }

  static double f2_der(double x) {
    double s;
    if (per)
      s = -10.0 * sin(10.0 * x);
    else
      s = 0;
    return 2.0 / 3.0 * x / pow(1 + pow(x, 2.0), 2.0 / 3.0) + s;
  }

  static double f3_der(double x) {
    double s;
    if (per)
      s = -10.0 * sin(10.0 * x);
    else
      s = 0;
    return cos(x + 1) / (x + 1) - sin(x + 1) / pow(x + 1, 2.0) + s;
  }

  static double f1_der_der(double x) {
    double s;
    if (per)
      s = -100.0 * cos(10.0 * x);
    else
      s = 0;
    return - x / pow(pow(x, 2.0) - 1, 2.0 / 3.0) - 1.0 / (x * sqrt(pow(x, 2.0) - 1)) + 2.0 * sqrt(pow(x, 2.0) - 1) / pow(x, 3.0) + s;
  }

  static double f2_der_der(double x) {
    double s;
    if (per)
      s = -100.0 * cos(10.0 * x);
    else
      s = 0;
    return 2.0 / (3.0 * pow(1 + pow(x, 2.0), 2.0 / 3.0)) - 8.0 / 9.0 * pow(x, 2.0) / pow(1 + pow(x, 2.0), 5.0 / 3.0) + s;
  }

  static double f3_der_der(double x) {
    double s;
    if (per)
      s = -100.0 * cos(10.0 * x);
    else
      s = 0;
    return -sin(x + 1) / (x + 1) - 2 * cos(x + 1) / pow(x + 1, 2.0) + 2 * sin(x + 1) / pow(x + 1, 3.0) + s;
  }

  static double a;
  static double b;

  static double a_s[4];
  static double b_s[4];

  static bool per;

  static double (*f)(double x);
  static double (*f_der)(double x);
  static double (*f_der_der)(double x);

  Functions() {}

  static void init() {
    
    per = false;
    a_s[0] = 2.0;
    a_s[1] = 0.0;
    a_s[2] = 0.0;
    a_s[3] = -1.0;
    b_s[0] = 4.0;
    b_s[1] = 1.0;
    b_s[2] = 2.0;
    b_s[3] = 1.0;
    set_task(TEST);
  }

  static void set_task(int task) {
    a = a_s[task];
    b = b_s[task];

    switch (task) {
    case FIRST:
      f = f1;
      f_der = f1_der;
      f_der_der = f1_der_der;
      break;
    case SECOND:
      f = f2;
      f_der = f2_der;
      f_der_der = f2_der_der;
      break;
    case THIRD:
      f = f3;
      f_der = f3_der;
      f_der_der = f3_der_der;
      break;
    case TEST:
      f = test;
      f_der = test_der;
      f_der_der = test_der_der;
      break;
    }
  }

};


#endif // FUNCTIONS_H