#!/bin/sh


echo "\n=== ORIGINAL FILE ==="
od -D data/input0.dat | head -8


echo "\n=== CLIENT ORIENTED ==="

echo "Compute Node 0"
od -D data/CP_CLIENT_0.dat | head -8

echo "IO Node 0"
od -D data/IO_CLIENT_0.dat | head -8

echo "\n=== SERVER ORIENTED ==="

echo "Compute Node 0"
od -D data/CP_SERVER_0.dat | head -8

echo "IO Node 0"
od -D data/IO_SERVER_0.dat | head -8

