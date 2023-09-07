% Define system parameters
m1 = 1;       % Mass of m1 in kg
m2 = 1;       % Mass of m2 in kg
k = 1000;     % Spring stiffness in N/m
c = 5;        % Damping coefficient in Ns/m
l = 0.5;      % Free length of the spring in m
g = 9.81;     % Acceleration due to gravity in m/s^2

% Initial conditions
x1_initial = 0;
y1_initial = 0;
vx1_initial = 0;
vy1_initial = 1;

x2_initial = 1/2;
y2_initial = 0;
vx2_initial = 0;
vy2_initial = -1;

% Time step
dt = 1e-5;

% Number of time steps
num_steps = 10000;

% Initialize arrays to store positions and velocities
x1 = zeros(1, num_steps);
y1 = zeros(1, num_steps);
vx1 = zeros(1, num_steps);
vy1 = zeros(1, num_steps);

x2 = zeros(1, num_steps);
y2 = zeros(1, num_steps);
vx2 = zeros(1, num_steps);
vy2 = zeros(1, num_steps);

% Set initial conditions
x1(1) = x1_initial;
y1(1) = y1_initial;
vx1(1) = vx1_initial;
vy1(1) = vy1_initial;

x2(1) = x2_initial;
y2(1) = y2_initial;
vx2(1) = vx2_initial;
vy2(1) = vy2_initial;

% Euler explicit integration
for i = 1:num_steps-1
    % Calculate forces
    spring_force = k * (sqrt((x2(i) - x1(i))^2 + (y2(i) - y1(i))^2) - l);
    damping_force_x = c * (vx2(i) - vx1(i));
    damping_force_y = c * (vy2(i) - vy1(i));
    
    % Calculate accelerations
    ax1 = (spring_force * (x2(i) - x1(i))) / (m1 * sqrt((x2(i) - x1(i))^2 + (y2(i) - y1(i))^2)) - damping_force_x / m1;
    ay1 = (spring_force * (y2(i) - y1(i))) / (m1 * sqrt((x2(i) - x1(i))^2 + (y2(i) - y1(i))^2)) - damping_force_y / m1 + g;
    
    ax2 = (-spring_force * (x2(i) - x1(i))) / (m2 * sqrt((x2(i) - x1(i))^2 + (y2(i) - y1(i))^2)) + damping_force_x / m2;
    ay2 = (-spring_force * (y2(i) - y1(i))) / (m2 * sqrt((x2(i) - x1(i))^2 + (y2(i) - y1(i))^2)) + damping_force_y / m2 + g;
    
    % Update velocities and positions using Euler method
    vx1(i+1) = vx1(i) + ax1 * dt;
    vy1(i+1) = vy1(i) + ay1 * dt;
    
    x1(i+1) = x1(i) + vx1(i+1) * dt;
    y1(i+1) = y1(i) + vy1(i+1) * dt;
    
    vx2(i+1) = vx2(i) + ax2 * dt;
    vy2(i+1) = vy2(i) + ay2 * dt;
    
    x2(i+1) = x2(i) + vx2(i+1) * dt;
    y2(i+1) = y2(i) + vy2(i+1) * dt;
end

% Plot the results
time = (0:num_steps-1) * dt;
figure;
subplot(2,2,1);
plot(time, x1, 'b', time, x2, 'r');
xlabel('Time');
ylabel('x Displacement');
legend('m1', 'm2');
title('x Displacement vs. Time');

subplot(2,2,2);
plot(time, y1, 'b', time, y2, 'r');
xlabel('Time');
ylabel('y Displacement');
legend('m1', 'm2');
title('y Displacement vs. Time');

subplot(2,2,3);
plot(time, vx1, 'b', time, vx2, 'r');
xlabel('Time');
ylabel('x Velocity');
legend('m1', 'm2');
title('x Velocity vs. Time');

subplot(2,2,4);
plot(time, vy1, 'b', time, vy2, 'r');
xlabel('Time');
ylabel('y Velocity');
legend('m1', 'm2');
title('y Velocity vs. Time');
