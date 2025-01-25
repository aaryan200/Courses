import numpy as np
import matplotlib.pyplot as plt

# Parameters
M = 64 # Number of grid points in x-direction
N = 64 # Number of grid points in y-direction
delta_x = 1.0/(M-1) # Grid spacing in x-direction
delta_y = 1.0/(N-1) # Grid spacing in y-direction

def solve_stream(omega, psi, tolerance=1e-6):
    """
    Solve the stream function using the vorticity field
    """
    # Solve stream function iteratively until convergence
    while True:
        psi_new = psi.copy()
        psi_new[1:-1, 1:-1] = 0.25 * (
            omega[1:-1, 1:-1] * delta_y**2 +
            psi[0:-2, 1:-1] + psi[2:, 1:-1] +
            psi[1:-1, 0:-2] + psi[1:-1, 2:]
        )
        if np.max(np.abs(psi_new - psi)) < tolerance:
            psi = psi_new
            break
        psi = psi_new
    return psi


def solve_lid_problem(u, v, omega, psi,
                      num_iters, upwind_scheme_order,
                      re = 100.0, delta_t = 0.001):
    """
    Solve the lid-driven cavity problem using the vorticity-stream function formulation
    """
    for it in range(num_iters):
        # Obtain wall vorticities
        ## Bottom wall
        omega[:, 0] = -2*(psi[:, 1] - psi[:, 0]) / (delta_y**2)
        ## Left wall
        omega[0, :] = -2*(psi[1, :] - psi[0, :]) / (delta_x**2)
        ## Right wall
        omega[-1, :] = -2*(psi[-2, :] - psi[-1, :]) / (delta_x**2)
        ## Top wall
        omega[:, -1] = -2*(psi[:, -2] - psi[:, -1] + delta_y) / (delta_y**2)

        # Obtain interior vorticities
        omega_new = omega.copy()
        for i in range(1, M-1):
            for j in range(1, N-1):
                rhs = (1/re) * (
                    (omega[i+1, j] - 2*omega[i, j] + omega[i-1, j]) / (delta_x**2) +
                    (omega[i, j+1] - 2*omega[i, j] + omega[i, j-1]) / (delta_y**2)
                )

                if upwind_scheme_order == 'first':
                    non_linear_term = (
                        u[i, j] * (omega[i, j] - omega[i-1, j]) / delta_x +
                        v[i, j] * (omega[i, j] - omega[i, j-1]) / delta_y
                    )
                elif upwind_scheme_order == 'second':
                    non_linear_term = (
                        u[i, j] * (3*omega[i, j] - 4*omega[i-1, j] + omega[i-2, j]) / (2*delta_x) +
                        v[i, j] * (3*omega[i, j] - 4*omega[i, j-1] + omega[i, j-2]) / (2*delta_y)
                    )

                rhs = rhs - non_linear_term
                omega_new[i, j] = omega[i, j] + delta_t * rhs

        omega = omega_new

        # Obtain stream function
        psi = solve_stream(omega, psi)

        ## Compute x-velocity (u) and y-velocity (v)
        u[1:-1, 1:-1] = (psi[1:-1, 2:] - psi[1:-1, 0:-2]) / (2 * delta_y)
        v[1:-1, 1:-1] = -(psi[2:, 1:-1] - psi[0:-2, 1:-1]) / (2 * delta_x)

    # Plot vorticity and stream function
    plt.figure(figsize=(10, 5))
    plt.contourf(omega.T, cmap='plasma')
    plt.colorbar()
    plt.title('Vorticity')
    plt.show()

    plt.close()

    plt.figure(figsize=(10, 5))
    plt.contourf(psi.T, cmap='jet')
    plt.colorbar()
    plt.title('Stream function')
    plt.show()

# Solving for Re = 100

# Initial conditions
u_100 = np.zeros((M,N))
v_100 = np.zeros((M,N))
omega = np.zeros((M,N)) # Vorticity
psi = np.zeros((M,N)) # Stream function
psi.fill(5.0) # Constant value of psi


re = 100.0 # Reynolds number

delta_t = 0.001 # Time step

num_iters = 5000

# Solve using first order upwind scheme
solve_lid_problem(u_100, v_100, omega, psi, num_iters, upwind_scheme_order = 'first',
                  re = re, delta_t = delta_t)
# Solve using second order upwind scheme
solve_lid_problem(u_100, v_100, omega, psi, num_iters, upwind_scheme_order = 'second',
                  re = re, delta_t = delta_t)

# Solving for Re = 400

# Initial conditions
u_400 = np.zeros((M,N))
v_400 = np.zeros((M,N))
omega = np.zeros((M,N)) # Vorticity
psi = np.zeros((M,N)) # Stream function
psi.fill(5.0) # Constant value of psi


re = 400.0 # Reynolds number

delta_t = 0.001 # Time step

num_iters = 5000

# Solve using first order upwind scheme
solve_lid_problem(u_400, v_400, omega, psi, num_iters, upwind_scheme_order = 'first',
                    re = re, delta_t = delta_t)
# Solve using second order upwind scheme
solve_lid_problem(u_400, v_400, omega, psi, num_iters, upwind_scheme_order = 'second',
                    re = re, delta_t = delta_t)

# Solving for Re = 1000

# Initial conditions
u_1000 = np.zeros((M,N))
v_1000 = np.zeros((M,N))
omega = np.zeros((M,N)) # Vorticity
psi = np.zeros((M,N)) # Stream function
psi.fill(5.0) # Constant value of psi


re = 1000.0 # Reynolds number

delta_t = 0.00005 # Time step is lesser for higher Reynolds number

num_iters = 6000

# Solve using first order upwind scheme
solve_lid_problem(u_1000, v_1000, omega, psi, num_iters, upwind_scheme_order = 'first',
                    re = re, delta_t = delta_t)
# Solve using second order upwind scheme
solve_lid_problem(u_1000, v_1000, omega, psi, num_iters, upwind_scheme_order = 'second',
                    re = re, delta_t = delta_t)

# Plot the horizontal velocity fields at x = 0.5 and vertical velocity fields at y = 0.5
plt.figure(figsize=(10, 5))
x = np.linspace(0, 1, M)
y = np.linspace(0, 1, N)
plt.plot(y, u_100[int(M/2), :], label='Re = 100')
plt.plot(y, u_400[int(M/2), :], label='Re = 400')
plt.plot(y, u_1000[int(M/2), :], label='Re = 1000')
plt.xlabel('y')
plt.ylabel('u')
plt.title('Horizontal velocity at $x = \\frac{L}{2}$')
plt.grid()
plt.legend()
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(x, v_100[:, int(N/2)], label='Re = 100')
plt.plot(x, v_400[:, int(N/2)], label='Re = 400')
plt.plot(x, v_1000[:, int(N/2)], label='Re = 1000')
plt.xlabel('x')
plt.ylabel('v')
plt.title('Vertical velocity at $y = \\frac{L}{2}$')
plt.grid()
plt.legend()
plt.show()