from compute_stiffness_force import global_stiffness_force
from readInput import readInputData
from applyBC import apply_bc
from make_plots import plot_deflection, plot_stress_profile
import numpy as np
import sys

def main():
    # Read input file name from arguments
    # take input file name from command line
    if len(sys.argv) != 2:
        print("Usage: python main.py <input_file>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    data = readInputData(input_file)

    K, F = global_stiffness_force(data)

    # Apply boundary conditions
    K_t, F_t = apply_bc(K, F, data['Prescribed_DOFs'])

    # Solve for u
    u = np.linalg.solve(K_t, F_t)

    # Plot deflection
    plot_deflection(data['Nodal_coords'], u[::2])

    # Plot stress profile
    plot_stress_profile(data, F)
    
if __name__ == '__main__':
    main()