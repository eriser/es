#!/bin/bash

./util/env.sh

header="PROJECT:"

cd ../es_linux_editor
./clean.sh
[ $? -ne 0 ] && exit -1

cd ../es_linux_lib
./clean.sh
[ $? -ne 0 ] && exit -1

cd ../es_linux_rt
./clean.sh
[ $? -ne 0 ] && exit -1

cd ../eswav_linux
./clean.sh
[ $? -ne 0 ] && exit -1

cd ../ptch_linux
./clean.sh
[ $? -ne 0 ] && exit -1

cd ../bin

echo -e "\n\nall done!"
sleep 3
