#define TRUE 1
#define FALSE 0
#include <stdio.h>
#define MAX_NUMBER 65536
#ifndef __cplusplus
typedef unsigned char bool;
#endif

typedef struct{
  short       nTransientRecorder; /** transient recorder address */
  int         nMemory;  /** memory 0 for MemA 1 for MemB */
  bool        bPhoton;  /** 0 - for analog dataset , 1 - photon counting */ 
#ifdef __BORLANDC__
  unsigned long huge     *plAddress;
#else
  unsigned long *plAddress;  /** memory to hold the raw data */
#endif
  int         nDAC;     /** discriminator level  between 0 and 63 */
  int         nRange;   /** analog input range 0 - 500mV, 1 - 100mV */
  int         nPolarisation; /** none -0, parallel -1, perpendicular -2*/
  int         nDataReduction; /**data reduction factor (2^n), this for data */
                              /** reduction of 2 for instance 2^2 = 4 tr   */
                              /** recorder bins will be combined into a 
                              /** single data bin. */ 
  double      dWavelength; /** data set wavelength in nm */
  int         nShotNumber; /** number of shots acquired  internal use*/
  int         nMHz;  /** Transient recorder sampling rate*/
  int         nTRLength; /** number bins available in the  TR @see licel_data.cpp*/
  int         nTRType;  /** type of the transient recorder 0 TR, 1 PR @see licel_data.cpp*/
  int         nLength; /** number of bins to read*/
  char        szTempfile[80]; /** path of a temp file*/
  int         nHighVoltage; /** high voltage of the detector*/
  int         nActive;   /** data set should read and stored*/
  double      dRepRate;  /** laser repetition rate*/
  int         nLaserType; /** laser source identifier 0 or 1*/  
  int         nADCBits; /** number of ADC Bits 12 or 16 are supported */
  int         nPCBits; /** number of PC Bits between 4 and 8*/
} Channel;

typedef struct{
  char        szLocation[20]; /** measurement site*/
  int         nAltitude;  /** altitude above sea level [m]*/
  double      dLongitude; /** longitude in degrees */
  double      dLatitude;  /** lattitude in degrees */
  char        cFirstLetter; /** first letter of the data file*/ 
  char        szOutPath[80]; /** output directory for data */
  int         nChannelNumber; /** number of datasets into a single file*/
  int         nMaxShots;      /** max num of shots @see shot.cpp*/
}MeasureInfo;

/* File handling routines */
void LERMess(int,const char *);
void CheckFinal(FILE *,int);
void CheckTemp(FILE *, char *,int);
void fill_up_line(char *,int); 
int file_error(FILE *);
int write_temp_nasa(Channel *,MeasureInfo *,char *);
int read_value(FILE*, const char*,void*);
int parseTRType(FILE*, Channel*);
int read_configuration(FILE *,Channel **,MeasureInfo **);
void get_config_filename(char *,char *,const char*);
int config_channel(SOCKET sd, Channel*);
