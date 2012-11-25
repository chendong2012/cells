#!/bin/bash
secrect=(1 2 3 4 5 6 7 8)
./clewarecontrol -w esecrect2 -l 8 "${secrect[@]}"
