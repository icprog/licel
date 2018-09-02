/* Prototypes */

    /* Converts the LSW and the MSW values into an integer array containing the
       summed up analog values. The first trash element (due to the data
       transmission scheme) are also removed. */
void  Licel_Combine_Analog_Datasets(unsigned short* iLsw,
                                    unsigned short* iMsw,
                                    int iNumber,
                                    unsigned long *lAccumulated,
                                    short *iClipping);
    
 // convert 16bit data

void Licel_Combine_Analog_Datasets_16_bit(unsigned short* iLsw,
                                           unsigned short* iMsw,
                                           unsigned short* iPhm,
                                           int             iNumber,
                                           unsigned long* lAccumulated,
                                           short*         iClipping);

    /* Converts the  raw Photon counting data into an integer array containing
       the summed up photon counting values. The first trash element (due to the
       data transmission scheme ) are also removed. The clipping information
       present in the most significant bit is masked out if neccessary*/
void Licel_Convert_Photoncounting(unsigned short* photon_raw,
                                  int iNumber,
                                  unsigned long * photon_c,
                                  int iPurePhoton);

   /* Equivalent functionality as the above function , except it is called when
      the acquired shot numbers is greater that 4096 or if the sampling rate is
      below 15 MHz */

void Licel_Convert_Photoncounting_FullWord(unsigned short* iLsw,
                                           unsigned short* iPhm,
                                           int             iNumber,
                                           unsigned long*  photon_c);

   /* Normalizes the accumulated Data with respect to the number of cycles*/
void Licel_Normalize_Data(unsigned long *lAccumulated,
                          int iNumber,
                          int iCycles,
                          double *dNormalized);
    /* Scales the normalized data with respect to the input range */
void Licel_Scale_Analog_Data(double *dNormalized,
                             int iNumber,
                             int iRange,
                             int ADCBits,
                             double *dmVData);
 /* Error message dump */
void ERRMess(int nNumber, const char *sPlace);

void getSecureMode(char* iniFilePath, int* mode, char* connectionPasswd);

void storeSecureMode(char* iniFilePath, int mode, char* connectionPasswd);


