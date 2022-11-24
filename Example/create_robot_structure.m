function R = create_robot_structure(n)
	
	if n < 1 
		error('wrong number of joints');
	elseif n > 10
		error('too many joints (1 <= n <= 10');
	end
	
	% ? ?
	R.name = 'untitled';
	
	%  
	R.n = n;
	
	R.T_gto0 = eye(4,4);
	
	for k = 1:n
				
		% DH ?		
		R.link(k).q_offset = 0;
		R.link(k).d = 0;
		R.link(k).l = 0;
		R.link(k).a = 0;
		
		
		% ? ?
		R.link(k).T_km1tok = eye(4,4);	% T^km1_k:  ??  ? ?
		R.link(k).T_gtok = eye(4,4);		% T^g_k:  ??  ? ?
		
		
				
		% 
		R.link(k).m = 0;
		R.link(k).Icm = zeros(3,3);
		
		%  ?
		R.link(k).q = 0;
		R.link(k).dq = 0;
		R.link(k).ddq = 0;
		
		% ? ? ((k) ? )
		R.link(k).s = zeros(3,1);
		
		% ? ? (base ?   ? ?)
		R.link(k).p = zeros(3,1);
		R.link(k).dp = zeros(3,1);
		R.link(k).ddp = zeros(3,1);
	
		R.link(k).color = zeros(3,1);
		
	end
	
	% ?
	R.J = zeros(6,n);
		