close all;
clear all;
clc;
home;


path(path, '..\utility');


robot = create_3axis_robot();


Ts = 0.1;
Tsim = 10;
axis_range = [-1.0 1.0 -1.0 1.0 -1.5 0.5];
[skeleton, coordinate_arrow] = create_skeleton(robot, axis_range);	% �׸� ����

iteration_flag = 1;

while iteration_flag
	
	for time = 0:Ts:Tsim
		tstart = tic;
		
		
		% ���ر��� �Է�
		% .....
		%----------------
		
		% ���� �Է� ����
		% .....
		%---------------
		
		% ��ġ ����
		% ....
		robot = set_joint_angles(robot, [30/180*pi*sin(2*pi/10*time);
										         40/180*pi*sin(2*pi/10*time);
                                       50/180*pi*sin(2*pi/10*time)]);
		%-------------------------

		% ������� ���
		robot = compute_forward_kinematics(robot);
		%-------------------------
		
		% �׸� ������Ʈ
% 		tstart = tic;
		update_skeleton(skeleton, coordinate_arrow, robot);
		%-------------------------
		
		% Ű �Է� ó��
		key = get(gcf, 'currentcharacter');	% Ű �Է�
		switch key
			case 'x'	% �ùķ��̼� ����
				iteration_flag = 0;
				display('simulation is terminated.');
				break;				
			case 'p'	% �Ͻ�����
				display('pause');
				while 1
					key = get(gcf, 'currentcharacter');
					if key == 'r'	% �ٽ� ����
						display('run');
						break;
					elseif key == 'x'	% �Ͻ����� �� ����
						iteration_flag = 0;
						display('simulation is terminated.');
						break;
					end
					pause(1);
				end					
			otherwise
		end
		%-------------------------
		
		telapsed = toc(tstart);
% 		display(telapsed);
		% ���� �ð� ��� �� �Ͻ� ����
		if telapsed < Ts
			pause(Ts-telapsed);
		end
	
	end

end

close all;
display('simulation is finished.');