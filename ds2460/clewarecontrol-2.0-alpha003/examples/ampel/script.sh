#!/bin/sh

while [ 1 ]
do
        clewarecontrol -as `perl -e 'print rand(3); '` `perl -e 'print rand(2); '`
done
