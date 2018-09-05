#include "stdafx.h"
#include "licel_util.h"
#include <assert.h>
#include <string.h>

/* Input Ranges */

#define MILLIVOLT500	0
#define MILLIVOLT100	1
#define MILLIVOLT20		2


#include <stdio.h>

/* Converts the LSW and the MSW values into an integer array containing the
summed up analog values. The first trash element (due to the data transmission
scheme) are also removed. */

void  Licel_Combine_Analog_Datasets(unsigned short* iLsw, unsigned short* iMsw,
                                    int iNumber, unsigned long *lAccumulated,
                                    short *iClipping)
{
    int i;
    for(i = 1; i < iNumber; i++)
    {
        lAccumulated[i-1] = (unsigned long) iLsw[i] +
                            (((unsigned long) iMsw[i] & 0xFF) << 16);
        iClipping[i-1]    = (short) ((iMsw[i] & 0x100) >> 8);
    }
}

// convert 16bit data

void  Licel_Combine_Analog_Datasets_16_bit(unsigned short* iLsw,
                                           unsigned short* iMsw,
                                           unsigned short* iPhm,
                                           int             iNumber,
                                           unsigned long   *lAccumulated,
                                           short           *iClipping)
{
    int i;
    for (i = 1; i < iNumber; i++)
    {
        lAccumulated[i-1]  = (unsigned long) iLsw[i] +
                            ((unsigned long) (iMsw[i]&0x0FFF) << 16) +
                            ((unsigned long) (iMsw[i]&0xE000) << 15) +
                            (iPhm != NULL ?
                                  ((unsigned long)(iPhm[i]&0x0100) << 23) : 0);
        iClipping[i-1]=(short) ((iMsw[i] & 0x1000) >> 12);
    }
}


/* Converts the  raw Photon counting data into an integer array containing the
summed up photon counting values. The first trash element (due to the data
transmission scheme) are also removed. The clipping information present in the
most significant bit is masked out if neccessary*/

void Licel_Convert_Photoncounting(unsigned short* photon_raw, int iNumber,
                                  unsigned long * photon_c, int iPurePhoton)
{
    int i, iMask;
    // XXX support for PRTypes needs to be added here
    if (iPurePhoton) {
      iMask = 0xFFFF;
    }
    else {
      iMask = 0x7FFF;
    }
    for (i = 1;i < iNumber; i++) {
       photon_c[i-1] = photon_raw[i] & iMask;
    }
}

/* Equivalent functionality as the above function , except it is called when the
acquired shot numbers is greater that 4096 or if the sampling rate is below
15 MHz */

void Licel_Convert_Photoncounting_FullWord(unsigned short* iLsw,
                                           unsigned short* iPhm,
                                           int             iNumber,
                                           unsigned long*  photon_c)
{
    int i;
    for(i = 1; i < iNumber; i++)
    {
        photon_c[i-1] = (unsigned long) iLsw[i] +
                         (((unsigned long) iPhm[i] & 0xFF) << 16);
    }
}

/* Normalizes the accumulated Data with respect to the number of cycles*/

void Licel_Normalize_Data(unsigned long *lAccumulated,
                          int iNumber,
                          int iCycles,
                          double *dNormalized)
{
   int i;
   if (iCycles == 0)
     iCycles = 1;
   for (i = 0;i < iNumber; i++) {
     dNormalized[i]=lAccumulated[i]/(double) iCycles;
   }
}

/* Scales the normalized data with respect to the input range */
void Licel_Scale_Analog_Data(double *dNormalized, int iNumber, int iRange,
                             int ADCBits, double *dmVData)
{
    int i;
    double dScale;
    switch(iRange)
    {
        case MILLIVOLT500:
                           dScale=500.0/(double) (1 << ADCBits);
                        break;
        case MILLIVOLT100:
                           dScale=100.0/(double) (1 << ADCBits);
                        break;
        case MILLIVOLT20:  dScale=20.0/(double) (1 << ADCBits);
                        break;
        default:			dScale=1.0;
                        break;
    }
    for(i = 0; i < iNumber; i++)
    {
        dmVData[i]=dScale * dNormalized[i];
    }
}


/* ERRMess  output of the error condtions to stderr */

void ERRMess(int nNumber, const char *sPlace)
{
  assert(sPlace);
  fprintf( stderr,"\n Error number %d has occured", nNumber);
  fprintf( stderr,"\n In Current Action \t%s\n", sPlace);
}

void getSecureMode(char* iniFilePath, int* mode, char* connectionPasswd)
{
  FILE *fp;
  char buffer[255];
  
  *mode = 0;
  strcpy(connectionPasswd,"");

  fp = fopen (iniFilePath,"rt");
  if (!fp) {
    return;
  }
  else {
    fgets(buffer, 250, fp);
    fgets(buffer, 250, fp);
    if (!strncmp(buffer,"UseSecureMode=TRUE", 18)){
      
      fgets(buffer, 250,fp);
      if (strstr(buffer, "SecureModePWD=")) {
        *mode = 1;
        strncpy(connectionPasswd,buffer+ 14, strlen(buffer)-15);
        connectionPasswd[strlen(buffer)-15]=0;
      }
    }
  }
}

void storeSecureMode(char* iniFilePath, int mode, char* connectionPasswd)
{
  FILE *fp;
  char buffer[255];
  
  
  fp = fopen (iniFilePath,"wt");
  if (!fp) {
    return;
  }
  strcpy(buffer, "SecureModePWD=");
  strcat(buffer, connectionPasswd);
  strcat(buffer,"\n");
  fprintf(fp, "[SecureMode]\n");
  if (mode) 
    fprintf(fp, "UseSecureMode=TRUE\n");
  else
    fprintf(fp, "UseSecureMode=FALSE\n");
  fprintf(fp, buffer);
  fclose(fp);
}
