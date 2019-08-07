/*

Raspberry Pi Cluster Power Board Debug LED Service
Created by Yuan Wang <bg3mdo@gmail.com>
Power supply board UH HW Ver 2.0, SW Ver 1.0
C is much quicker than Python operations.

Copyright (C) 2019 by Yuan Wang BG3MDO

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
Boston, MA  02110-1301, USA.

*/

#include "gpiolib.h"
#define PIDBUF_MAX 10            /* Large enough to hold maximum PID as string */

#define DEBUG1  17 /* GPIO27 */
#define DEBUG2  27 /* GPIO17 */
#define SLOWMS  1000
#define FASTMS  250

int DEBUGsInit_OFF()
{
	if (-1 == GPIOExport(DEBUG1) || -1 == GPIOExport(DEBUG2))
		return(1);
	if (-1 == GPIODirection(DEBUG1, OUT) || -1 == GPIODirection(DEBUG2, OUT))
		return(2);
    if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, LOW))
        return(3);
    return -1;
}

int DEBUGsDeInit(){
	if (-1 == GPIOUnexport(DEBUG1) || -1 == GPIOUnexport(DEBUG2))
		return(4);
    return -1;
}

int DEBUG1_On(){
    DEBUGsInit_OFF();
    if (-1 == GPIOWrite(DEBUG1, HIGH) || -1 == GPIOWrite(DEBUG2, LOW))
        return(3);
    while (1){
        usleep(1000 * SLOWMS);
    }
    return -1;
}

int DEBUG2_On(){
    DEBUGsInit_OFF();
    if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, HIGH))
        return(3);
    while (1){
        usleep(1000 * SLOWMS);
    }
    return -1;
}

int DEBUG1_SlowBlink(){
    DEBUGsInit_OFF();
    while(1){
        if (-1 == GPIOWrite(DEBUG1, HIGH) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * SLOWMS);
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * SLOWMS);
    }
    return -1;
}

int DEBUG2_SlowBlink(){
    while(1){
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * SLOWMS);
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, HIGH))
            return(3);
        usleep(1000 * SLOWMS);
    }
    return -1;
}

int DEBUG1_FastBlink(){
    DEBUGsInit_OFF();
    while(1){
        if (-1 == GPIOWrite(DEBUG1, HIGH) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * FASTMS);
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * FASTMS);
    }
    return -1;
}

int DEBUG2_FastBlink(){
    DEBUGsInit_OFF();
    while(1){
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * FASTMS);
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, HIGH))
            return(3);
        usleep(1000 * FASTMS);
    }
    return -1;
}

int DEBUGCrossover_SlowBlink(){
    DEBUGsInit_OFF();
    while(1){
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        if (-1 == GPIOWrite(DEBUG1, HIGH) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * SLOWMS);
        if (-1 == GPIOWrite(DEBUG1, HIGH) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, HIGH))
            return(3);
        usleep(1000 * SLOWMS);
    }
    return -1;
}

