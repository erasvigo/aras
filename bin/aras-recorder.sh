#!/bin/sh
#
#Script for start aras(player|daemon|recorder) with the proper config file
#By martintxo@sindominio.net. License: the same of ARAS.

if [ -f /etc/aras/aras.conf ];then
  /usr/bin/aras-recorder /etc/aras/aras.conf
elif [ -f ~/.aras/aras.conf ];then
  /usr/bin/aras-recorder ~/.aras/aras.conf
fi
exit
