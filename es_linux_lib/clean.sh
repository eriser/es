#!/bin/bash
pwd
if [ -d Debug ]; then
	rm -r Debug
fi
if [ -d Release ]; then
	rm -r Release
fi
if [ -e libes.a ]; then
	rm libes.a
fi
