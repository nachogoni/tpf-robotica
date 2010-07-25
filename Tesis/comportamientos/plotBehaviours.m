
plot(wander)
xlabel "Steps";
ylabel "# de steps activo";
title 'Progreso de "Wandering"'
print('graphics/Wander.png','-dpng');

plot(focus)
xlabel "Steps";
ylabel "# de steps activo";
title 'Progreso de "Enfocar basura"'
print('graphics/Focus.png','-dpng');

plot(goto)
xlabel "Steps";
ylabel "# de steps activo";
title 'Progreso de "Ir a basura"'
print('graphics/GoTo.png','-dpng');

plot(collect)
xlabel "Steps";
ylabel "# de steps activo";
title 'Progreso de "Recolectar basura"'
print('graphics/Collect.png','-dpng');

plot(unload)
xlabel "Steps";
ylabel "# de steps activo";
title 'Progreso de "Descargar basura"'
print('graphics/Unload.png','-dpng');

plot(recharge)
xlabel "Steps";
ylabel "# de steps activo";
title 'Progreso de "Recargar bateria"'
print('graphics/Recharge.png','-dpng');

plot(avoid)
xlabel "Steps";
ylabel "# de steps activo";
title 'Progreso de "Evitar Obstaculos"'
print('graphics/Avoid.png','-dpng');
