#include <stdio.h>
#include <stdlib.h>
#include <graph.h>

struct videoconfig vc;
short XRES;          /* x-rezolucija aktivne graficke kartice */
short YRES;          /* y-rezolucija aktivne graficke kartice */
short XTXT;          /* broj tekst kolona */
short YTXT;          /* broj tekst vrsta */
short YHLF;          /* vrsta po simetrali table */
short CLR;           /* aktivna boja */

float A;
float B;
int KX=0;
int KY=0;
int STEP;
double ZOOM;
short VMODE;

FILE *INPUT;
char FILE_NAME[30];

int view();
void drawline(float x1,float y1,float x2,float y2);
void print(float x1,float y1, char *txt);
void parameters();


main(int argc,char *argv[])
{
int aktivgc,gc;
char c;

/*printf("\n");
printf("GCLC/LaTeX  Previewer  Version 1.00\n");
printf("Written by Predrag Janicic, Belgrade.\n");
printf("Copyright (c) 1996. PJ Software. Not for commercial use.\n");
printf("\n");*/
if (argc<2) 
  { printf("Error in command line. Input file not specified.\n"); exit(-1); }
/*printf("Input file: %s\n",argv[1]);
printf("\n");*/

INPUT=fopen(argv[1],"r");
if (INPUT==NULL) 
  { printf("File error. Can not open input file !\n "); exit(0); }
strcpy(FILE_NAME,argv[1]);

_getvideoconfig(&vc);
aktivgc=0;
if (vc.adapter & _HGC) VMODE=_HERCMONO;
if (vc.adapter & _EGA) VMODE=_ERESCOLOR;
if (vc.adapter & _CGA) VMODE=_HRESBW;
if (vc.adapter & _VGA) VMODE=_VRES16COLOR;
aktivgc=_setvideomode(VMODE);

if (aktivgc==0)
  {
    _clearscreen(_GCLEARSCREEN);
    printf("If you have HERCULES graphic adapter, you have to");
    printf("activate driver by command MSHERC \n");
    exit(0);
  }

_getvideoconfig(&vc);
XRES=vc.numxpixels;
YRES=vc.numypixels;
XTXT=vc.numtextcols;
YTXT=vc.numtextrows;
YHLF=(11*YTXT)/25;

A=160.00;
B=3*A/4;
STEP=8;
ZOOM=1;
parameters();
view();
parameters();
fclose(INPUT);

for(;;)
 { 
   while (!kbhit());
   c=getch();
   if ((c=='f')||(c=='F')||(c=='c')||(c=='C'))
     {
       if (((c=='f')||(c=='F'))&&(STEP>1)) STEP=STEP/2;
         else if (((c=='c')||(c=='C'))&&(STEP<256)) STEP=STEP*2;
     }
   if ((c=='q')||(c=='Q')||(c=='+')||(c=='-')||
       (c=='K')||(c=='M')||(c=='H')||(c=='P'))
     {
       INPUT=fopen(argv[1],"r");   
       if ((c=='q')||(c=='Q')) { _setvideomode(_DEFAULTMODE); exit(0); }
       if ((c=='+')&&(ZOOM<10)) { ZOOM=ZOOM*1.20; A=A/1.20; B=3*A/4; }
       if ((c=='-')&&(ZOOM>0.12)) { ZOOM=ZOOM/1.20; A=A*1.20; B=3*A/4; }
       if (c=='K') KX=KX+STEP;
       if (c=='M') KX=KX-STEP;
       if (c=='H') KY=KY+STEP;
       if (c=='P') KY=KY-STEP;
       _clearscreen(_GCLEARSCREEN);
       parameters();
       view();
       fclose(INPUT);   
     }
   parameters();
  } 
}


int xekransko(float x)
{
return (int)((x*((float)XRES))/A)+KX;
}


