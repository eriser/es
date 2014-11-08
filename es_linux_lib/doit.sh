#!/bin/bash

export CFG=Release
make
cd Release
if [ -f libes.a ]
then
	rm libes.a
fi
ar rs libes.a *.o
cp libes.a ..
#read -p "press any key..."
