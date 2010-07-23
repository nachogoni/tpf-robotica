
%plot(nw,'1;Wander;');
%hold on;
%plot(nf,'2;Focus;');
%plot(ng,'3;Go To;');
%plot(nc,'4;Collect;');
plot(nu,'5;Unload;');
%plot(nr,'0;Recharge;');
%plot(na,'1;Avoid;');
%plot(np,'2;Panic;');
%hold off;

pie(vars(length(vars),:),[0,0,0,0,1,0,0,0],["Wander","Focus Garbage","Go To Garbage","Collect Garbage","Unload Garbage","Recharge","Avoid Obstacles","Panic"])

bar(contActnw)
print('graphics/WanderContAct.png','-dpng');

bar(contActnf)
print('graphics/FocusContAct.png','-dpng');

bar(contActng)
print('graphics/GoToContAct.png','-dpng');

bar(contActnc)
print('graphics/CollectContAct.png','-dpng');

bar(contActnu)
print('graphics/UnloadContAct.png','-dpng');

bar(contActnr)
print('graphics/RechargeContAct.png','-dpng');

bar(contActna)
print('graphics/AvoidContAct.png','-dpng');

meanV

desvStd

max(meanV)
max(desvStd)

mw = max(contActnw);
mf = max(contActnf);
mg = max(contActng);
mr = max(contActnr);
mc = max(contActnc);
ma = max(contActna);
mu = max(contActnu);
mp = max(contActnp);

max([mw mf mg mr mc ma mu mp])

contActnw;
contActnf;
contActng;
contActnc;
contActnu;
contActnr;
contActna;
contActnp;
%bar(contActnp)


