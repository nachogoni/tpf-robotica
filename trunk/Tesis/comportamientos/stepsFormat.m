function ret = stepsFormat(timesteps)

totalmillis = timesteps * 32

milliseconds = mod(totalmillis,1000)

totalseconds = totalmillis/1000;
seconds = mod(totalseconds,60)

totalminutes = totalseconds/60;
minutes = mod(totalminutes,60)

totalhours = totalminutes / 60;
hours = mod(totalhours,24)

totaldays = totalhours / 24
