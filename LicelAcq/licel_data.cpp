#include "stdafx.h"
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "licel_tr_tcpip.h"
#include "licel_acq.h"


void CheckTemp(FILE *fp,char *szFile,int nRetVal)
{
  if(nRetVal==EOF)
    {
      if(file_error(fp)!=0)
    {
      fprintf(stderr,"Temp File %s corrupt",szFile);
      exit(0);
    }
    }
}
void CheckFinal(FILE *fp,int nRetVal)
{
  if(nRetVal==EOF)
    {
      if(file_error(fp)!=0)
    {
      fprintf(stderr,"\nCannot write to final file");
      exit(0);
    }
    }
}
int file_error(FILE *filep)
{
  /*Error detection */
  if( ferror( filep) != 0) {
    fflush(NULL);
    /* really an error happens */
    perror("");
    clearerr(filep);
    return -2;
  }
  else {
    if( feof( filep) != 0) {
         fflush( NULL);
         fprintf( stderr, "\nEND OF FILE detected");
         return( -1);
    }
  }
  return 0;
}
int parseTRType(FILE *input, Channel *ch)
{
  char buffer[100];
  char type[3];
  int nMemoryLength;
  ch->nMHz = 20;
  ch->nTRLength = 16382;
  ch->nTRType = 0;
  if (fgets(buffer,80,input)==NULL) {
    file_error(input);
    return FALSE;
  }
 
  if (strstr(type, "PR")) {
    ch->nTRType = 1;
  }  
  if (strstr(buffer,"bit")) {
    sscanf(buffer, "%2s%02d-%d", type, &ch->nMHz, &ch->nADCBits);
  }
  else {
    sscanf(buffer, "%2s%02d-%d", type, &ch->nMHz, &nMemoryLength);
    ch->nADCBits = 12;
        switch (nMemoryLength) {
              case 640:
                ch->nTRLength = 65534;
                break;
              case 320:
                ch->nTRLength = 32766;
                break;
              case 160:
                ch->nTRLength = 16382;
                break;
              case 80:
                ch->nTRLength = 8190;
                break;
              case 40:
                ch->nTRLength = 4094;
                break;
              case 20:
                ch->nTRLength = 2046;
                break;
              case 10:
                ch->nTRLength = 1022;
                break;
              case 05:
                ch->nTRLength = 510;
                break;
              case 02:
                ch->nTRLength = 254;
                break;
              default:
                ch->nTRLength = 16382;
                break;
            }
        }
 
  return TRUE;
}

