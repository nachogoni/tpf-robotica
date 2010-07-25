
plot(unload,'1*;Descargar Basura;')
hold on;
plot(wander,'3*;Wandering;');
plot(recharge,'2*;Recargar Bateria;')
plot(avoid,'5;Evitar obstaculos;')
plot(goto,'0;Ir a Basura;');
plot(collect,'1;Recolectar;')
plot(focus,'2;Enfocar;');
print('graphics/All.png','-dpng');
xlabel "Steps";
ylabel "# de steps activo";
hold off;
