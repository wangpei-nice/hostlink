/*
   hostlink.c

   Modul per a interpretar i preparar comandes tipus Host Link
   per autòmats Omron CPM/CQM

   Smart Software 1993/99 (C)
   Tomeu Capó i Capó 1999 (C)

   Last Revision: 29/09/1999
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <curses.h>
#include "hostlink.h"
#include "propies.h"

/* 
   Calculates the FCS of a string of characters to perform the checksum.
*/

int calculaFCS(char *cad)
{
    int i,r=0;
    for(i=0;i<=strlen(cad)-1;i++) 
       r=(r^cad[i]);

    return r;
}

/*
  To define a new PLC
*/

p_plc designa_plc(int model, int node)
{
      p_plc p;

      if((p=(p_plc) malloc(sizeof(p_plc)+1))==NULL) {
        printf("malloc failed...\n");
        return 0;
      }
    
      p->model = model;
      p->node = node;

      p->darrera_comanda = NULL;
      p->trama = (char *) malloc(255);
      return p;
}

/*
   It returns the model of the PLC that is being worked with before calling it,
   it is necessary to send the model request command to the PLC.
*/

char *modelPLC(p_plc p)
{
     if(p->darrera_comanda==NULL)
       return 0;
}

/*
   Function that returns the start block of the PLC->PC response command,
   without the termination and the FCS.
*/

char *agafa_inici(char *com_hl)
{
     int i,l;
     char *paquet;
     
     l = strlen(com_hl)-5;
     i = 0;

     paquet=(char *) malloc(l);
     
     while(i<=l) {
          paquet[i]=com_hl[i];
          i++;
     }

     paquet[i]='\0';

     return paquet;
}

/* 
   It returns the node number of the PLC that sends us the command.
*/

int agafa_node(char *com_hl)
{
    int node = 0;
    node = node*10+(com_hl[1]-48);
    node = node*10+(com_hl[2]-48);
    return node;
}

/* 
   It returns the received FCS code in hex.
*/
 
int agafa_FCS(char *com_hl)
{
    int p_FCS,FCS=0;
    
    p_FCS=strlen(com_hl)-4;

    FCS=FCS*16+(com_hl[p_FCS]-48);
    FCS=FCS*16+(com_hl[p_FCS+1]-48);
    
    return FCS;
}

int finalitzacio(char *com_hl)
{
    int FIN = 0;
    FIN = FIN*16+(com_hl[5]-48);
    FIN = FIN*16+(com_hl[6]-48);
    return FIN;
}

/***********************************************************************/

int que_es(char *com_hl)
{
    int l;
  
    l = strlen(com_hl);

    if(com_hl[0]=='@' && com_hl[l-2]=='*')
      return COMANDA_OK;
    else if(com_hl[l-1]=='|')
      return COMANDA;
    else
      return -1;
}

int comprova_comanda(p_plc p, char *com_hl)
{
    if (p->node!=agafa_node(com_hl))
       return NO_NODE;

    if (finalitzacio(com_hl)!=0)
       return NO_FIN;

    return 0;
}

/*
 This function can not be compiled successfully,
 so I comment out this function.
*/
/*
int llegeix_comanda(p_plc p,char *com_hl)
{
   switch(que_es(com_hl)) {
   case COMANDA:
      comprova_comanda(p,com_hl);
      break;
   case COMANDA_OK:
      if (comprova_comanda(p,com_hl)!=0)
         afegeix_a_trama(com_hl);
      else
         return;
      break;
   case -1:
      return -1;
   }
}
*/

char *darrera_trama(p_plc p)
{
     return p->trama;
}
