#!/bin/bash
# post-image.sh for SoCkit
# 2014, "Roman Diouskine" <roman.diouskine@savoirfairelinux.com>
# 2014, "Sebastien Bourdelin" <sebastien.bourdelin@savoirfairelinux.com>

# create a DTB file copy with the name expected by the u-boot config
#cp -af $BINARIES_DIR/socfpga_cyclone5_sockit.dtb  $BINARIES_DIR/socfpga.dtb


DATE="$(date +%Y%m%d)"
FW_FILE="Printer_images_$2_$DATE.zip"
OTA_FILE="NT320_ota_firmware_images_$2_$DATE.zip"

TARGET_BUILD_DIR="${OUTDIR}/images/"
HWINFO_WORK_DIR="sunmi/x2600/hwinfo/"

write_version(){
		echo "type=firmware" > version.txt
		echo "model=NT320" >> version.txt
		echo "submodel=NT320_U,NT320_S,NT320_W,NT320_L" >> version.txt
		echo "version=$1" >> version.txt
		echo "vender=SUNMI" >> version.txt
		echo "min_version=1.0.0" >> version.txt
}


#source sunmi/x2600/hwinfo/hwinfo_bin_generate.sh
#cp sunmi/x-loader/x-loader-pad-with-sleep-lib.bin ${OUTDIR}/images
cp sunmi/x2600/hwinfo/hwinfo.bin ${OUTDIR}/images
cp ${OUTDIR}/images/rootfs.squashfs ${OUTDIR}/images/rootfs_bak.squashfs
cp ${OUTDIR}/images/xImage ${OUTDIR}/images/xImage_bak
#mkfs.ubifs -c 1760 since the max flash size for data partition is (92MB+128MB) = 220MB: 220*1024KB/128KB = 1760 
#mkfs.ubifs -d sunmi/x2600/datafs -e 0x1f000 -c 1760 -m 0x800 -o ${OUTDIR}/images/datafs.ubifs
mkdir -p PrinterImage
mkdir -p PrinterImage/APP
touch PrinterImage/APP/APPINFO
echo name:FW > PrinterImage/APP/APPINFO
echo type:FIRMWARE >> PrinterImage/APP/APPINFO
echo desc:FW >> PrinterImage/APP/APPINFO
echo vender:SUNMI >> PrinterImage/APP/APPINFO
echo version:$2 >> PrinterImage/APP/APPINFO
echo model:NT320-XXX >> PrinterImage/APP/APPINFO
cp ${OUTDIR}/images/rootfs.squashfs PrinterImage/APP/
md5sum ${OUTDIR}/images/rootfs.squashfs|awk '{printf $1}' >PrinterImage/APP/rootfs.squashfs.md5file
cp ${OUTDIR}/images/xImage PrinterImage/APP/
md5sum ${OUTDIR}/images/xImage|awk '{printf $1}' >PrinterImage/APP/xImage.md5file
find ${OUTDIR} -name $FW_FILE -delete
zip -r $FW_FILE PrinterImage
write_version $2
zip $OTA_FILE $FW_FILE version.txt
mv $FW_FILE ${OUTDIR}
mv $OTA_FILE ${OUTDIR}
rm -rf PrinterImage
