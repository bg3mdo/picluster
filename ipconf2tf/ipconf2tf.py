import os
import re
import getpass
import time

#Raspberry Pi Cluster IP/Hostname TF Card Preparation Tool - ipconf2tf V0.1
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

script_path = os.path.dirname(os.path.abspath( __file__ ))

print("Raspberry Pi Cluster IP/Hostname TF Card Preparation Tool - ipconf2tf V0.1")
print("This tool helps you to move 'ipconfgen' generated files to TF Cards.")
print("Created by Yuan Wang <bg3mdo@gmail.com>")
print("Under GNU Library General Public License.")
print("You need to have /mnt/tf folder, otherwise create it: sudo mkdir /mnt/tf")
print("")

def yn_choice(message, default='y'):
    choices = 'Y/n' if default.lower() in ('y', 'yes') else 'y/N'
    choice = raw_input("%s (%s)?: " % (message, choices))
    values = ('y', 'yes', '') if choices == 'Y/n' else ('y', 'yes')
    return choice.strip().lower() in values

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    return [atoi(c) for c in re.split(r'(\d+)', text)]

print("CAUTION: The TF card has to be having Raspberry Pi OS pre imaged!")
print("Other TF cards will be NOT processed and resulted in errors!")

if not yn_choice('\nDo you want to continue?'):
    exit()

tfpath = raw_input("\nPlease give the TF card path, e.g. /dev/sdb: ")
tfpath = "/dev/sdb"
print("You entered TF card path is: " + tfpath);
if not yn_choice('\nDo you want to continue?'):
    exit()

sudopass = getpass.getpass("\nPlease give your sudo password: ")

confpath = raw_input("\nPlease providing 'conf' files folder name, e.g. confs: ")
nameprefix = raw_input("Please providing hostname prefix, e.g. pifarm_c: ")
confpath = "confs"
nameprefix = "pifarm_c"
subdirs = [x[0] for x in os.walk('.')]
confsubdir = []
confsubdirnum = 0

for item in subdirs:
    if nameprefix in item:
        confsubdir.append(item)
        confsubdirnum = confsubdirnum + 1

if confsubdirnum == 0:
    print("Make sure your input is correct, and you have 'conf' file folder!")
    exit()

confsubdir.sort(key=natural_keys)
print("\nFound the following 'confs' sub folders:\n")
for item in confsubdir:
    print(item)
print("\nThere are total " + str(confsubdirnum) + " sub folders.")

print("\nProvide which 'confs' range to move to TF card.")
print("If you input is X to X to process, only the X-st sub folder content will be processed.\n")
stindex = raw_input("Please enter from which start index to process: ")
endindex = raw_input("Please enter from which end index to process: ")

if stindex < confsubdirnum or endindex < confsubdirnum:
    print("Out of conf folder number range!")
    exit()

if stindex > endindex:
    print("End index can NOT greater than start index!")
    exit()
if stindex < 1 or endindex < 1:
    print("You cannot input negative numbers! It starts from 1.")
    exit()

mntcmd = "sudo mount " + tfpath + "2" + " /mnt/tf"
umntcmd = "sudo umount /mnt/tf"


for i in range(int(stindex), int(endindex) + 1):
    cpyipcmd = "sudo cp " + script_path + confsubdir[i - 1][1:] + "/dhcpcd.conf /mnt/tf/etc"
    cpyhostcmd = "sudo cp " + script_path + confsubdir[i - 1][1:] + "/hostname /mnt/tf/etc"
    print("\nIMPORTANT: Inserting the target TF card to card reader and plug into USB.")
    print("IMPORTANT: Wait your Linux OS recognise the card.")
    if not yn_choice('Card reader read/write LED is not blinking?'):
        exit()
    print("Copying '" + confsubdir[i-1] + "' configuration files to TF card...")
    if not yn_choice('Do you want to continue?'):
        exit()
    print("Processing...")
    pm = os.system('echo %s|sudo -S %s' % (sudopass, mntcmd))
    time.sleep(1)
    pcp1 = os.system('echo %s|sudo -S %s' % (sudopass, cpyipcmd))
    time.sleep(0.5)
    pcp2 = os.system('echo %s|sudo -S %s' % (sudopass, cpyhostcmd))
    time.sleep(1)
    pum = os.system('echo %s|sudo -S %s' % (sudopass, umntcmd))
    time.sleep(1)
    print("Finished the process.")
    print("Now remove current card and insert next card if you need to process more cards.")

print("All job done :)")


