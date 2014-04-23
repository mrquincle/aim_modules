% read from file
tz = load('../data/gaussian1.data');

% display first 5 entries
%tz0_x=tz(1:5,:)

tz0_ind=(tz(:,3)==1);
tz0_xy=tz(tz0_ind,:);
tz0_x=tz0_xy(:,1);
tz0_y=tz0_xy(:,2);

tz1_ind=(tz(:,3)==2);
tz1_xy=tz(tz1_ind,:);
tz1_x=tz1_xy(:,1);
tz1_y=tz1_xy(:,2);

tz2_ind=(tz(:,3)==3);
tz2_xy=tz(tz2_ind,:);
tz2_x=tz2_xy(:,1);
tz2_y=tz2_xy(:,2);



% plot, if you want to, uncomment
plot(tz0_x, tz0_y, 'x');
hold on
plot(tz1_x, tz1_y, 'o');
hold on
plot(tz2_x, tz2_y, '.');
hold off

