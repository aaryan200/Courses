% Aaryan
% CO21BTECH11001
clc;
clear;

m1 = 1.0;       % Mass of m1 in kg
m2 = 1.0;       % Mass of m2 in kg
k = 1000.0;       % Spring stiffness in N/m
c = 5.0;        % Damping coefficient in Ns/m
l = 0.5;      % Free length of the spring in m
g = 9.81;     % Acceleration due to gravity in m/s^2

% Initial conditions
x1_initial = 0.0;
y1_initial = 0.0;
vx1_initial = 0.0;
vy1_initial = 1.0;

x2_initial = 0.5;
y2_initial = 0.0;
vx2_initial = 0.0;
vy2_initial = -1.0;

% Time step
dt = 1.0e-5;

% Number of time steps
num_steps = 10000;

% Time array
time = (0:num_steps-1) * dt;

% Initialize arrays to store positions and velocities
x1 = zeros(1, num_steps);
y1 = zeros(1, num_steps);
vx1 = zeros(1, num_steps);
vy1 = zeros(1, num_steps);

x2 = zeros(1, num_steps);
y2 = zeros(1, num_steps);
vx2 = zeros(1, num_steps);
vy2 = zeros(1, num_steps);

% Initialize distance between masses array
distance_between_masses = zeros(1, num_steps);

% Initialize kinetic energy, spring potential energy, gravitational potential energy and total energy array
kinetic_energy = zeros(1, num_steps);
spring_pot_energy = zeros(1, num_steps);
grav_pot_energy = zeros(1, num_steps);
total_energy = zeros(1, num_steps);

% Set initial conditions
x1(1) = x1_initial;
y1(1) = y1_initial;
vx1(1) = vx1_initial;
vy1(1) = vy1_initial;

x2(1) = x2_initial;
y2(1) = y2_initial;
vx2(1) = vx2_initial;
vy2(1) = vy2_initial;

% Fill initial values of distance and energies
distance_between_masses(1) = sqrt((x2(1) - x1(1))^2 + (y2(1) - y1(1))^2);
kinetic_energy(1) = 0.5*m1*((vx1(1))^2 + (vy1(1))^2) + 0.5*m2*((vx2(1))^2 + (vy2(1))^2);
spring_pot_energy(1) = 0.5*k*((distance_between_masses(1) - l)^2);
grav_pot_energy(1) = m1*g*y1(1) + m2*g*y2(1);
total_energy(1) =  kinetic_energy(1) + spring_pot_energy(1) + grav_pot_energy(1);


% Using Euler's explicit scheme
for i = 1:num_steps-1
    % Forces
    spring_force = k * (distance_between_masses(i) - l);
    damper_force_x = c * (vx2(i) - vx1(i));
    damper_force_y = c * (vy2(i) - vy1(i));
    
    % Accelerations
    ax1 = (spring_force * (x2(i) - x1(i))) / (m1 * distance_between_masses(i)) + damper_force_x / m1;
    ay1 = (spring_force * (y2(i) - y1(i))) / (m1 * distance_between_masses(i)) + damper_force_y / m1 - g;
    
    ax2 = (spring_force * (x1(i) - x2(i))) / (m2 * distance_between_masses(i)) - damper_force_x / m2;
    ay2 = (spring_force * (y1(i) - y2(i))) / (m2 * distance_between_masses(i)) - damper_force_y / m2 - g;
    
    % Update velocities and positions of m1
    vx1(i+1) = vx1(i) + ax1 * dt;
    vy1(i+1) = vy1(i) + ay1 * dt;
    
    x1(i+1) = x1(i) + vx1(i+1) * dt;
    y1(i+1) = y1(i) + vy1(i+1) * dt;
    
    % Update velocities and positions of m2
    vx2(i+1) = vx2(i) + ax2 * dt;
    vy2(i+1) = vy2(i) + ay2 * dt;
    
    x2(i+1) = x2(i) + vx2(i+1) * dt;
    y2(i+1) = y2(i) + vy2(i+1) * dt;

    % Update distance
    distance_between_masses(i+1) = sqrt((x2(i+1) - x1(i+1))^2 + (y2(i+1) - y1(i+1))^2);

    % Update energies
    kinetic_energy(i+1) = 0.5*m1*((vx1(i+1))^2 + (vy1(i+1))^2) + 0.5*m2*((vx2(i+1))^2 + (vy2(i+1))^2);
    spring_pot_energy(i+1) = 0.5*k*((distance_between_masses(i+1) - l)^2);
    grav_pot_energy(i+1) = m1*g*y1(i+1) + m2*g*y2(i+1);
    total_energy(i+1) =  kinetic_energy(i+1) + spring_pot_energy(i+1) + grav_pot_energy(i+1);
end

% Round the decimal numbers

ROUND = 3;  

for i = 1:num_steps
    kinetic_energy(i) = round(kinetic_energy(i), ROUND);
    spring_pot_energy(i) = round(spring_pot_energy(i), ROUND);
    grav_pot_energy(i) = round(grav_pot_energy(i), ROUND);
    total_energy(i) = round(total_energy(i), ROUND);
end

% Plot total energy vs time graph
plot_energy(time, total_energy);

% Uncomment below line to plot x1, y1, x2, y2, vx1, vy1, vx2, vy2 in one plot
% plot_x_y(time, x1, y1, x2, y2, vx1, vy1, vx2, vy2);

function plot_energy(time, total_energy)
    figure;
    plot(time, total_energy, 'b');
    xlabel('t');
    ylabel('Total Energy');
    title('Total Energy vs. Time');
end

function plot_x_y(time, x1, y1, x2, y2, vx1, vy1, vx2, vy2)
    figure;

    % x vs time plot
    subplot(2,2,1);
    plot(time, x1, 'b', time, x2, 'r');
    xlabel('t');
    ylabel('x');
    legend('m1', 'm2', 'Location','southeast');
    title('x vs. Time ');

    % y vs time plot
    subplot(2,2,2);
    plot(time, y1, 'b', time, y2, 'r');
    xlabel('t');
    ylabel('y');
    legend('m1', 'm2', 'Location','southwest');
    title('y vs. Time');
    
    % vx vs time plot
    subplot(2,2,3);
    plot(time, vx1, 'b', time, vx2, 'r');
    xlabel('t');
    ylabel('vx');
    legend('m1', 'm2', 'Location','northwest');
    title('vx vs. Time');
    
    % vy vs time plot
    subplot(2,2,4);
    plot(time, vy1, 'b', time, vy2, 'r');
    xlabel('t');
    ylabel('vy');
    legend('m1', 'm2', 'Location','northeast');
    title('vy vs. Time');
end
