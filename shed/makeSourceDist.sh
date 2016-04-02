#!/bin/bash
#
CURRENTDIR=`pwd`
CURRENTPROJECT=`basename $CURRENTDIR`

pushd .. || exit

tar cvzf ${CURRENTPROJECT}_sources.tgz \
 ${CURRENTPROJECT}/maxlib-meta.pd \
 ${CURRENTPROJECT}/src/ \
 ${CURRENTPROJECT}/help/ \
 ${CURRENTPROJECT}/LICENSE.txt \
 ${CURRENTPROJECT}/Makefile \
 ${CURRENTPROJECT}/Makefile.pdlibbuilder \
 ${CURRENTPROJECT}/README.txt \
 ${CURRENTPROJECT}/README.md \
 ${CURRENTPROJECT}/shed

popd
