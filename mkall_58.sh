#!/bin/sh
make distclean
if [ $? -ne 0 ]; then
  	echo "make distclean failed"
	exit 1
else
	echo "make distclean succeed
############################################
	start to make 58 config"
fi
make sunmi_x1021_defconfig
if [ $? -ne 0 ]; then
  	echo "make sunmi_x1021_defconfig failed"
	exit 1
else
	echo "make sunmi_x1021_defconfig succeed
############################################
	start to make 58 project"
	make
fi
