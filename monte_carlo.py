# -*- coding: utf-8 -*-
import scipy
import numpy as np
import matplotlib.pyplot as plt

"""### Дз: Интегрирование параболы методом Монте-Карло:"""

# Функция параболы
def fun(x):
  return -x**2 + 3*x -2

# Функция интегрирования для заданного числа точек
def integrate(q0):
    x = np.linspace(1, 2, q0) # Для истинного значения параболы на графике
    y = fun(x)                # Для истинного значения параболы на графике
    x_a = np.random.uniform(1, 2, q0)    # Случайная координата по x
    y_a = np.random.uniform(0, 0.25, q0) # Случайная координата по y
    xr = []
    yr = []
    xb = []
    yb = []
    num = 0
    for i in range(q0):
        if(y_a[i] < fun(x_a[i])): # Окрашиваем и считаем попавшие точки
            xr.append(x_a[i])
            yr.append(y_a[i])
            num += 1
        else:
            xb.append(x_a[i])
            yb.append(y_a[i])
    num = 0.25 * num / q0
    sol.append(num)
    plt.title(f"N = {q0}")
    plt.plot(x, y, 'r-') # Строим параболу
    plt.scatter(xr, yr, color = 'r', marker = '.') # Строим красные точки
    plt.scatter(xb, yb, marker = '.')              # Строим синие точки


fig, axs = plt.subplots(2, 2, figsize=(6, 6))
Nm = [2000, 100, 500, 1000]
sol = []

for i in range(4):
    N = Nm[i]
    ax = axs[i // 2, i % 2]  # Получаем текущий активный subplot
    integrate(N)
    plt.sca(ax)
plt.tight_layout()  # Улучшает расположение графиков в сетке
plt.show()

print("Решение методом монте карло:", sol[1], sol[2], sol[3], sol[0])
print("Аналитическое решение: 0.166666")