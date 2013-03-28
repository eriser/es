#!/bin/bash

./util/env.sh

header="PROJECT:"

echo -e "\n$header es_editor\n"
cd ../es_linux_editor
./doit.upx.sh
[ $? -ne 0 ] && exit -1
cp Release/prj_upx ../bin/editor/${PRJ}_editor_linux
[ $? -ne 0 ] && exit -1

echo -e "\n\n$header libes\n"
cd ../es_linux_lib
./doit.sh
[ $? -ne 0 ] && exit -1
cp libes.a ../bin/lib/lib${PRJ}_linux_static.a
[ $? -ne 0 ] && exit -1

echo -e "\n\n$header eswav\n"
cd ../eswav_linux
./doit.upx.sh
[ $? -ne 0 ] && exit -1
cp Release/prj_upx ../bin/eswav/eswav_${PRJ}_linux
[ $? -ne 0 ] && exit -1

echo -e "\n\n$header ptch\n"
cd ../ptch_linux
./doit.upx.sh
[ $? -ne 0 ] && exit -1
cp Release/prj_upx ../bin/ptch/ptch_${PRJ}_linux
[ $? -ne 0 ] && exit -1

cd ../bin

echo -e "\n\nall done!"
sleep 3
