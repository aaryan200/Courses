"""
Solution of Question 5 of Homework 1.
Run this script using the following command:
python co21btech11001_q5.py
"""

import numpy as np

arr = np.array([[-1, 3, -2], [2, -4, 2], [0, 4, 1]], dtype=np.float64)

# Print the matrix
print("Matrix:")
print(arr)

ZERO_THRESHOLD = 1e-10

def check_close(val, compare_with = 0.0):
    """
    Return True if the value is close to `compare_with` within a threshold.
    """
    return abs(val - compare_with) < ZERO_THRESHOLD

def determinant(arr: np.ndarray):
    """
    Calculate determinant of a 2d matrix using row reduction method.
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
    
    # Return the product of the diagonal elements
    return np.prod(np.diag(brr))

def inverse_adjoint(arr: np.ndarray):
    """
    Calculate inverse of a matrix using adjoin method.
    """
    det_a = determinant(arr)
    if check_close(det_a):
        raise ValueError("Matrix is singular")
    
    n = arr.shape[0]
    a_inv = np.zeros_like(arr, dtype=np.float64)

    for i in range(n):
        for j in range(n):
            # Hide the ith row and jth column to form a submatrix
            b = np.delete(arr, i, axis=0)
            b = np.delete(b, j, axis=1)

            # Calculate cofactor corresponding to a[i][j]
            cofactor = ((-1)**(i + j)) * determinant(b)

            # Adjoint matrix is the transpose of cofactor matrix
            # Inverse = Adjoint / det(A)
            a_inv[j, i] = cofactor / det_a
    
    return a_inv

def inverse_row_reduction(arr: np.ndarray):
    """
    Calculate inverse of a matrix using row reduction method.
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

# Calculate inverse of the matrix
arr_inv_adj = inverse_adjoint(arr)
print("\nInverse using adjoint method:")
print(arr_inv_adj)

arr_inv_row_red = inverse_row_reduction(arr)
print("\nInverse using row reduction method:")
print(arr_inv_row_red)