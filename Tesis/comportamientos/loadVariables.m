
load simStatsOct.txt;
newvars = vars';
wander = newvars(1,:);
focus = newvars(2,:);
goto = newvars(3,:);
collect = newvars(4,:);
unload = newvars(5,:);
recharge = newvars(6,:);
avoid = newvars(7,:);
panic = newvars(8,:);


%hold off;
%plot(odomA,'1;Odom A;');
%hold on;
%plot(gpsNA,'2;GPS A;');
%print('odomVSgpsA.eps', '-deps', '-color');

nw = activated(wander,1);
nf = activated(focus,2);
ng = activated(goto,3);
nc = activated(collect,4);
nu = activated(unload,5);
nr = activated(recharge,6);
na = activated(avoid,7);
np = activated(panic,8);

contActnw = continuousTimeActive(nw);
contActnf = continuousTimeActive(nf);
contActng = continuousTimeActive(ng);
contActnc = continuousTimeActive(nc);
contActnu = continuousTimeActive(nu);
contActnr = continuousTimeActive(nr);
contActna = continuousTimeActive(na);
contActnp = continuousTimeActive(np);


desvStd = zeros(1,8);
desvStd(1) = std(contActnw);
desvStd(2) = std(contActnf);
desvStd(3) = std(contActng);
desvStd(4) = std(contActnc);
desvStd(5) = std(contActnu);
desvStd(6) = std(contActnr);
desvStd(7) = std(contActna);
desvStd(8) = std(contActnp);

meanV = zeros(1,8);
meanV(1) = mean(contActnw);
meanV(2) = mean(contActnf);
meanV(3) = mean(contActng);
meanV(4) = mean(contActnc);
meanV(5) = mean(contActnu);
meanV(6) = mean(contActnr);
meanV(7) = mean(contActna);
meanV(8) = mean(contActnp);

