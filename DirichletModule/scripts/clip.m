y=dlmread("../builds/standard/output.octave");

p=dlmread("../data/clusters.txt");

% Draw ellipse using mean and covariance
function draw_ellipse(mu, covariance, type)
	theta=linspace(0, 2*pi, 1000);
	A=chol(covariance, "lower");	
	for i = 1:length(theta)
	  x(:,i) = mu' + 2.5 * A * [cos(theta(i)); sin(theta(i))];
	endfor
	%hold on
	clr=hsv2rgb([type,1,1]);
	plot(x(1,:), x(2,:), "color", clr, "LineWidth", 2)
	axis("square", "equal");
end

ind=y(:,1);
mx=max(ind);
mn=min(ind)
skip=1;
for t=mn:skip:mx,
	t
	% get all clusters at timestep t
	pnts=y(find(ind==t),:);
	% get x-coordinates of all clusters
	x0=pnts(:,2);
	% get y-coordinates of all clusters
	%x1=pnts(:,3);
	%plot(x0, x1, "go","LineWidth", 4);
	cluster_cnt=length(x0);

	p0=p(:,1);
	p1=p(:,2);
	plot(p0,p1,"r.");
	
	hold on;


	for i = 1:cluster_cnt
		type=i/cluster_cnt;
		mu=[pnts(i,2) pnts(i,3)];
		R=[pnts(i,4) pnts(i,5); pnts(i,6) pnts(i,7)];
		draw_ellipse(mu, R, type);
		clr=hsv2rgb([type,1,1]);
		plot(pnts(i,2), pnts(i,3), 'Marker', 'o', 'color', clr, 'LineWidth', 4);
	end

	hold off;
	sleep(0.1)
end
