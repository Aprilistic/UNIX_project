#!/bin/sh

echo "Show the first 8 lines of the data file"
od -D data/p0.dat | head -8

echo "Show the first 8 lines of client IO_0"
od -D data/IO_CLIENT_0.dat | head -8

echo "Show the first 8 lines of server IO_3"
od -D data/IO_SERVER_3.dat | head -8

