#!/bin/bash
export CFG=Release
make -B
[ $? -ne 0 ] && exit -1
cd ./$CFG
cp prj prj_upx
../../bin/util/compress.sh prj_upx
#./prj_upx
