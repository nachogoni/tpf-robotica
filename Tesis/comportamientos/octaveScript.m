
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

plotInContinousBehaviours

printStats