int read_configuration( FILE *input, Channel **channel, MeasureInfo **mesinfo)
{
  char szBuffer[80];
  int nDummy,i,n1;
  MeasureInfo *mes;
  Channel *ch1;
  /* Check for valid input file */
  assert( input!= NULL);

  *mesinfo= (MeasureInfo *) malloc(sizeof(MeasureInfo));
  if( *mesinfo == NULL) {
          fprintf(stderr,"\nNot enough Memory for Measureinfo");
    exit(0);
  }
  mes = *mesinfo;
  strcpy(szBuffer,"");
  while(( strlen( szBuffer) <2) && ( !feof( input))) {
    fgets(szBuffer,20,input);
  }
  nDummy = strcspn( szBuffer,":"); 
  strncpy( mes->szLocation , szBuffer, nDummy);
  read_value( input, "%d" , &mes->nAltitude); 
  read_value( input, "%lf", &mes->dLongitude);
  read_value( input, "%lf", &mes->dLatitude);
  fgets( szBuffer, 80, input);
  sscanf( szBuffer, " %c", &mes->cFirstLetter);
  fgets( szBuffer , 80, input);
  sscanf( szBuffer, " %s", mes->szOutPath);       
  read_value( input, "%d", &mes->nMaxShots);
  read_value( input, "%d", &mes->nChannelNumber);    
  if((*channel = (Channel *) malloc( sizeof( Channel) * mes->nChannelNumber)) == NULL) { 
    printf("\n Not enough memory for datasets");
    exit(0);
  }
  ch1 =* channel;
  for( i = 0; i < mes->nChannelNumber; i++) {
    ch1[i].nActive = 1;
    if(! read_value( input, "%d", &ch1[i].nTransientRecorder))  
         break;
    /* Range Check nCard between 0 and 7*/
    if((ch1[i].nTransientRecorder < 0 )|| (ch1[i].nTransientRecorder >7)) {
         LERMess(-1,"Channel Number out of Range Channel set to 0\n\
      Channel disabled");
         ch1[i].nTransientRecorder = 0;
         ch1[i].nActive = 0;
       }
    if (!parseTRType(input, &ch1[i]))
      break;
    if (!read_value( input, "%d", &n1))
         break;
    if(n1==1) {
         ch1[i].nMemory=MEM_B;
       }
    else {
         ch1[i].nMemory=MEM_A; 
       }
    if( !read_value(input,"%d",&n1))
      break;
    if(n1==1) {
         ch1[i].bPhoton=TRUE;
      }
    else {
         ch1[i].bPhoton=FALSE;
      }
    if(!read_value(input,"%d",&ch1[i].nLength))
      break;
    if(ch1[i].bPhoton) {
 	    if( !read_value(input,"%d",&ch1[i].nDAC))
           break;
         /* Range Check nDac between 0 and 63 */
         if((ch1[i].nDAC<0 )|| (ch1[i].nDAC >63)) {
                  LERMess( -1, "Discriminator Level out of Range \nSet to 4");
            ch1[i].nDAC = 4;
         }
         ch1[i].nRange = 0;
       }
    else {
         if( !read_value(input,"%d",&ch1[i].nRange))
           break;
         /* Range Check nRange between 0 and 2 */
         if((ch1[i].nRange<0) ||( ch1[i].nRange >2)) {
           LERMess(-1,"Voltage Range not valid \nSet to 500mV");
           ch1[i].nRange = 0;
         }
         ch1[i].nDAC = 0;
       }
    if( !read_value(input,"%d",&ch1[i].nHighVoltage))
      break;
      /* Range Check nHighVoltage between 0 and 10000 */
    if((ch1[i].nHighVoltage<0 )||( ch1[i].nHighVoltage >10000)) {
         LERMess(-1,"High Voltage invalid \n set to 0V");
         ch1[i].nHighVoltage=0;
       }
    if( !read_value(input,"%lf",&ch1[i].dRepRate))
      break;
    /* Range Check Reprate between 0 and 20000 */
    if((ch1[i].dRepRate<0) || (ch1[i].dRepRate >20000)) {
           LERMess(-1,"Repition Rate invalid set to 10Hz");
           ch1[i].dRepRate = 10;
       }
    if( !read_value(input,"%d",&n1))
      break;
    if(n1==0) {
         ch1[i].nDataReduction = 0;
      }
    else {
         ch1[i].nDataReduction = 1;
         if (2*ch1[i].nLength >ch1[i].nTRLength){
           printf ("!!! trace length exceeds TR capabilities\r\n");
           ch1[i].nLength = ch1[i].nTRLength / 2;
         }
      }
    if( !read_value(input,"%d",&ch1[i].nPolarisation))
        break;
    if((ch1[i].nPolarisation<0) || (ch1[i].nPolarisation >2)) {
         LERMess(-1,"Polarisation invalid  Set to unpolarized");
         ch1[i].nPolarisation=0;
       }

    if( !read_value(input,"%lf",&ch1[i].dWavelength))
      break;
    if((ch1[i].dWavelength<0) || (ch1[i].dWavelength >10000)) {
         LERMess(-1,"Wavelength invalid \n set to 0nm");
         ch1[i].dWavelength=0;
       }
    if( !read_value(input,"%d",&ch1[i].nLaserType))
      break;
    strcpy(ch1[i].szTempfile,"");     
  } 
  if( i < mes->nChannelNumber)
    mes->nChannelNumber=i;
  return(0);
}

void get_config_filename(char *complete_path, char *exe_path,const char *filename)
{
  char *stringp;
  int nStrL;
  stringp=exe_path;
#ifndef __LINUX__
  while( strchr(stringp,'\\') != NULL) {
      stringp=strchr(stringp,'\\')+1; 
  }  
#else
  while(strchr(stringp,'/')!=NULL) {
    stringp=strchr(stringp,'/')+1; 
  }  
#endif
  nStrL = strlen( exe_path);
  nStrL -= strlen( stringp);
  strncpy( complete_path, exe_path, nStrL);
  complete_path[nStrL] = 0;
  strcat( complete_path, filename);
}
/* XXX string skipping looks strange + no error handling for sscanf*/
int read_value(FILE *input,const char* format,void *ptr)
{
  char cr[80],buffer[80];
  strcpy(cr,"a");
  while(((cr[0]!='.') && (cr[0]!='+')&& (cr[0]!='-')&& !isdigit(cr[0]))&&(!feof(input))) {
    if( fgets(buffer,80,input)==NULL) {
         file_error(input);
       }
    else {
         sscanf(buffer,"%s",cr); 
       }
  }
  sscanf(buffer,format,ptr);
  return( !feof(input));
}

