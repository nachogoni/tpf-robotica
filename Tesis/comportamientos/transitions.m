function ret = transitions(stats)

i=2;
j=1;

n = size(stats)(1) + 1;
l = size(stats)(2);

ret = zeros(l+2,l+2);

lastIdx = find(stats(2,:));
while( i < n )

	diff = stats(i,:) - stats(i-1,:);
	newIdx = find(diff);

	if ( newIdx != lastIdx )
		ret(lastIdx,newIdx)++;
	endif

	lastIdx = newIdx;
	i++;
endwhile

k = 1
while( k < l )
	[aux idx] = max(ret(k,:));
	ret(k,l+1) = aux;
	ret(k,l+2) = idx;
%	[aux idx] = min(ret(k,:));
%	ret(k,l+3) = aux;
%	ret(k,l+4) = idx;
	[aux idx] = max(ret(:,k));
	ret(l+1,k) = aux;
	ret(l+2,k) = idx;

%	[aux idx] = min(ret(:,k));
%	ret(l+3,k) = aux;
%	ret(l+4,k) = idx;
	k++;
endwhile

endfunction
