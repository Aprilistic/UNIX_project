#!/bin/bash

mkdir -p res
od -An -t d4 data/IO_CLIENT_0.dat > res/io_c0.txt
od -An -t d4 data/IO_CLIENT_1.dat > res/io_c1.txt
od -An -t d4 data/IO_CLIENT_2.dat > res/io_c2.txt
od -An -t d4 data/IO_CLIENT_3.dat > res/io_c3.txt
od -An -t d4 data/IO_SERVER_0.dat > res/io_s0.txt
od -An -t d4 data/IO_SERVER_1.dat > res/io_s1.txt
od -An -t d4 data/IO_SERVER_2.dat > res/io_s2.txt
od -An -t d4 data/IO_SERVER_3.dat > res/io_s3.txt
od -An -t d4 data/CP_CLIENT_0.dat > res/cp_c0.txt
od -An -t d4 data/CP_CLIENT_1.dat > res/cp_c1.txt
od -An -t d4 data/CP_CLIENT_2.dat > res/cp_c2.txt
od -An -t d4 data/CP_CLIENT_3.dat > res/cp_c3.txt
od -An -t d4 data/CP_SERVER_0.dat > res/cp_s0.txt
od -An -t d4 data/CP_SERVER_1.dat > res/cp_s1.txt
od -An -t d4 data/CP_SERVER_2.dat > res/cp_s2.txt
od -An -t d4 data/CP_SERVER_3.dat > res/cp_s3.txt