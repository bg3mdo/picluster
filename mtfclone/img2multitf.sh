#!/bin/bash

#Image File to Multiple TF Cards Clone Tool V0.1
#Created by Yuan Wang <bg3mdo@gmail.com>
#
#Copyright (C) 2019 by Yuan Wang BG3MDO
#
#This library is free software; you can redistribute it and/or
#modify it under the terms of the GNU Library General Public
#License as published by the Free Software Foundation; either
#version 2 of the License, or (at your option) any later version.
#
#This library is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#Library General Public License for more details.
#
#You should have received a copy of the GNU Library General Public
#License along with this library; if not, write to the
#Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
#Boston, MA  02110-1301, USA.

echo "This is an image file to multiple TF cards clone tool V0.1"
echo "By Yuan Wang <bg3mdo@gmail.com>"
echo "Syntax: img2multitf.sh [image file path] [dev1] [dev2] ... [devN]"
echo "[devX] will be like sdb, sdc ..."
echo "Please handle this carefully, some card readers has more slot, more devs."
echo ""

if [ $# -eq 0 ]; then
	echo "There is no any parameters supplied!"
	exit 0
fi

if [ $# -eq 1 ]; then
	echo "Please check syntax!"
	exit 0
fi

if [ ! -e $1 ]; then
	echo "Image file is NOT found!"
	exit 0
fi

echo "CAUTION: the following disk/s will be WIPED and WROTE:"

i=2
while [ $i -le $# ]; do
	eval par=\$$i
	echo $par
	i=$(($i + 1))
done

echo ""

read -r -p "Are you sure to process? [y/N] " response
echo ""
case "$response" in
    [yY][eE][sS]|[yY]) 

	i=2
	while [ $i -le $# ]; do
		eval par=\$$i
		if [ ! -e "/dev/$par" ]; then
			echo "Can NOT find /dev/$par"
			echo "Exit the process."
			exit 0
		fi
		i=$(($i + 1))
	done

       	echo "Start to process TF card/s."
	echo ""
	i=2
	while [ $i -le $# ]; do
		eval par=\$$i
		echo "Starting to burn TF card $par ..."
		dd if=$1 of=/dev/$par bs=1M status=progress
		echo "Have completed TF card $par :)"
		i=$(($i + 1))
	done
        ;;
    *)
        echo "Exit the process."
        ;;
esac


