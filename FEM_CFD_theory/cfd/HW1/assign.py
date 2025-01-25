import numpy as np
import matplotlib.pyplot as plt

#### Parameters
M = 40 # Number of grid points
delta_x = 1.0/(M-1) # Grid spacing
delta_t = 0.0002 # Time step

def solve_1d_heat(biot_number, fourier_number_max):
    """
    Solve 1D unsteady heat equation using explicit FTCS scheme.
    """
    # Initial condition
    theta = np.ones(M)
    theta_new = np.zeros(M)

    num_steps = int(fourier_number_max / delta_t)

    output = np.zeros((num_steps + 1, M))
    output[0, :] = theta

    c = delta_t / (delta_x**2)

    for t in range(num_steps):
        for i in range(1, M-1):
            theta_new[i] = theta[i] + c * (theta[i+1] - 2*theta[i] + theta[i-1])

        # Boundary conditions
        theta_new[0] = theta_new[1]
        theta_new[M-1] = theta_new[M-2] / (1 + biot_number * delta_x)

        # Update theta
        theta[:] = theta_new
        output[t + 1, :] = theta

    return output

#### Problem A
biot_numbers = [0.2, 1.0, 5.0, 10.0]

analytical_timesteps = [0.2, 0.4, 0.6, 0.8, 1.0]
# Analytical solution from Heisler chart
heisler_chart_sol = np.array([
    [0.99, 0.96, 0.91, 0.89, 0.86], # Bi = 0.2
    [0.96, 0.84, 0.72, 0.62, 0.54], # Bi = 1.0
    [0.93, 0.64, 0.44, 0.29, 0.24], # Bi = 5.0
    [0.82, 0.52, 0.33, 0.28, 0.18] # Bi = 10.0
])

colors = ['b', 'orange', 'g', 'r']

plt.figure(figsize=(10, 6))

for i, biot_number in enumerate(biot_numbers):
    result = solve_1d_heat(biot_number, fourier_number_max = 10)
    # Plot the center temperature variation for this biot number
    t = np.linspace(0, 10, result.shape[0], endpoint=True)
    plt.plot(t, result[:, 0], label=f'Numerical $B_i={biot_number}$', color=colors[i])

    # Plot the analytical solution
    plt.plot(analytical_timesteps, heisler_chart_sol[i],
                linestyle='--', color=colors[i], 
                label=f'Analytical $B_i={biot_number}$')

plt.xlabel('Fourier number')
plt.ylabel('Temperature')
plt.title('Center temperature variation')
plt.legend()
plt.grid()
plt.savefig('center_temperature_variation.png')


#### Problem B
fourier_numbers = [2, 4]
biot_numbers = [0.2, 1.0, 5.0, 10.0]

heisler_chart_soln = np.array([
    [0.66, 0.23, 0.02, 0.017],
    [0.44, 0.06, 0.002, 0.0005]
])

plt.figure(figsize=(10, 6))

for i, f in enumerate(fourier_numbers):
    temp_profile = [] # Store the temperature profile at x/L = 0.5 at final time step
    for biot_number in biot_numbers:
        result = solve_1d_heat(biot_number, fourier_number_max = f)
        temp_profile.append(result[-1, M//2])
    plt.plot(biot_numbers, temp_profile, label=f'$F_o={f}$ (Numerical)', color=colors[i])
    plt.plot(biot_numbers, heisler_chart_soln[i], linestyle='--', color=colors[i], label=f'$F_o={f}$ (Analytical)')

plt.xlabel('Biot number')
plt.ylabel('Temperature')
plt.title('Temperature variation at $\\frac{x}{L} = 0.5$')
plt.legend()
plt.grid()
plt.savefig('temperature_variation_half.png')