#!/bin/bash

PATH_BUILDROOT=$1
PWD=`pwd`
if [[ -z $PATH_BUILDROOT ]]; then
	echo "usage:"
	echo "$0 <path_to_buildroot_directory>"
	exit 1
fi

cp -rf $PWD/packages/* $PATH_BUILDROOT/package
NUM_LINES=$((`wc -l $PATH_BUILDROOT/package/Config.in | awk '{ print $1 }'`-1))i
for i in `ls $PWD/packages`; do \
	TMP="source "package/$i/Config.in""
	grep --quiet $i $PATH_BUILDROOT/package/Config.in || sed -i "$NUM_LINES $TMP" $PATH_BUILDROOT/package/Config.in
done
