
hold off;
legend('','location','northwest');
hold on;
plot(unload,'1;Descargar Basura;',"linewidth",1,"linestyle","--")
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title "Progreso de todos los comportamientos";
plot(wander,'3;Wandering;',"linewidth",2);
plot(recharge,'2;Recargar Bateria;',"linewidth",3);
plot(avoid,'5;Evitar obstaculos;',"linewidth",4);
plot(goto,';Ir a Basura;',"linewidth",5);
plot(collect,'1;Recolectar;',"linewidth",6);
plot(focus,'2;Enfocar;',"linewidth",7,"linestyle","-.");
print('graphics/All.png','-dpng');
hold off;