int DEBUGCrossover_FastBlink(){
    DEBUGsInit_OFF();
    while(1){
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        if (-1 == GPIOWrite(DEBUG1, HIGH) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        usleep(1000 * FASTMS);
        if (-1 == GPIOWrite(DEBUG1, HIGH) || -1 == GPIOWrite(DEBUG2, LOW))
            return(3);
        if (-1 == GPIOWrite(DEBUG1, LOW) || -1 == GPIOWrite(DEBUG2, HIGH))
            return(3);
        usleep(1000 * FASTMS);
    }
    return -1;
}

void HelpInfo(){
    printf("\n");
    printf("Syntax: $ledservice -[parameter: h/d1/d2 ... off]\n\n");
    printf("-h      Print this message.\n");
    printf("-d1     Turn on DEBUG 1 LED.\n");
    printf("-d2     Turn on DEBUG 2 LED.\n");
    printf("-sb1    Slow blink DEBUG 1 LED.\n");
    printf("-sb2    Slow blink DEBUG 2 LED.\n");
    printf("-fb1    Fast blink DEBUG 1 LED.\n");
    printf("-fb2    Fast blink DEBUG 2 LED.\n");
    printf("-sc     Slow blink between DEBUG LEDs.\n");
    printf("-fc     Fast blink between DEBUG LEDs.\n");
    printf("-off    Turn off all DEBUG LEDs.\n\n");
}

void PrintErr(){
    printf("\nYou have not given a parameter or wrong parameters!\n");
    HelpInfo();
}

int LocatePIDFile(){
    int fd = open("/var/run/ledservice.pid", O_RDONLY);
    if (fd == -1){
        printf("Could not find PID file: /var/run/ledservice.pid\n");
        return -1; //not found
    }
    //printf("There is a PID file: /var/run/ledservice.pid\n");
    return 1; //found
}

int IsValidPID(int pid){
    char pidnamebuf[1024];
    sprintf(pidnamebuf,"/proc/%d/cmdline", pid);
    int fd = open(pidnamebuf, O_RDONLY);
    if(fd == -1){
        printf("Could not open proc PID folder: /proc/%d/cmdline\n", pid);
    }else{
        read(fd, pidnamebuf, 1024);
        if(strstr(pidnamebuf, "ledservice") != NULL){
            return 1;
        }
    }
    return -1;
}


int ChkPIDFilePID(){
    char pidbuf[PIDBUF_MAX];
    int pid;
    struct stat stat_record;
    int fd = open("/var/run/ledservice.pid", O_RDONLY);
    if (fd == -1){
        printf("Could not open PID file: /var/run/ledservice.pid\n");
        return -1; //not found
    }
    if(stat("/var/run/ledservice.pid", &stat_record))
        printf("%s\n", strerror(errno));
    else if(stat_record.st_size <= 1){
        return -1;
    }else{
        read(fd, pidbuf, PIDBUF_MAX);
        sscanf(pidbuf, "%d", &pid);
        return pid;
    }
    return -1;
}

int WritePIDFile(){
    char pidbuf[PIDBUF_MAX];
    int fd = open("/var/run/ledservice.pid", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1){
        printf("Could not generate PID file: /var/run/ledservice.pid\n");
        return -1;
    }
    write(fd, pidbuf, sprintf(pidbuf, "%d\n", getpid()));
    return 0;
}

int RemovePIDFile(){
    int status;
    status = remove("/var/run/ledservice.pid");
    if (status == 0)
        //printf("PID file deleted successfully.\n");
        usleep(1);
    else
    {
        printf("Unable to delete the PID file.\n");
    }
    return status;
}

void KillProcess(int pid){
    kill(pid,SIGKILL);
    printf("Killed previous ledservice process.\n");
}

void Singleton(){
    int pidchk;
    int validpid;
    int foundpidfile = LocatePIDFile();
    if(foundpidfile == -1){
        WritePIDFile();
    }else if(foundpidfile == 1){
        pidchk = ChkPIDFilePID();
        if(pidchk == -1){
            WritePIDFile();
        }else{
            validpid = IsValidPID(pidchk);

            if(validpid == -1){
                printf("Created new process with new PID file: /var/run/ledservice.pid\n");
                RemovePIDFile();
                WritePIDFile();

            }else if(validpid == 1){
                KillProcess(pidchk);
                RemovePIDFile();
                WritePIDFile();
            }
        }
    }
}

/*void sigint_handler(int sig){
    printf("\n");
    RemovePIDFile();
    exit(0);
}*/


int main(int argc, char *argv[]){
    printf("\nRaspberry Pi Cluster Power Board Debug LED Service \n");
    printf("Created by Yuan Wang <bg3mdo@gmail.com> \n");
    printf("Power supply board UH HW Ver 2.0, SW Ver 1.0 \n");
    printf("Under GNU Library General Public License.\n");

    Singleton();
    //RemovePIDFile();
    //return 0;
    //signal(SIGINT, sigint_handler);

    if(argc != 2){
        PrintErr();
        return 0;
    }

    if(strcmp("-h", argv[1]) == 0){
        HelpInfo();
        return 0;
    }

    if(strcmp("-d1", argv[1]) == 0){
        DEBUG1_On();
        return 0;
    }

    if(strcmp("-d2", argv[1]) == 0){
        DEBUG2_On();
        return 0;
    }

    if(strcmp("-sb1", argv[1]) == 0){
        DEBUG1_SlowBlink();
        return 0;
    }

    if(strcmp("-sb2", argv[1]) == 0){
        DEBUG2_SlowBlink();
        return 0;
    }

    if(strcmp("-fb1", argv[1]) == 0){
        DEBUG1_FastBlink();
        return 0;
    }

    if(strcmp("-fb2", argv[1]) == 0){
        DEBUG2_FastBlink();
        return 0;
    }

    if(strcmp("-sc", argv[1]) == 0){
        DEBUGCrossover_SlowBlink();
        return 0;
    }

    if(strcmp("-fc", argv[1]) == 0){
        DEBUGCrossover_FastBlink();
        return 0;
    }

    if(strcmp("-off", argv[1]) == 0){
        DEBUGsInit_OFF();
        DEBUGsDeInit();
        /*while(1){
            usleep(1000 * SLOWMS);
        }*/
        return 0;
    }

    PrintErr();

	return(0);
}
