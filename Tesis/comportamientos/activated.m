function ret = activated(arr,value)

ret(1) = 0;
for i=2:(length(arr))
	if arr(i) > arr(i-1)
		aux = value;
	else
		aux = 0;
	endif
	ret(i) = aux;
endfor

endfunction
