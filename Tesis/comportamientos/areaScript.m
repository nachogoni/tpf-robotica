load areaStats.txt

nvars = vars';

function ret = cutSeen(aseen,maxdel)

for i=1:(length(aseen))
	aux = aseen(i);
	if aux > maxdel
		ret(i) = maxdel;
	else
		ret(i) = aux;
	endif
endfor

endfunction

function ret = cutLeft(aseen,mindel)

for i=1:(length(aseen))
	aux = aseen(i);
	if aux < mindel
		ret(i) = mindel;
	else
		ret(i) = aux;
	endif
endfor

endfunction

function ret = makeSamples(arr,qty)

i=1;
k=1;
n = length(arr);
while (i < n)
	ret(k) = arr(i);
	k++;
	i+=qty;
endwhile
ret(k) = arr(n);
endfunction

function ret = diffSamples(arr)

i=1;
n = length(arr);
while (i < n)
	ret(i) = arr(i+1) - arr(i);
	i++;
endwhile
endfunction



left = nvars(1,:);
Rleft = nvars(2,:);
seen = nvars(3,:);

cleft = cutLeft(left,0);
cseen = cutSeen(seen,max(left));

plot(left,"1");
hold on;
plot(Rleft,"2");
plot(seen,"3");
plot(cseen,"4");
plot(cleft,"5");
hold off;

plot(cleft,"1");
hold on;
plot(cseen,"2");
hold off;

plot(cleft(1:100000),"1;Slots restantes;");
hold on;
plot(cseen(1:100000),"2;Slots vistos;");
title("Slots vistos Vs. Slots restantes");
xlabel("Steps");
ylabel("Cantidad de Slots");
hold off;
print('graphics/areaCoveredVsTime.png','-dpng');

samples = makeSamples(cseen(1:100000),10000);

bar(samples)
samples./(max(samples))

diffsamples = diffSamples(samples./(max(samples)))
bar(diffsamples)

%diffsamples./(max(diffsamples))

max(left)
