function ret = genImg(pxwidth, pxheight)

ret = {};

for i=1:(pxwidth)
	for j=1:(pxheight)
		ret{j,i} = [(i - pxwidth/2) (pxheight - j)];
	endfor
endfor
endfunction

