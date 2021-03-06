#!/bin/sh

cd `dirname $0`

export BASENAME=${PWD##*/}
export SCRIPTDIR=$PWD
export BUILDDIR=$PWD/build
export PLATFORM=`uname| tr '[A-Z]' '[a-z]'`

if [ "$1" = "debug" ]
then
  echo "Building $BASENAME in Debug mode"
  export DEBUG_MODE=1
else
  echo "Building $BASENAME in Release mode"
  export DEBUG_MODE=0
fi

if [ "$2" ]
then
  echo "Output directory of $2 has been requested"
  export OUTPUT_DIR=$2
fi

mkdir -p $BUILDDIR/$PLATFORM
cd $BUILDDIR/$PLATFORM
qmake $SCRIPTDIR/$BASENAME.pro
make
