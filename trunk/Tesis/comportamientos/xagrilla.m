
imgwidth = 320;
imgheight = 640;

fovh = 1.1;
cameraHeight = 0.038;
cameraAngle = 0.5;

myimg = genImg(imgwidth,imgheight);
lala = getFovKs(myimg,fovh);
solucion = distancesToPixels(lala,cameraHeight,fovh,cameraAngle);

maximum = max(max(solucion));
minimum = min(min(solucion));

save grilladedistancias maximum minimum solucion

