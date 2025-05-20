#! /bin/sh
insmod /lib/modules/3.10.14/kernel/drivers/net/wireless/esp8089/esp_premalloc/esp_prealloc.ko
insmod /lib/modules/3.10.14/kernel/drivers/net/wireless/esp8089/esp8089_touch/wlan.ko power_gpio=84 fwpath="/data"
sleep 5
wpa_supplicant -D wext -i wlan0 -c /data/wpa_supplicant.conf -B