void LERMess(int nNumber, const char* sPlace)
{

        fprintf( stderr, "\n Error number %d has occured", nNumber);
        fprintf( stderr, "\n In Current Action \t%s\n", sPlace);
  fflush(stderr);
}
void fill_up_line(char *szBuffer,int nMax)
{
  int nStrl,i;
  nStrl = strlen( szBuffer);
  assert( nStrl < nMax);  
  for( i = nStrl; i < nMax; i++) {
      szBuffer[i] = ' ';
  }
  szBuffer[ nMax] = 0;
}
int write_temp_nasa(Channel *channel, MeasureInfo *measinfo,char *filename)
{

  int nStatus=0;
  FILE *fp,*fpStarted,*fpTemp;
  char szBuffer[81],szDummy[81];

  int i,j,k;
 
  time_t tStartTime,tFinishTime;
  struct tm *tmStart,*tmFinish;

  long int acc;
  long lNdYAGShot=0;
  long lTisaShot=0;
  int nNdYAGFreq=0;
  int nTisaFreq=0; 
  double binwidth;
  int binsCombine;
  int rawBins;
/* Test for invalid calls */ 
  assert(channel!=NULL);
  assert(filename!=NULL );
  assert(strlen(filename)!=0);

  strcpy(szBuffer,measinfo->szOutPath);
#ifdef __WIN32__
  strcat(szBuffer,"\\");
#else
  strcat(szBuffer,"/");
#endif
  strcpy(szDummy,szBuffer);
  strcat(szBuffer,"started.dat");
  if((fpStarted=fopen(szBuffer,"rt"))==NULL)
    {
      fflush(NULL);
      fprintf(stderr,"\n Cannot open %s",szBuffer);
      nStatus=EOF;
      return(nStatus);
    }
  strcat(szDummy,filename);
  if((fp=fopen(szDummy,"wb"))==NULL) {
    fflush(NULL);
    fprintf(stderr,"\n Cannot open %s",szDummy);
    nStatus=EOF;
    return(nStatus);
  }
  /* Write the first Nasa header line */
  strcpy (szBuffer, "");
  sprintf(szBuffer," %s",filename);
  fill_up_line (szBuffer,78);
  strcat (szBuffer, "\r\n");
  if(fputs(szBuffer,fp)<=0)
   {
     if((nStatus=file_error(fp))!=0)
       return(nStatus);
   }

#ifndef __OSF2__
   if(!read_value(fpStarted,"%ld",&acc)) {
#else
   if(!read_value(fpstarted," %d",&tStartTime)) {
#endif
       LERMess(EOF,"sscanf 2 in lidarsub.c");
       return(-1);
    }
  tStartTime = acc;
  if(fclose(fpStarted)!=0)
    {
      file_error(fpStarted);
      fprintf(stderr,"File error in started.dat");
    }
  /* prepare the second Nasa header line */
  tzset();
  tmStart=localtime(&tStartTime);
  /* Overlap check not implementerd */
  strcpy (szBuffer, " ");
  strncat(szBuffer,measinfo->szLocation,8); /* the Location*/
  szBuffer[9]=0;
  sprintf(szDummy," %02d/%02d/%4d %02d:%02d:%02d",tmStart->tm_mday,
          tmStart->tm_mon+1, 1900+tmStart->tm_year,tmStart->tm_hour,
          tmStart->tm_min, tmStart->tm_sec);
  strcat(szBuffer,szDummy);
  /* compute the measurement time */
  tFinishTime= time(NULL);
  tmFinish=localtime(&tFinishTime);
  sprintf(szDummy," %02d:%02d:%02d",tmFinish->tm_hour,tmFinish->tm_min,
          tmFinish->tm_sec);
  strcat(szBuffer,szDummy);
  sprintf(szDummy," %04d",measinfo->nAltitude);
  strcat(szBuffer,szDummy);
  sprintf(szDummy," %06.2f %06.2f 00",measinfo->dLongitude,measinfo->dLatitude);
  strcat(szBuffer,szDummy);
  fill_up_line (szBuffer,78);
  strcat (szBuffer, "\r\n");
  if(fputs(szBuffer,fp) <= 0) {
    if((nStatus=file_error(fp))!=0)
      return(nStatus);
  }
  /* prepare the third nasa header line */
  strcpy(szBuffer,"");
  strcpy(szDummy,"");
  /* the meaning of the 2 zeros is unclear */
  for(i = 0;i < measinfo->nChannelNumber; i++) {
    if(channel[i].nActive!=0) {
      switch(channel[i].nLaserType) {
        case 1: /* NdYAG*/
          if (lNdYAGShot < channel[i].nShotNumber) {
            lNdYAGShot=channel[i].nShotNumber;
            nNdYAGFreq= (int) channel[i].dRepRate;
        }
        break;
        case 3: /* Tisa*/
          if(lTisaShot<channel[i].nShotNumber) {
            lTisaShot=channel[i].nShotNumber;
          }
          nTisaFreq= (int) channel[i].dRepRate;
          break;
      }
    }
  }
  sprintf(szBuffer," %07ld %03d %07ld %03d %02d",lNdYAGShot, 
  nNdYAGFreq, lTisaShot, nTisaFreq , measinfo->nChannelNumber);
  fill_up_line (szBuffer,78);
  strcat (szBuffer, "\r\n");
  if (fputs(szBuffer,fp) <= 0) {
    if((nStatus=file_error(fp))!=0)
      return(nStatus);
  }
  /* channel special lines */
  for (i = 0;i < measinfo->nChannelNumber; i++) {
    strcpy(szBuffer,"");
    if(channel[i].nActive != 0) {
      if((fpTemp=fopen(channel[i].szTempfile,"r+b"))==NULL) {
        fprintf(stderr,"\n File %s not found",channel[i].szTempfile);
        channel[i].nActive=0;
        channel[i].nShotNumber=0;
      }
      else {
        channel[i].nActive=1;
        fclose(fpTemp);
      }
    }
    else {
      channel[i].nShotNumber=0;
    }
    sprintf(szBuffer, " %d", channel[i].nActive);
    if(channel[i].bPhoton) {
      strcat(szBuffer, " 1");
    }
    else {
     strcat(szBuffer, " 0");
    }
    switch(channel[i].nLaserType) {
      case 1: /*Nd YAG*/
        strcat(szBuffer," 1");
        break;
     case 3: /* TiSa*/
       strcat(szBuffer," 3");
       break;
     default:  /* unidentified Laser */
       break;
    }
    sprintf(szDummy," %05d",channel[i].nLength);
    strcat(szBuffer,szDummy);
    /* I don't know what is the meaning of the next number */
    strcat(szBuffer," 1");
    sprintf(szDummy," %04d", channel[i].nHighVoltage);
    strcat(szBuffer,szDummy);
    binwidth = 150.0/channel[i].nMHz; // 150m --- 1us
    for (k = 0; k < channel[i].nDataReduction; k++) {
      binwidth *= 2.0;
    }
    if (binwidth < 10) {
      if (binwidth < 5) {
        sprintf(szBuffer + strlen(szBuffer), " %4.2lf", binwidth);
      }
      else {
        sprintf(szBuffer + strlen(szBuffer), " %04.1lf", binwidth);
      }
    }
    else {
      sprintf(szBuffer + strlen(szBuffer), " %04.0lf", binwidth);
    }
    sprintf(szDummy,"% 05.0f",channel[i].dWavelength);
    strcat(szBuffer,szDummy);
    switch(channel[i].nPolarisation) {
      case 0: strcat(szBuffer,".o");
      break;
      case 1: strcat(szBuffer,".p");
      break;
      case 2: strcat(szBuffer,".s");
      break;
      default: strcat(szBuffer,".o");
      break;
    }
    strcat(szBuffer," 0 0 00 000");
    if (!channel[i].bPhoton) {
      k = channel[i].nDataReduction + channel[i].nADCBits;
      sprintf(szBuffer + strlen(szBuffer), " %d", k);
    }
    else {
      strcat(szBuffer," 00");
    }
    sprintf(szDummy," %05d",channel[i].nShotNumber);
    strcat(szBuffer,szDummy);
    if(channel[i].bPhoton) {
      /* assume the whole triggerange as 1.3V */
      sprintf(szDummy," %6.4f BC",channel[i].nDAC*0.025/63.0);
      strcat(szBuffer,szDummy);
    }
    else {
      switch(channel[i].nRange) {
        case 0: strcat(szBuffer," 0.5000 BT");
        break;
        case 1: strcat(szBuffer," 0.1000 BT");
        break;
        case 2: strcat(szBuffer," 0.0200 BT");
        break;
        default:
          LERMess(-1, "unidentified Range");
          strcat(szBuffer," 0.0000 XXX");
        break;
      }
    }
    sprintf(szBuffer + strlen(szBuffer),"%1X",channel[i].nTransientRecorder);
    fill_up_line (szBuffer,78);
    strcat (szBuffer, "\r\n");
    if (fputs(szBuffer,fp)<=0) {
      if ((nStatus = file_error(fp))!= 0)
        return(nStatus);
    }
  }
  fprintf(fp,"\r\n");
  for(i = 0; i < measinfo->nChannelNumber; i++) {
    if(channel[i].nActive!=0) {
      rawBins = 1 << channel[i].nDataReduction;
      rawBins *= channel[i].nLength;
      if((fpTemp=fopen(channel[i].szTempfile,"r+b"))==NULL) {
        fprintf(stderr,"\n File %s vanished",channel[i].szTempfile);
        exit(0);
      }
      if (fread((void *)channel[i].plAddress,sizeof(long int), rawBins, fpTemp) !=
          rawBins) {
        fprintf(stderr,"\n File %s no succes in fread",
        channel[i].szTempfile);
        exit(0);
      }
      if(fclose(fpTemp)!=0) {
        nStatus=file_error(fpTemp);
      }
      if((nStatus = remove(channel[i].szTempfile)) != 0) {
        fflush(NULL);
        perror("");
      }
      if(channel[i].nDataReduction != 0) {
        binsCombine = 1 << channel[i].nDataReduction;
        for(k=0; k < channel[i].nLength; k += binsCombine) {
          acc = 0;
          for (j = 0; j < binsCombine; j++ ){
            acc += channel[i].plAddress[k * binsCombine + j];
          }
          channel[i].plAddress[k] = acc;
        }
      }
      fwrite((void*) channel[i].plAddress,sizeof(long int),channel[i].nLength,fp);
      fprintf(fp,"\r\n");
    }
  }/* for all channels */
  if(fclose(fp) != 0){
    nStatus=file_error(fp);
  }
  printf("\nFinal output to %s\r\n",filename);
  return(nStatus);
}


int config_channel(SOCKET sd, Channel *ch1)
{
 int nStatus;
 assert( ch1 != 0);
 Licel_TCPIP_SelectTR(sd, ch1->nTransientRecorder);
 if((nStatus=Licel_TCPIP_StopAcquisition(sd)) < 0) {
     LERMess( nStatus, "Licel_TCPIP_Stop in config_channel");
     return(nStatus);
 }
 mDelay( 10);
 printf("\nChannel for %g nm ", ch1->dWavelength);
 if(!ch1->bPhoton) {
   printf(" analog regime  %dMHz", ch1->nMHz);
   if(( nStatus = Licel_TCPIP_SetInputRange(sd, ch1->nRange)) < 0) {
     LERMess(nStatus,"Licel_TCPIP_SetInputRange in config_channel");
     return(nStatus);
   }
   else {
     switch(ch1->nRange) {
       case 0: printf(" 500mV");
         break;
       case 1: printf(" 100mV");
         break;
       case 2: printf("  20mV");
         break;
       default:
               printf(" Arrrgh! unknown range");
         break;
     }
   }
   mDelay( 10);
 }
 else {
   printf(" photon counting ");
   if(( nStatus = Licel_TCPIP_SetDiscriminatorLevel(sd, ch1->nDAC)) < 0) {
               LERMess( nStatus, "SetDiscret in config_channel");
               return( nStatus);
   }
   else {
     printf(" Discr. level %4.2fV", 0.25f/63.0f*ch1->nDAC);
   }
 }
 switch(ch1->nPolarisation) {
   case 0:  printf("\nunpolarized          ");
     break;
   case 1:  printf("\nparallel polarisation ");
     break;
   case 2:  printf("\ncross polarisation    ");
     break;
   default: printf("\nArrgh! unknown polarization");
     break;
 }
 if(ch1->nMemory==0) {
   printf(" in memory A ");
 }
 else {
   printf(" in memory B ");
 }
 printf("configured.");
 return(nStatus);
}
