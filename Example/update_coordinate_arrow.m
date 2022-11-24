function update_coordinate_arrow(h, T, scale)
	
	if scale < 0 || scale > 1
		scale = 1;
	end

	
	weight1 = 0.3;
	weight2 = 0.3;

	% ��ǥ���� ���� �� �������� ����
	p = T(1:3,4);
	a = T(1:3,1:3)*scale;

	% ȭ��ǥ�� ����� ���� ���� ����
	a_xy = a(:,1) - a(:,2);
	a_yz = a(:,2) - a(:,3);
	a_zx = a(:,3) - a(:,1);
	
	% �� �������� ���� 2���� ���� ����
	ax_1 = a(:,1) + (-a(:,1) + a_yz*weight1)*weight2;
	ax_2 = a(:,1) + (-a(:,1) - a_yz*weight1)*weight2;
	
	ay_1 = a(:,2) + (-a(:,2) + a_zx*weight1)*weight2;
	ay_2 = a(:,2) + (-a(:,2) - a_zx*weight1)*weight2;
	
	az_1 = a(:,3) + (-a(:,3) + a_xy*weight1)*weight2;
	az_2 = a(:,3) + (-a(:,3) - a_xy*weight1)*weight2;
	
	% �� �������� �� ȭ��ǥ ����
	xx = [zeros(3,1) a(:,1) a(:,1) ax_1 a(:,1) ax_2];
	yy = [zeros(3,1) a(:,2) a(:,2) ay_1 a(:,2) ay_2];
	zz = [zeros(3,1) a(:,3) a(:,3) az_1 a(:,3) az_2];
	
	% ������Ʈ
	set(h(1), 'Xdata', p(1)+xx(1,:), 'Ydata', p(2)+xx(2,:), 'Zdata', p(3)+xx(3,:));
	set(h(2), 'Xdata', p(1)+yy(1,:), 'Ydata', p(2)+yy(2,:), 'Zdata', p(3)+yy(3,:));
	set(h(3), 'Xdata', p(1)+zz(1,:), 'Ydata', p(2)+zz(2,:), 'Zdata', p(3)+zz(3,:));
	
