
load odometryErrorOct.txt;
newvars = vars';
odomXs = newvars(1,:);
gpsXs = newvars(2,:);
odomZs = newvars(3,:);
gpsZs = newvars(4,:);
odomA = newvars(5,:);
gpsA = newvars(6,:);

errXs = abs(odomXs - gpsXs);
errZs = abs(odomZs - gpsZs);

gpsNA = toStdRadians(gpsA);
errAs = abs(odomA - gpsNA);

plot(errXs);
print('errorX.eps', '-deps');

plot(errZs);
print('errorZ.eps', '-deps');

plot(errAs);
print('errorA.eps', '-deps');

