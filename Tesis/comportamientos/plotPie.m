
names = {"Wandering","Enfocar basura", "Ir a basura","Recolectar basura", "Descargar basura","Recargar bateria", "Evitar obstaculos", "Salir situaciones no deseadas"};

i=1;

while( i < 9 )
	labels{i} = sprintf("%s (%.2g)", names{i}, percentages(i));
	i++;
endwhile

pie(vars(length(vars),:),[0,0,0,0,1,0,0,0],labels)
axis([-3.4 3.4]);
print('graphics/BehaviourDistributionPie.png','-dpng');
