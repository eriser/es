#!/bin/bash
strip --strip-all -v -R .comment $1
upx -9 $1
sleep 1
