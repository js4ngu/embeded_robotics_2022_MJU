close all;
clear all;
home;
clc;

figure;

line_hd = [line('LineWidth', 3.0, 'Color', 'r'); 
           line('LineWidth', 3.0, 'Color', 'g');
           line('LineWidth', 3.0, 'Color', 'b')];

axis equal;
axis([-30 30 -30 30 -30 30]);
grid on;

for i=1: 10000
	
	
	e1 = 10*sin(2*pi/100*i);
	e2 = 20*sin(2*pi/100*i);
	e3 = 30*sin(2*pi/100*i);
	
	set(line_hd(1), 'Xdata', [0;0], 'Ydata', [0;0], 'Zdata', [0;e1]);
	set(line_hd(2), 'Xdata', [0;0], 'Ydata', [0;e1], 'Zdata', [0;e2]);
	set(line_hd(3), 'Xdata', [0;0], 'Ydata', [0;e2], 'Zdata', [0;e3]);
	
	drawnow;
	pause(0.1);
	
end



