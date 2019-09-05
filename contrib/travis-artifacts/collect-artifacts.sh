#!/bin/bash -ex

if [ $# -lt 5 ] ; then
    echo "Usage: $0 basedir outdir releasedir commit host"
    exit 1
fi

BASEDIR=$1
OUTDIR=$2
RELEASEBASE=$3
COMMIT=$4
HOST=$5

RELEASEDIR=$BASEDIR/$RELEASEBASE/$COMMIT/$HOST

cd src
ls
cd ..
mkdir -p $BASEDIR/bitcoin-$HOST
cd $BASEDIR/bitcoin-$HOST

mkdir -p $RELEASEDIR
ZIPFILES=$(ls $OUTDIR/bin/* || true)

# Linux artifacts
[ -z "$ZIPFILES" ] || \
       zip -uj $RELEASEDIR/VLC-$COMMIT.zip ${ZIPFILES}

# MaxOSX artifacts
cp -a *.dmg $RELEASEDIR || true

# Windows artifacts
cp -a *.exe $RELEASEDIR || true

find $RELEASEDIR