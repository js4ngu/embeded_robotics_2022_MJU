function [skeleton, coordinate_arrow] = create_skeleton(R, axis_range)
	
	n = R.n;
	if n < 0
		error('wrong drawing parameters');
	end
	
	skeleton = zeros(n,4);
	coordinate_arrow = zeros(n, 3, 4);
	
	
	view_angle = [90 90;			% top view
		           90+37.5 30;	% perspective view
					  90, 0;			% frontal view
					  180, 0];		% side view
	
	title_sentence = {'top view';
							'perspective view';
							'frontal view';
							'side view'};
		
	figure;
	hold on;
	for index = 1:4
		
		subplot(2,2,index);
		
		for k = 1:n
			skeleton(k,index) = line('LineWidth', 5.0, 'Color', R.link(k).color);
			coordinate_arrow(k, :, index) = create_coordinate_arrow();
		end
				
		axis equal;		
		view(view_angle(index,:));
		axis(axis_range);	% [xi xf yi yf zi zf] 
		grid on;
		xlabel('x axis');
		ylabel('y axis');
		zlabel('z axis');
		title(title_sentence(index));
	end
	