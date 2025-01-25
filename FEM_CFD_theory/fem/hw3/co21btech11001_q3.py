"""
Solution of Question 3(f) of Homework 1.
Run this script using the following command:
python co21btech11001_q3.py
"""

import numpy as np

def gauss_integration(f, deg, a, b):
    """
    Gauss integration of function f(x) with degree deg from a to b
    """
    # n_gp >= (deg + 1) / 2
    n_gp = (deg + 2) // 2
    zeta, W = np.polynomial.legendre.leggauss(n_gp)
    x = (a + b)/2 + ((b-a)/2)*zeta
    fx = f(x)
    J = (b-a)/2
    return float(J*np.dot(W, fx))

def main():
    # Degree of the polynomial
    degree = 4
    # Quadrature points
    n_gp = (degree + 2) // 2
    # Polynomial function f(x)
    f = lambda x: (x**4 + 2*(x**2))
    # Domain
    a = -1; b = 1

    # Gauss integration
    result = gauss_integration(f, degree, a, b)

    print(f"Integral of f(x) = x^4 + 2x^2 from {a} to {b} using Gauss integration with {n_gp} quadrature points: {result}")

if __name__ == "__main__":
    main()