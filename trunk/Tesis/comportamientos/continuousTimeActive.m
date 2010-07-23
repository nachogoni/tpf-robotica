function ret = continuousTimeActive(arr)

i=1;
j=1;

n = length(arr) + 1;

while( i < n )

	while( i < n && arr(i) == 0 )
		i++;
	endwhile

	% Counter for active steps
	k=0;
	while( i < n && arr(i) != 0 )
		k++;
		i++;
	endwhile
	if( k != 0 )
		ret(j) = k;
		j++;
	endif
endwhile

endfunction
