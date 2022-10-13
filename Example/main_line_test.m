close all;
clear all;
home;

x_data = [0;1];
y_data = [0;2];
z_data = [0;3];

figure;
line(x_data, y_data, z_data);

figure;
line(x_data, y_data, z_data, 'LineWidth', 3, 'Color', 'r');

figure;
line(x_data, y_data, z_data, 'LineWidth', 3, 'Color', [205;50;218]/255);
grid on;

figure;
handle = line('LineWidth', 3, 'Color', [0.7;0.9;0.5]);
set(handle, 'Xdata', x_data, 'Ydata', y_data, 'Zdata', z_data);
grid on;

figure;
handle = quiver3(0,0,0,0,0,0,'LineWidth', 3, 'Color', [0.7;0.9;0.5]);
set(handle, 'Xdata', 1, 'Ydata', 2, 'Zdata', 3, 'Udata', 0.1, 'Vdata', 0.2, 'Wdata', 0.3);	






