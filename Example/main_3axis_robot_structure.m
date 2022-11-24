close all;
clear all;
clc;
home;


path(path, '..\utility');


robot = create_3axis_robot();


Ts = 0.1;
Tsim = 10;
axis_range = [-1.0 1.0 -1.0 1.0 -1.5 0.5];
[skeleton, coordinate_arrow] = create_skeleton(robot, axis_range);	% 그림 생성

iteration_flag = 1;

while iteration_flag
	
	for time = 0:Ts:Tsim
		tstart = tic;
		
		
		% 기준궤적 입력
		% .....
		%----------------
		
		% 제어 입력 생성
		% .....
		%---------------
		
		% 수치 적분
		% ....
		robot = set_joint_angles(robot, [30/180*pi*sin(2*pi/10*time);
										         40/180*pi*sin(2*pi/10*time);
                                       50/180*pi*sin(2*pi/10*time)]);
		%-------------------------

		% 운동기하학 계산
		robot = compute_forward_kinematics(robot);
		%-------------------------
		
		% 그림 업데이트
% 		tstart = tic;
		update_skeleton(skeleton, coordinate_arrow, robot);
		%-------------------------
		
		% 키 입력 처리
		key = get(gcf, 'currentcharacter');	% 키 입력
		switch key
			case 'x'	% 시뮬레이션 종료
				iteration_flag = 0;
				display('simulation is terminated.');
				break;				
			case 'p'	% 일시정지
				display('pause');
				while 1
					key = get(gcf, 'currentcharacter');
					if key == 'r'	% 다시 실행
						display('run');
						break;
					elseif key == 'x'	% 일시정지 중 종료
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
		% 지연 시간 계산 후 일시 정지
		if telapsed < Ts
			pause(Ts-telapsed);
		end
	
	end

end

close all;
display('simulation is finished.');