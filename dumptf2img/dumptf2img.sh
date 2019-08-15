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

echo "TF Card Image Dump Tool V0.1"
echo "By Yuan Wang <bg3mdo@gmail.com>"
echo "Syntax: sudo sh dumptf2img.sh [/dev/sdX] [image file (x.img) path]"
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
	echo "TF card is NOT found!"
	exit 0
fi

echo "Starting to dump TF card $1 ..."
dd if=$1 of=$2 bs=1M status=progress
echo "Have completed TF card dump :)"
echo ""
echo "Image is stored at $2"
echo "Exit the process."
