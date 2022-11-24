function update_coordinate_arrow(h, T, scale)
	
	if scale < 0 || scale > 1
		scale = 1;
	end

	
	weight1 = 0.3;
	weight2 = 0.3;

	% 좌표계의 원점 및 단위벡터 추출
	p = T(1:3,4);
	a = T(1:3,1:3)*scale;

	% 화살표를 만들기 위한 보조 벡터
	a_xy = a(:,1) - a(:,2);
	a_yz = a(:,2) - a(:,3);
	a_zx = a(:,3) - a(:,1);
	
	% 각 단위벡터 별로 2개의 직선 생성
	ax_1 = a(:,1) + (-a(:,1) + a_yz*weight1)*weight2;
	ax_2 = a(:,1) + (-a(:,1) - a_yz*weight1)*weight2;
	
	ay_1 = a(:,2) + (-a(:,2) + a_zx*weight1)*weight2;
	ay_2 = a(:,2) + (-a(:,2) - a_zx*weight1)*weight2;
	
	az_1 = a(:,3) + (-a(:,3) + a_xy*weight1)*weight2;
	az_2 = a(:,3) + (-a(:,3) - a_xy*weight1)*weight2;
	
	% 각 단위벡터 별 화살표 생성
	xx = [zeros(3,1) a(:,1) a(:,1) ax_1 a(:,1) ax_2];
	yy = [zeros(3,1) a(:,2) a(:,2) ay_1 a(:,2) ay_2];
	zz = [zeros(3,1) a(:,3) a(:,3) az_1 a(:,3) az_2];
	
	% 업데이트
	set(h(1), 'Xdata', p(1)+xx(1,:), 'Ydata', p(2)+xx(2,:), 'Zdata', p(3)+xx(3,:));
	set(h(2), 'Xdata', p(1)+yy(1,:), 'Ydata', p(2)+yy(2,:), 'Zdata', p(3)+yy(3,:));
	set(h(3), 'Xdata', p(1)+zz(1,:), 'Ydata', p(2)+zz(2,:), 'Zdata', p(3)+zz(3,:));
	
