function ret = millisToSeconds(timesteps)

totalmillis = timesteps * 32;

milliseconds = mod(totalmillis,1000)

seconds = mod((totalmillis/1000),60)

minutes = mod((totalmillis/1000)/60,60)

