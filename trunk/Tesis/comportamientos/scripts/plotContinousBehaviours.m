
bar(contActnw)
xlabel "Steps";
ylabel "# de steps continuamente activo";
title 'Progreso de "Wander"'
print('graphics/WanderContAct.png','-dpng');

bar(contActnf)
xlabel "Steps";
ylabel "# de steps continuamente activo";
title 'Progreso de "Enfocar basura"'
print('graphics/FocusContAct.png','-dpng');

bar(contActng)
xlabel "Steps";
ylabel "# de steps continuamente activo";
title 'Progreso de "Ir a basura"'
print('graphics/GoToContAct.png','-dpng');

bar(contActnc)
xlabel "Steps";
ylabel "# de steps continuamente activo";
title 'Progreso de "Recolectar basura"'
print('graphics/CollectContAct.png','-dpng');

bar(contActnu)
xlabel "Steps";
ylabel "# de steps continuamente activo";
title 'Progreso de "Descargar basura"'
print('graphics/UnloadContAct.png','-dpng');

bar(contActnr)
xlabel "Steps";
ylabel "# de steps continuamente activo";
title 'Progreso de "Recargar bateria"'
print('graphics/RechargeContAct.png','-dpng');

bar(contActna)
xlabel "Steps";
ylabel "# de steps continuamente activo";
title 'Progreso de "Evitar Obstaculos"'
print('graphics/AvoidContAct.png','-dpng');