int yekransko(float y)
{
return YRES-(int)((y*(float)YRES)/B)+KY;
}

  
box(int x1,int y1,int x2,int y2,int n,int d)
{
/* n ozna~ava visinu zaglavlja; d ozna~ava debljinu linije */
int i,j;
_settextposition(y1,x1);
if (d==2) _outtext(" É"); else _outtext(" Ú");
for(i=x1+1;i<=x2-3;i++) 
  if (d==2) _outtext("Í"); else _outtext("Ä");
if (d==2) _outtext("» "); else _outtext("¿ ");
for(i=y1+1;i<=y2-1;i++)
  if (i==(y1+n))
    {
      _settextposition(i,x1);
      if (d==2) _outtext(" Ç"); else _outtext(" Ã");
      for(j=x1+1;j<=x2-3;j++) _outtext("Ä");
       if (d==2) _outtext("¶ "); else _outtext("´ ");
    }
      else
    {
      _settextposition(i,x1);
      if (d==2) _outtext(" º"); else _outtext(" ³");
      for (j=x1+1;j<=x2-3;j++) _outtext(" ");       
       if (d==2) _outtext("º "); else _outtext("³ ");
    }

_settextposition(y2,x1);
if (d==2) _outtext(" È"); else _outtext(" À");
for(i=x1+1;i<=x2-3;i++)
  if (d==2) _outtext("Í"); else _outtext("Ä");
if (d==2) _outtext("¼ "); else _outtext("Ù ");
}  


int view()
{
int i,j, k;
float x1,y1,x2,y2,thickness;
char c, txt[100];
for(;;)
  {
    c=(char)fgetc(INPUT); 
    if (c==EOF) return -1;
    if (c==92) 
      {
        c=(char)fgetc(INPUT); 
        if (c=='e')
          {
            for(i=1;i<=6;i++) c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&x1);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&y1);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%i",&j);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&x2);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&y2);               
            drawline(x1,y1,x2,y2);
          }
          else                                      
        if (c=='p')
          {
            for(i=1;i<=3;i++) c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&x1);
            c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&y1);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
          /*  for(i=1;i<=15;i++) c=(char)fgetc(INPUT); */
          /* changed 18.07.1997                        */
            for(i=1;i<=18;i++) c=(char)fgetc(INPUT);
            if (c=='{')  c=(char)fgetc(INPUT);
            fscanf(INPUT,"%s",txt);
            k=0;
            while (txt[k]!='\0') k++;
            txt[k-3]='\0';
            print(x1,y1,txt);
          } 
          else    
        if (c=='b')
          {
            for(i=1;i<=14;i++) c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&x1);
            c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&y1);
            _setlinestyle(1);
            drawline(0.00,0.00,x1,0.00);
            drawline(x1,0.00,x1,y1);            
            drawline(x1,y1,0.00,y1);            
            drawline(0.00,y1,0.00,0.00);
            _setlinestyle(-1);
          }
          else
         if (c=='g')
          {
            for(i=1;i<=8;i++) c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&x1);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&y1);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%i",&j);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&x2);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&y2);
            c=(char)fgetc(INPUT); c=(char)fgetc(INPUT); 
            fscanf(INPUT,"%f",&thickness);
            drawline(x1,y1,x2,y2);
          }

        }
  }
return 1;
}


void drawline(float x1,float y1,float x2,float y2)
{
int x1e,y1e,x2e,y2e;
x1e=xekransko(x1);
y1e=yekransko(y1);
x2e=xekransko(x2);
y2e=yekransko(y2);
_moveto(x1e,y1e);
_lineto(x2e,y2e);
}


void print(float x,float y, char *txt)
{
int xt,yt,xe,ye;
xe=xekransko(x);
ye=yekransko(y);
xt=(int)(((float)80*xe)/XRES);
yt=(int)(((float)YTXT*ye)/YRES);
if ((xt>1)&&(xt<80)&&(yt>1)&&(yt<YTXT))
  { 
    _settextposition(yt+1,xt+1);
  /*  printf("*");*/
    printf("%s",txt);
  }
}


void parameters()
{
_settextposition(YTXT,1);
printf("  GCLC Previewer    FILE: %s",FILE_NAME);
_settextposition(YTXT,44);
printf(" ZOOM: %f ",ZOOM);
_settextposition(YTXT,64);
printf(" STEP: %i ",STEP);
}




