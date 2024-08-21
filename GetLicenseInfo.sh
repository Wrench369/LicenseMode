#!/bin/bash -e
exec 2>/dev/null
if [ ! -f "/usr/sbin/dmidecode" ]; then apt -y install dmidecode; fi
if [ ! -f "/sbin/hdparm" ]; then apt -y install hdparm; fi
if [  -f ~/sysInfo ]; then rm -f ~/sysInfo; fi
uuid=$(dmidecode -s system-uuid)
proccesorid=$(dmidecode -t processor |grep ID |head -1|sed 's/ID:/ /g')
baseboard=$(dmidecode -s baseboard-serial-number)
mac=$(awk '(!/^[#|$]/)&&/./{print $4}' /etc/udev/rules.d/70-persistent-net.rules |sed 's/ATTR{address}=="/ /g'|sed 's/",//g')
harddisk=$(hdparm -i /dev/sda | grep SerialNo |awk '{print $3}'|sed 's/SerialNo=//g')
sec=$(echo ${proccesorid}%${baseboard}%${mac}%${harddisk}%${uuid}| sed 's/ //g')
echo $sec|base64 -i|awk '{<!-- -->{printf "%s",$0}}'