# Makefile.maxlib for Makefile.pdlibbuilder. 
# 

lib.name = maxlib

uname := $(shell uname -s)
ifeq (MINGW,$(findstring MINGW,$(uname)))
  ldlibs = -lwsock32 -lpthread
endif

allow.class.sources = allow.c  
arbran.class.sources = arbran.c  
arraycopy.class.sources = arraycopy.c  
average.class.sources = average.c  
beat.class.sources = beat.c  
beta.class.sources = beta.c  
bilex.class.sources = bilex.c  
borax.class.sources = borax.c  
cauchy.class.sources = cauchy.c  
chord.class.sources = chord.c  
delta.class.sources = delta.c  
deny.class.sources = deny.c  
dist.class.sources = dist.c  
divide.class.sources = divide.c  
divmod.class.sources = divmod.c  
edge.class.sources = edge.c  
expo.class.sources = expo.c  
fifo.class.sources = fifo.c  
gauss.class.sources = gauss.c  
gestalt.class.sources = gestalt.c  
history.class.sources = history.c  
ignore.class.sources = ignore.c  
iso.class.sources = iso.c  
lifo.class.sources = lifo.c  
limit.class.sources = limit.c  
linear.class.sources = linear.c  
listfifo.class.sources = listfifo.c  
listfunnel.class.sources = listfunnel.c  
match.class.sources = match.c  
#maxlib.class.sources = maxlib.c  
minus.class.sources = minus.c  
mlife.class.sources = mlife.c  
multi.class.sources = multi.c  
nchange.class.sources = nchange.c  
netclient.class.sources = netclient.c  
netdist.class.sources = netdist.c  
netrec.class.sources = netrec.c  
netserver.class.sources = netserver.c  
nroute.class.sources = nroute.c  
pitch.class.sources = pitch.c  
plus.class.sources = plus.c  
poisson.class.sources = poisson.c  
pong.class.sources = pong.c  
pulse.class.sources = pulse.c  
remote.class.sources = remote.c  
rewrap.class.sources = rewrap.c  
rhythm.class.sources = rhythm.c  
scale.class.sources = scale.c  
score.class.sources = score.c  
speedlim.class.sources = speedlim.c  
split.class.sources = split.c  
step.class.sources = step.c  
subst.class.sources = subst.c  
sync.class.sources = sync.c  
temperature.class.sources = temperature.c  
tilt.class.sources = tilt.c  
timebang.class.sources = timebang.c  
triang.class.sources = triang.c  
unroute.class.sources = unroute.c  
urn.class.sources = urn.c  
velocity.class.sources = velocity.c  
weibull.class.sources = weibull.c  
wrap.class.sources = wrap.c

extradirs = examples manual
datafiles = \
$(wildcard *-help.pd) \
LICENSE.txt \
README.txt \
maxlib-meta.pd

externalsdir = ..
# include Makefile.pdlibbuilder from parent or current directory 
-include $(externalsdir)/Makefile.pdlibbuilder 

ifndef Makefile.pdlibbuilder 
include Makefile.pdlibbuilder 
endif
