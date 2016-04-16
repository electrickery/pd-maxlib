# Makefile.maxlib for Makefile.pdlibbuilder. 
# 

lib.name = maxlib

uname := $(shell uname -s)
ifeq (MINGW,$(findstring MINGW,$(uname)))
  ldlibs = -lwsock32 -lpthread
endif

allow.class.sources =       src/allow.c  
arbran.class.sources =      src/arbran.c  
arraycopy.class.sources =   src/arraycopy.c  
average.class.sources =     src/average.c  
beat.class.sources =        src/beat.c  
beta.class.sources =        src/beta.c  
bilex.class.sources =       src/bilex.c  
borax.class.sources =       src/borax.c  
cauchy.class.sources =      src/cauchy.c  
chord.class.sources =       src/chord.c  
delta.class.sources =       src/delta.c  
deny.class.sources =        src/deny.c  
dist.class.sources =        src/dist.c  
divide.class.sources =      src/divide.c  
divmod.class.sources =      src/divmod.c  
edge.class.sources =        src/edge.c  
expo.class.sources =        src/expo.c  
fifo.class.sources =        src/fifo.c  
gauss.class.sources =       src/gauss.c  
gestalt.class.sources =     src/gestalt.c  
history.class.sources =     src/history.c  
ignore.class.sources =      src/ignore.c  
iso.class.sources =         src/iso.c  
lifo.class.sources =        src/lifo.c  
limit.class.sources =       src/limit.c  
linear.class.sources =      src/linear.c  
listfifo.class.sources =    src/listfifo.c  
listfunnel.class.sources =  src/listfunnel.c  
match.class.sources =       src/match.c  
#maxlib.class.sources =     src/maxlib.c  
minus.class.sources =       src/minus.c  
mlife.class.sources =       src/mlife.c  
multi.class.sources =       src/multi.c  
nchange.class.sources =     src/nchange.c  
#netclient.class.sources =   src/netclient.c  
#netdist.class.sources =     src/netdist.c  
#netrec.class.sources =      src/netrec.c  
#netserver.class.sources =   src/netserver.c  
nroute.class.sources =      src/nroute.c  
pitch.class.sources =       src/pitch.c  
plus.class.sources =        src/plus.c  
poisson.class.sources =     src/poisson.c  
pong.class.sources =        src/pong.c  
pulse.class.sources =       src/pulse.c  
pwrap.class.sources =       src/pwrap.c
remote.class.sources =      src/remote.c  
rewrap.class.sources =      src/rewrap.c  
rhythm.class.sources =      src/rhythm.c  
scale.class.sources =       src/scale.c  
score.class.sources =       src/score.c  
speedlim.class.sources =    src/speedlim.c  
split.class.sources =       src/split.c  
step.class.sources =        src/step.c  
subst.class.sources =       src/subst.c  
sync.class.sources =        src/sync.c  
temperature.class.sources = src/temperature.c  
tilt.class.sources =        src/tilt.c  
timebang.class.sources =    src/timebang.c  
triang.class.sources =      src/triang.c  
unroute.class.sources =     src/unroute.c  
urn.class.sources =         src/urn.c  
velocity.class.sources =    src/velocity.c  
weibull.class.sources =     src/weibull.c  

extradirs = examples manual
datafiles = \
$(wildcard help/*-help.pd) \
LICENSE.txt \
README.txt \
maxlib-meta.pd

externalsdir = ..
# include Makefile.pdlibbuilder from parent or current directory 
-include $(externalsdir)/Makefile.pdlibbuilder 

ifndef Makefile.pdlibbuilder 
include Makefile.pdlibbuilder 
endif
