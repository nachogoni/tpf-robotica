
plot(wander)
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title 'Progreso de "Wandering"'
print('graphics/Wander.png','-dpng');

plot(focus)
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title 'Progreso de "Enfocar basura"'
print('graphics/Focus.png','-dpng');

plot(goto)
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title 'Progreso de "Ir a basura"'
print('graphics/GoTo.png','-dpng');

plot(collect)
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title 'Progreso de "Recolectar basura"'
print('graphics/Collect.png','-dpng');

plot(unload)
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title 'Progreso de "Descargar basura"'
print('graphics/Unload.png','-dpng');

plot(recharge)
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title 'Progreso de "Recargar bateria"'
print('graphics/Recharge.png','-dpng');

plot(avoid)
xlabel "Cantidad de steps de simulacion";
ylabel "Cantidad de steps activo";
title 'Progreso de "Evitar Obstaculos"'
print('graphics/Avoid.png','-dpng');
