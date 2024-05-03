#!/bin/sh
#
#Script for start aras(player|daemon|recorder) with the proper config file
#By martintxo@sindominio.net. License: the same of ARAS.

if [ -f /etc/aras/aras.conf ];then
  /usr/bin/aras-daemon /etc/aras/aras.conf
elif [ -f ~/.aras/aras.conf ];then
  /usr/bin/aras-daemon ~/.aras/aras.conf
fi
exit
