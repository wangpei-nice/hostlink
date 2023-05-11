
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS10"

int CalculaFCS(char *cads)
{
    int i,r;

    r=0;

    for(i=0;i<=strlen(cads)-1;i++) {
       r=(r ^ cads[i]);
       printf("%c,",cads[i]);
    }

    return r;
}

int main()
{
     int fd,c,res,i;
     struct termios oldtio,newtio;
     char buf[255],ins[10];

     printf("Opening the port..\n");
    
     fd = open(MODEMDEVICE,O_RDWR | O_NOCTTY);
     
     if (fd<0) {
        perror(MODEMDEVICE);
	     //exit(-1);
     }
 
         
     tcgetattr(fd,&oldtio);                     // Saves the current port values
     bzero(&newtio,sizeof(newtio));             // Empty the values ​​to initialize them
     printf("Configuring the port...\n");

     // Initialize the data transmission type
     // OMROM CPM1: 9600,7,E,2
     //

     newtio.c_cflag = BAUDRATE | CRTSCTS | CS7 | CSTOPB | PARENB | CLOCAL | CREAD;
     
     newtio.c_iflag = ICRNL;         
     newtio.c_oflag = 0;
     newtio.c_lflag = ICANON;
     
     // Initialize the timeout
     
     newtio.c_cc[VTIME] = 10;
     newtio.c_cc[VMIN]  = 5;
     
     tcflush(fd,TCIFLUSH);
     tcsetattr(fd,TCSANOW,&newtio);
     
     bcopy("@00MM40*\r\0",&buf,10);
     bcopy("@00MM",&ins,6);

     printf("%s %2x\n",ins,CalculaFCS(ins));

     printf("Sending status request...\n");
     write(fd,buf,10); 
           
     printf("Waiting for an answer...\n");
     res = read(fd,buf,255);

     buf[res]=0;
     printf("reply: %s",buf); 
    
     tcsetattr(fd,TCSANOW,&oldtio);
     return 0;
}
