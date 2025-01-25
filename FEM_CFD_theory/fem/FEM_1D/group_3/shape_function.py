import numpy as np

def shape_fn(nen, zeta, le):
    if nen == 2:
        Nu1 = 0.25 * ((1-zeta)**2) * (2 + zeta)
        Nth1 = (le / 8) * ((1-zeta)**2) * (1+zeta)
        Nu2 = 0.25 * ((1+zeta)**2) * (2 - zeta)
        Nth2 = (le / 8) * ((1+zeta)**2) * (zeta-1)

        N = np.array([Nu1, Nth1, Nu2, Nth2])

        d2N = (1/le) * np.array([
            6*zeta/le,
             3*zeta - 1,
             -6*zeta/le,
             3*zeta + 1])

        return N, d2N
    else:
        raise NotImplementedError("Only 2-noded elements are supported")