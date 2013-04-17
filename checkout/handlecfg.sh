#!/bin/bash
configfile="$1"
sed -i 's/buildmode=user,once/buildmode=eng/' "$configfile"
sed -i 's/buildmode=eng,once/buildmode=user/' "$configfile"

sed -i 's/switch=on,once/switch=off/' "$configfile"
sed -i 's/switch=off,once/switch=on/' "$configfile"
