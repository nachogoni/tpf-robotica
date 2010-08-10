function ret = getFovKs(imgInfo,fovH)

ret = {};
h = size(imgInfo)(1)
w = size(imgInfo)(2)

fovV = fovH * h / w;

for i=1:h
	for j=1:w
		elem = imgInfo{i,j};
		kp = abs(elem(1)/(w/2));
		k = elem(2)/h;
		ret{i,j} = [ (kp*fovH/2) (k*fovV) ];
	endfor
endfor
endfunction

