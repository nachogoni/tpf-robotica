function ret = toStdRadians(angles)

for i=1:(length(angles))
	aux = angles(i);
	if aux > pi()*2
		aux = aux - 2*pi();
	endif
	if aux < 0
		aux = aux + 2*pi();
	endif
	ret(i) = aux;
endfor

