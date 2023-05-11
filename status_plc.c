
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <malloc.h>
#include "comm.h"
#include "hostlink.h"
#include "models.h"

int main(void)
{
     p_port COM5;
     p_plc cpm1;
     int resposta;

     COM5 = defineix_port("/dev/ttyS10",B9600, CS7 | CSTOPB | PARENB);
     cpm1 = designa_plc(0x11,0);
     
     obre_port(COM5);
  
     printf("Port obert..\n"); 
 
     envia_comanda(COM5,"@00MM40*\r"); //send
     rebre_comanda(COM5);     //receive

     printf("Response received: %10s\n",in_buffer(COM5));
     printf("Command interpreted: %d\n",que_es(in_buffer(COM5)));
     printf("Checking command: %d\n",comprova_comanda(cpm1,in_buffer(COM5)));


     tanca_port(COM5);//close the port

    // free(cpm1);
     free(COM5);

}
