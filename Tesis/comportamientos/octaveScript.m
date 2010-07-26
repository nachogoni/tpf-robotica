
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


plotPie

% Plot Behaviour's progress in each timestep
plotBehaviours

% Plot All behaviours progress
allPlot

% Plot Behaviours's continous active time steps

plotContinousBehaviours



% Plot Behaviours's continous inactive time steps

bar(icontActnw)
print('graphics/WanderContInact.png','-dpng');

bar(icontActnf)
print('graphics/FocusContInact.png','-dpng');

bar(icontActng)
print('graphics/GoToContInact.png','-dpng');

bar(icontActnc)
print('graphics/CollectContInact.png','-dpng');

bar(icontActnu)
print('graphics/UnloadContInact.png','-dpng');

bar(icontActnr)
print('graphics/RechargeContInact.png','-dpng');

bar(icontActna)
print('graphics/AvoidContInact.png','-dpng');

meanV
imeanV

desvStd
idesvStd

max(meanV)
max(imeanV)
max(desvStd)
max(idesvStd)

mw = max(contActnw)
mf = max(contActnf)
mg = max(contActng)
mr = max(contActnr)
mc = max(contActnc)
ma = max(contActna)
mu = max(contActnu)

cantActnw = length(contActnw)
cantActnf = length(contActnf)
cantActng = length(contActng)
cantActnr = length(contActnr)
cantActnc = length(contActnc)
cantActna = length(contActna)
cantActnu = length(contActnu)
%mp = max(contActnp)

max([mw mf mg mr mc ma mu])

contActnw;
contActnf;
contActng;
contActnc;
contActnu;
contActnr;
contActna;

%contActnp;
%bar(contActnp)

scontActnw;
scontActnf;
scontActng;
scontActnc;
scontActnu;
scontActnr;
scontActna;

smeanV
sdesvStd

