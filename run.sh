#!/bin/sh

cd `dirname $0`

export BASENAME=${PWD##*/}
export SCRIPTDIR=$PWD
export BUILDDIR=$PWD/build
export PLATFORM=`uname| tr '[A-Z]' '[a-z]'`

$BUILDDIR/$PLATFORM/bin/$BASENAME "$@"
