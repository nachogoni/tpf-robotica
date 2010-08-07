
clear labels
bnames = {"Wandering","Enfocar basura", "Ir a basura","Recolectar basura", "Descargar basura","Recargar bateria", "Evitar obstaculos", "Salir situaciones no deseadas"};

i=1;
while( i <= length(bnames) )
	labels{i} = sprintf("%s (%.2g)", bnames{i}, percentages(i));
	i++;
endwhile

h = pie(vars(length(vars),:),[0,0,0,0,1,0,0,0],labels)
%set(h(4),'color',aux2);
set(h(3),'facecolor',[1 1 0]);
%set(h(12),'color',[0.5 0.5 0.5]');
axis([-3.4 3.4]);
replot()
print('graphics/BehaviourDistributionPie.png','-dpng');
