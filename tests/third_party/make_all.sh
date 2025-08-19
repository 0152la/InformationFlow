#!/bin/sh

subdirs=$(find . -type d)
for x in $subdirs
do
    (cd $x && make)
done
