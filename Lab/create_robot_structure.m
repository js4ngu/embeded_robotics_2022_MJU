function R = create_robot_structure(n)	
	if n < 1 
		error('wrong number of joints');
	elseif n > 10
		error('too many joints (1 <= n <= 10');
	end
	
	% 로봇 이름
	R.name = 'untitled';
	
	% 관절 개수
	R.n = n;
	
	R.T_gto0 = eye(4,4);
	
	for k = 1:n
		% DH 파라미터
		R.link(k).q_offset = 0;
		R.link(k).d = 0;
		R.link(k).l = 0;
		R.link(k).a = 0;
		
		% 좌표 변환
		R.link(k).T_km1tok = eye(4,4);		% T^km1_k: 이전 좌표계에서 현재 좌표계로의 변환
		R.link(k).T_gtok = eye(4,4);		% T^g_k: 전역 좌표계에서 현재 좌표계로의 변환	
				
		% 질량
		R.link(k).m = 0;
		R.link(k).Icm = zeros(3,3);
		
		% 관절 좌표
		R.link(k).q = 0;
		R.link(k).dq = 0;
		R.link(k).ddq = 0;
		
		% 무게중심 좌표 (현재(k) 좌표계 기준)
		R.link(k).s = zeros(3,1);
		
		% 링크 좌표 (base 좌표계 기준 무게 중심의 위치)
		R.link(k).p = zeros(3,1);
		R.link(k).dp = zeros(3,1);
		R.link(k).ddp = zeros(3,1);
	
		R.link(k).color = zeros(3,1);
	end
	
	% 자코비안
	R.J = zeros(6,n);