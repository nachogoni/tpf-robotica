function ret = distancesToPixels(fovImgInfo,CamH,fovH,CamAngle)

h = size(fovImgInfo)(1)
w = size(fovImgInfo)(2)

fovV = fovH * h / w;
gamma = pi()/2 - CamAngle - fovV/2;

CamHcuad = CamH^2;

for i=1:h
	for j=1:w
		elem = fovImgInfo{i,j};
		dcuad = ( CamH * tan(gamma + elem(2)) ) ^ 2;
		dpluscamHcuad = CamHcuad + dcuad;
		sencuad = sin(elem(1)/2) ^ 2;

		ret(i,j) = sqrt( 4 * dpluscamHcuad * sencuad + dcuad);
	endfor
endfor
endfunction

