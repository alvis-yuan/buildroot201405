#!/bin/sh
make distclean
if [ $? -ne 0 ]; then
  	echo "make distclean failed"
	exit 1
else
	echo "make distclean succeed
############################################
	start to make 80 config"
fi
make sunmi_x1021_cloud_printer80_defconfig
if [ $? -ne 0 ]; then
  	echo "make sunmi_x1021_cloud_printer80_defconfig failed"
	exit 1
else
	echo "make sunmi_x1021_cloud_printer80_defconfig succeed
############################################
	start to make 80 project"
	make
fi
