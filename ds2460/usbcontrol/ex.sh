#!/bin/bash
./write_inputdata.sh
sleep 1
./write_esecrect3.sh
sleep 1
./write_compute_from_esecrect3_to_macbuf.sh
sleep 1
./read_mac.sh
