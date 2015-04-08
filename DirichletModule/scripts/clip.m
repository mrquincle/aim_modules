y=dlmread("../builds/standard/output.octave");

p=dlmread("../data/clusters.txt");

ind=y(:,1);
mx=max(ind);
mn=min(ind);
for t=mn:mx,

	pnts=y(find(ind==t),:);
	x0=pnts(:,2);
	x1=pnts(:,3);
	plot(x0,x1,"go", "LineWidth", 4);

	hold on;

	p0=p(:,1);
	p1=p(:,2);
	plot(p0,p1,"r.");

	hold off;
	sleep(0.1)
end
