maxlib - music analysis extensions library
copyright (c) 2002-2003 by Olaf Matthes

This git-fork of the Pure-data library maxlib is cloned from https://git.puredata.info/cgit/svn2git/libraries/maxlib.git, which is the svn-to-git from https://sourceforge.net/p/pure-data/svn/HEAD/tree/trunk/externals/maxlib/.

The initial changes were adding/recreating some separate directories for source and help files and replace the buildsystem by pdlibbuilder. This to simplify creation of a deken package of the libary.

See the original README.txt for more information.

F.J. Kraan
fjkraan@xs4all.nl
2016-02-19

Release notes:
- Changed version to 1.6,
- Fixed help symbol for arbran, beta, bilex, cauchy, delta, expo, gauss, linear, poisson, triang, weibull
- Added checks for missing arrays: arbran, subst,
- Removed alias 'd' for dist,
- Removed error for second inlet of pitch. Have to find out its usage,
- Applied sourceforge patch 1199 for garray_getfloatwords issue,

ToDo:
- Fix instability of netclient, netserver (or remove all net* objects),
- Change array code to support delayed loading (arbran, subst, ..),
