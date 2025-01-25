"""
Solution of Question 8 of Homework 1.
Run this script using the following command:
python co21btech11001_q8.py
"""
import numpy as np

ZERO_THRESHOLD = 1e-10

def check_close(val, compare_with = 0.0):
    """
    Return True if the value is close to `compare_with` within a threshold.
    """
    return abs(val - compare_with) < ZERO_THRESHOLD

def determinant(arr: np.ndarray):
    """
    Calculate determinant of a 2d arrrix using row redupr_sizeion method.
    """
    n = arr.shape[0]
    brr = arr.copy()

    for i in range(n):
        # Take the ith diagonal element as pivot
        # And make all the elements below it zero
        for j in range(i+1, n):
            if check_close(brr[j, i]):
                continue
            
            if check_close(brr[i, i]):
                # R[i] -> R[i] + R[j]
                brr[i, :] += brr[j, :]
            
            # R[j] -> R[j] - (a[j][i] / a[i][i]) * R[i]
            brr[j, :] -= (brr[j, i] / brr[i, i]) * brr[i, :]
    
    # Return the produpr_size of the diagonal elements
    return np.prod(np.diag(brr))

def inverse_row_reduction(arr: np.ndarray):
    """
    Calculate inverse of a matrix using row redupr_sizeion method.
    """
    # Augment the matrix with identity matrix horizontally
    n = arr.shape[0]
    a_aug = np.hstack((arr, np.eye(n)))

    for i in range(n):
        # Take the ith diagonal element as pivot and convert it to 1
        # And make all the elements below it zero
        for j in range(i+1, n):
            if check_close(a_aug[j, i]):
                continue
            
            if check_close(a_aug[i, i]):
                # R[i] -> R[i] + R[j]
                a_aug[i, :] += a_aug[j, :]

            # Convert the pivot to 1 if it is not already
            if not check_close(a_aug[i, i], 1.0):
                a_aug[i, :] /= a_aug[i, i]
            
            # R[j] -> R[j] - a[j][i] * R[i]
            a_aug[j, :] -= a_aug[j, i] * a_aug[i, :]

    # Make all the elements above the diagonal zero
    for i in range(n-1, -1, -1):
        for j in range(i-1, -1, -1):
            if check_close(a_aug[j, i]):
                continue
            
            if check_close(a_aug[i, i]):
                # R[i] -> R[i] + R[j]
                a_aug[i, :] += a_aug[j, :]

            # Convert the pivot to 1 if it is not already
            if not check_close(a_aug[i, i], 1.0):
                a_aug[i, :] /= a_aug[i, i]
            
            # R[j] -> R[j] - a[j][i] * R[i]
            a_aug[j, :] -= a_aug[j, i] * a_aug[i, :]

    if n == 1:
        if check_close(a_aug[0, 0]):
            raise ValueError("Matrix is singular")
        return np.array([[1 / a_aug[0, 0]]], dtype=np.float64)
    
    # Check if the left part is identity matrix
    if not np.allclose(a_aug[:, :n], np.eye(n)):
        raise ValueError("Matrix is singular")
    
    # Return the right part of the augmented matrix
    return a_aug[:, n:]


def disp_solve(arr: np.ndarray, displacement: np.ndarray, force: np.ndarray):
    disp_len = displacement.shape[0]

    # present is a list of indices of displacements that are present
    present = []
    non_present = []
    for i in range(disp_len):
        if displacement[i] == None:
            present.append(i)
        else:
            non_present.append(i)

    pr_size = len(present)

    temp_arr = np.zeros((pr_size, pr_size))

    temp_b = np.zeros((pr_size, 1))

    # Create a temporary matrix and vector with only the present displacements
    for i in range(pr_size):
        for j in range(pr_size):
            temp_arr[i][j] = arr[present[i]][present[j]]

    # Create a temporary vector with only the present displacements
    for i in range(pr_size):
        temp_b[i] = force[present[i]]
        for j in non_present:
            temp_b[i] -= arr[present[i]][j] * displacement[j]

    # Calculate the present displacements
    temp_u = np.matmul(inverse_row_reduction(temp_arr), temp_b)

    for i in range(pr_size):
        displacement[present[i]] = temp_u[i][0]
    
    return displacement

def force_solve(arr: np.ndarray, displacement: np.ndarray):
    disp_len = displacement.shape[0]
    force = np.zeros((disp_len, 1))

    for i in range(disp_len):
        force[i] = np.matmul(arr[i], displacement)

    return force

# Stiffness matrix
arr = np.array([[500, 0, -500, 0],
              [0, 500, -500, 0],
              [-500, -500, 2000, -1000],
              [0, 0, -1000, 1000]], dtype=np.float64)

# Displacement vector
displacement = np.array([0, 0, None, 0])

# Force vector
force = np.array([
    [None],
    [None],
    [4],
    [None]
])

displacement = disp_solve(arr, displacement, force)

force = force_solve(arr, displacement)

print("Displacement vector:")
print(displacement)

print("\nForce vector:")
print(force)