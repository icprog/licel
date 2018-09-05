#include "stdafx.h"
#include "licel_tr_tcpip.h"





int Licel_TCPIP_SetIPParameter(SOCKET s, char* newHost, char* mask, int  newPort,char* gateway, char*  passwd)
{
  char command[1000];
  sprintf(command,"TCPIP \"%s\" \"%s\" \"%s\" \"%d\" \"%s\"\r\n", newHost, mask, gateway, newPort, passwd);  
  writeCommand(s, command);
  int rv = readResponse(s, command, 100, 1000);
  if (rv <0)
    return rv;
  if (!strstr(command,"executed")) {
    printf("%s\n",command);
    return -5803;
  }
  return rv;
}

int Licel_TCPIP_ActivateDHCP(SOCKET s, int nPort, const char* passwd)
{
  if (!passwd || strlen(passwd) > 900) {
    return -1;
  }
  char command[1000];
  sprintf(command,"TCPIP \"DHCP\" \"%d\" \"%s\"\r\n", nPort, passwd);
  writeCommand(s, command);
  int rv = readResponse(s, command, 100, 1000);
  if (rv <0)
    return rv;
  if (strcmp(command,"DHCP activated"))
    return -5803;
  return rv;
}

int Licel_TCPIP_GetID(SOCKET s, char* buffer, int bufferLength)
{
  if (!buffer || bufferLength <= 0) {
    return -1;
  }
  writeCommand(s,"*IDN?\r\n");
  return readResponse(s, buffer, bufferLength, 1000);
}

int Licel_TCPIP_GetCapabilities(SOCKET s, char* cap, int bufferLength){
  if (!cap || bufferLength <= 0) {
    return -1;
  }
  writeCommand(s,"CAP?\r\n");
  return readResponse(s, cap, bufferLength, 1000);
}

/********************* Transient *****************************/

int Licel_TCPIP_WaitForReady(SOCKET s, int Delay)
{
#ifdef WIN32
  DWORD start = GetTickCount();
#else
  struct timeb t1, t2;
  double duration;
  ftime(&t1);
#endif
  do {
    long int shotNumber;
    int memory;
    int acquisitionState, recording;
    int rv = Licel_TCPIP_GetStatus(s, &shotNumber, &memory, &acquisitionState, &recording);
    if (rv < 0)
      return rv;
    if (!acquisitionState)
      return rv;
#ifdef WIN32
  } while (start + Delay > GetTickCount());
#else
  ftime(&t2);
  duration = (1000.0*(t2.time-t1.time)+t2.millitm-t1.millitm);  
} while (duration < Delay); // + (start.millitm - now.millitm));  
#endif
  return -5700; 
}

int Licel_TCPIP_ClearMemory(SOCKET s)
{
  writeCommand(s, "CLEAR\r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"CLEAR Memory executed")) {
    printf("\r\nLicel_TCPIP_ClearMemory - Error 5092 :%s", response);
    return -5092;
  }
  return rv;
}

int Licel_TCPIP_ContinueAcquisition(SOCKET s)
{
  writeCommand(s, "CONTINUE\r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"CONTINUE executed")) {
    printf("\r\nLicel_TCPIP_ContinueAcquisition - Error 5093 :%s", response);
    return -5093;
  }
  return rv;
}

int Licel_TCPIP_MultipleClearMemory(SOCKET s)
{
  writeCommand(s, "MCLEAR\r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"MCLEAR executed")) {
    printf("\r\nLicel_TCPIP_MultipleClearMemory - Error 5080 :%s", response);
    return -5080;
  }
  return rv;
}

int Licel_TCPIP_MultipleContinueAcquisition(SOCKET s)
{
  writeCommand(s, "MCONTINUE\r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"MCONTINUE executed")) {
    printf("\r\nLicel_TCPIP_MultipleContinueAcquisition - Error 5080 :%s", response);
    return -5080;
  }
  return rv;
}

int Licel_TCPIP_MultipleStartAcquisition(SOCKET s)
{
  writeCommand(s, "MSTART\r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"MSTART executed")) {
    printf("\r\nLicel_TCPIP_MultipleStartAcquisition - Error 5086 :%s", response);
    return -5086;
  }
  return rv;
}

int Licel_TCPIP_MultipleStopAcquisition(SOCKET s)
{
  writeCommand(s, "MSTOP\r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"MSTOP executed")) {
    printf("\r\nLicel_TCPIP_MultipleStopAcquisition - Error 5082 :%s", response);
    return -5082;
  }
  return rv;
}

int Licel_TCPIP_MultipleWaitForReady(SOCKET s, int milliSecond)
{
  char command[1000];
  sprintf(command,"MWAIT %d\r\n", milliSecond);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
   if (strcmp(command,"MWAIT executed")) {
     printf("\r\nLicel_TCPIP_MultipleWaitForReady - Error 5083 :%s", command);
     return -5083;
  }
  return rv; 
}

int Licel_TCPIP_SelectTR(SOCKET s, int TR)
{
  char command[1000];
  sprintf(command,"SELECT %d\r\n", TR);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
   if (!strstr(command,"executed")) {
     printf("\r\nLicel_TCPIP_SelectTR - Error 5083 :%s", command);
     return -5083;
  }
  return rv; 
}

int Licel_TCPIP_SelectMultipleTR(SOCKET s, int* TRList, int trNumber)
{
  if (!TRList || trNumber > 20 || trNumber < 1)
    return -1;
  int i, offset;
  char command[1000];
  sprintf(command,"SELECT ");
  for (i = 0; i < trNumber; i++) {
    offset = strlen(command);
    sprintf(command + offset,"%d, ", TRList[i]);
  }
  offset = strlen(command);
  sprintf(command + offset - 2,"\r\n");
  writeCommand(s, command);
  int rv = readResponse(s, command, 500, 1000);
  if (rv < 0)
    return rv;
  if (!strstr(command,"executed")) {
    printf("\r\nLicel_TCPIP_SelectMultipleDevice - Error 5081 :%s", command);
    return -5081;
  }
  return rv;
}

int Licel_TCPIP_SetDiscriminatorLevel(SOCKET s, int discriminatorLevel)
{
  char command[1000];
  sprintf(command,"DISC %d\r\n", discriminatorLevel);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (!strstr(command,"set to")) {
     printf("\r\nLicel_TCPIP_SetDiscriminatorLevel - Error 5096 :%s", command);
     return -5096;
  }
  return rv; 
}

int Licel_TCPIP_SetInputRange(SOCKET s, int inputRange)
{
  char command[1000];
  sprintf(command,"RANGE  %d\r\n", inputRange);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
   if (!strstr(command,"set to")) {
     printf("\r\nLicel_TCPIP_SetInputRange - Error 5097 :%s", command);
     return -5097;
  }
  return rv; 
}

int Licel_TCPIP_SetThresholdMode(SOCKET s, int thresholdMode)
{
  char command[1000];
  sprintf(command,"THRESHOLD %d\r\n", thresholdMode);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (!strstr(command,"Damping")) {
    printf("\r\nLicel_TCPIP_SetThresholdMode - Error 5098 :%s", command);
    return -5098;
  }
  return rv; 
}

int Licel_TCPIP_SingleShot(SOCKET s)
{
  writeCommand(s, "SINGLE \r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"SINGLE executed")) {
    printf("\r\nLicel_TCPIP_SingleShot - Error 5091 :%s", response);
    return -5091;
  }
  return rv;
}

int Licel_TCPIP_StartAcquisition(SOCKET s)
{
  writeCommand(s, "START \r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"START executed")) {
    printf("\r\nLicel_TCPIP_SingleShot - Error 5095 :%s", response);
    return -5095;
  }
  return rv;
}

int Licel_TCPIP_IncreaseShots(SOCKET s, int shots )
{
  char command[1000];
  sprintf(command, "INCREASESHOT %d\r\n", shots);
  writeCommand(s, command);
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"INCREASESHOT executed")) {
    printf("\r\nLicel_TCPIP_IncreaseShots - Error 5099 :%s", response);
    return -5095;
  }
  return rv;
}
int Licel_TCPIP_SetShotLimit(SOCKET s, int mode)
{
  char command[1000];
  char response[1000];
  if (mode) {
    sprintf(command, "LIMIT 64K\r\n");
  }
  else {
    sprintf(command, "LIMIT 4K\r\n");
  }
  writeCommand(s, command);
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"LIMIT executed")) {
    printf("\r\nLicel_TCPIP_SetShotLimit - Error 5100 :%s", response);
    return -5100;
  }
  return rv;
}
int Licel_TCPIP_GetStatus(SOCKET s, long int* shotNumber, int* memory, int* acquisitionState, int* recording)
{
  writeCommand(s, "STAT?\r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strstr(response,"Shots")) {
    *memory = 0;
    *acquisitionState = false;
    * recording = false;
    sscanf(response, "Shots %ld", shotNumber);
    if (strstr(response,"Armed"))
      *acquisitionState = true;
    if (strstr(response,"Armed"))
      *recording = true;
    if (strstr(response,"MemB"))
      *memory = 1;
    return rv;
  }
  else {
    printf("\r\nLicel_TCPIP_GetStatus - Error 5765 :%s", response);
    return -5765;
  }
}

/***
   * Important!! Note : This function will reset the shot counter.
   ***/
int Licel_TCPIP_GetTRTYPE(SOCKET s, unsigned long int* trfifoLength,
                          int* trPCbits, int* trADCBits,
                          float* trSamplingRate)
{
    writeCommand(s,"TRTYPE?\r\n");
    char response[1000];
    int rv = readResponse(s, response, 200, 1000);
    if (rv < 0)
        return rv;
    if (strstr(response,"TRTYPE ADC Bits")) {
        sscanf(response,
           "TRTYPE ADC Bits %d PC Bits %d FIFOLength %lu binwidth %f",
               trADCBits, trPCbits, trfifoLength, trSamplingRate);
    }
    else {
      /* THESE ARE THE DEFAULT VALUES SET IF THE "TRTYPE?" COMMAND IS NOT SUPPORTED! */
      *trfifoLength = 16384;
      *trPCbits = 4;
      *trADCBits = 12;
      *trSamplingRate = 20.0f;
    }
            
    return rv;
}

int Licel_TCPIP_ReadData(SOCKET s, int numberToRead, unsigned char* data){
  return ReadArray(s, data, 2* numberToRead, 1000);
}

int Licel_TCPIP_GetDatasets(SOCKET s, int device,  int dataSet, int numberToRead, int Memory, unsigned char* data)
{
  char command[1000];
  sprintf(command,"DATA? %d %d", device, numberToRead);
  switch (dataSet) {
  case 0: 
    strcat(command, " PHO");
    break;
  case 1: 
    strcat(command, " LSW");
    break;
  case 2:
    strcat(command, " MSW");
    break;
  case 3:
    strcat(command, " PHM");
    break;
  }
  if (!Memory)
    strcat (command, " A");
  else
    strcat (command, " B");
  strcat(command,"\r\n");
  int rv = writeCommand(s, command);
  if (rv < 0)
    return rv;
  return Licel_TCPIP_ReadData(s, numberToRead, data);
}


int Licel_TCPIP_SetSlaveMode(SOCKET s)
{
  writeCommand(s, "SLAVE \r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"SLAVE executed")) {
    printf("\r\nLicel_TCPIP_SetSlaveMode - Error 5085 :%s", response);
    return -5085;
  }
  return rv;
}

int Licel_TCPIP_SetPushMode(SOCKET s, int shots,int dataset,int numberToRead,int Memory)
{
  char command[1000];
  sprintf(command,"PUSH %d %d", shots, numberToRead);
  switch (dataset) {
  case 0: 
    strcat(command, " PHO");
    break;
  case 1: 
    strcat(command, " LSW");
    break;
  case 2:
    strcat(command, " MSW");
    break;
  }
  if (!Memory)
    strcat (command, " A");
  else
    strcat (command, " B");
  strcat(command,"\r\n");
  int rv = writeCommand(s, command);
  if (rv < 0)
    return rv;
  char response[1000];
  rv = readResponse(s, response, 200, 1000);
  if (rv < 0) 
    return rv;
  if (strcmp(response,"PUSH executed")) {
    printf("\r\nLicel_TCPIP_SetPushMode - Error 5096 :%s", response);
    return -5096;
  }
  return rv;
}

int Licel_TCPIP_StopAcquisition(SOCKET s)
{
  writeCommand(s, "STOP \r\n");
  char response[1000];
  int rv = readResponse(s, response, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(response,"STOP executed")) {
    printf("\r\nLicel_TCPIP_StopAcquisition - Error 5094 :%s", response);
    return -5094;
  }
  return rv;
}

/***************************** APD ***********************/

int Licel_TCPIP_APDGetStatus(SOCKET s, int APD, bool* ThermoCooler, bool* TempInRange, 
                             float* HV, bool* HVControl)
{
  if( !ThermoCooler || !TempInRange || !HV || !HVControl)
    return -1;
  *ThermoCooler = false;
  *TempInRange  = false;
  *HVControl    = false;
  char command[1000];
  sprintf(command,"APD? %d\r\n", APD);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  sscanf(command,"APD %f", HV);
  if (strstr(command,"HV_remote"))
    *HVControl = true;
  if (strstr(command,"T_on"))
    *ThermoCooler = true;
  if (strstr(command,"T_in_range"))
    *TempInRange = true;
  return rv;
}

int Licel_TCPIP_APDSetCoolingState(SOCKET s, int APD, bool ThermoCooler)
{
  char command[1000];
  if (ThermoCooler) {
    sprintf(command,"APDT %d on\r\n", APD);
  }
  else {
    sprintf(command,"APDT %d off\r\n", APD);
  }
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"APDT executed"))
    return -5801;
  return rv; 
}

int Licel_TCPIP_APDSetGain(SOCKET s, int APD, float HV)
{
  char command[1000];
  sprintf(command,"APDG %d %f\r\n", APD, HV);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"APDG executed"))
    return -5801;
  return rv; 
}


/***************************** PMT ***********************/

int Licel_TCPIP_PMTSetGain(SOCKET s, int PMT, int HV)
{
  char command[1000];
  sprintf(command,"PMTG %d %d\r\n", PMT, HV);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"PMTG executed"))
    return -5801;
  return rv; 
}

int Licel_TCPIP_PMTGetStatus(SOCKET s, int PMT, bool* on, float* HV, bool* remote)
{
  if( !on || !HV || !remote)
    return -1;
  *on = false;
  *remote  = false;
  char command[1000];
  sprintf(command,"PMT? %d\r\n", PMT);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  *HV = 0.0f;
  sscanf(command,"PMT %f", HV);
  if (strstr(command,"remote"))
    *remote = true;
  if (strstr(command,"on"))
    *on = true;
  return rv;
}


/***************************** TIMING *************************/

// old functions with board id = 1
int Licel_TCPIP_SetTriggerMode(SOCKET s, bool LaserActive, bool PreTriggerActive, bool QSwitchActive, bool GatingActive, bool MasterTrigger)
{
  return Licel_TCPIP_SetTriggerMode(s, 1, LaserActive, PreTriggerActive, QSwitchActive, GatingActive, MasterTrigger);
}

int Licel_TCPIP_SetTriggerTiming(SOCKET s, long repetitionRate , long Pretrigger , long PretriggerLength, long QSwitch , long QswitchLength)
{
    return Licel_TCPIP_SetTriggerTiming(s, 1, repetitionRate , Pretrigger , PretriggerLength, QSwitch , QswitchLength);
}

// new API
int Licel_TCPIP_SetTriggerTiming(SOCKET s, int boardID, long repetitionRate , long Pretrigger , long PretriggerLength, long QSwitch , long QswitchLength)
{
  char command[1000];
  if (boardID) {
    sprintf(command,"TRIGGERTIME%d %ld %ld %ld %ld %ld\r\n",boardID, repetitionRate , Pretrigger , PretriggerLength, QSwitch , QswitchLength);
  }
  else {
    sprintf(command,"TRIGGERTIME %ld %ld %ld %ld %ld\r\n", repetitionRate , Pretrigger , PretriggerLength, QSwitch , QswitchLength);
  }
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"TRIGGERTIME executed"))
    return -5090;
  return rv; 
}

int Licel_TCPIP_SetTriggerMode(SOCKET s, int boardID, bool Laser, bool PreTrigger, bool QSwitch, bool Gating, bool MasterTrigger)
{
  int mode = 0;
  if (Laser)
    mode += 1;
  if (PreTrigger)
    mode += 2;
  if (QSwitch)
    mode += 4;
  if (Gating)
    mode += 8;
  if (MasterTrigger)
    mode += 16;
  char command[1000];
  if (boardID) {
    sprintf(command,"TRIGGERMODE%d %d\r\n",boardID, mode);
  }
  else {
    sprintf(command,"TRIGGERMODE %d\r\n", mode);
  }
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"TRIGGERMODE executed"))
    return -5089;
  return rv; 
}


/************************ SECURITY ***************************/
int Licel_TCPIP_SetAccessLimited(SOCKET s,char* connectionPasswd, char* passwd) 
{
  char command[1000];
  sprintf(command,"ACCESS LIMIT \"%s\" \"%s\"\r\n",passwd ,connectionPasswd);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"ACCESS Limited"))
    return -5090;
  return rv; 
}
int Licel_TCPIP_SetAccessUnLimited(SOCKET s,char* passwd)
{
  char command[1000];
  sprintf(command,"ACCESS FREE \"%s\"\r\n",passwd);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
   if (strcmp(command,"ACCESS Unlimited"))
    return -5090;
  return rv; 
}
int Licel_TCPIP_SetWhiteList(SOCKET s,char* whiteHost1,char*  whiteHost2,char*  whiteHost3, char* passwd)
{
  char command[1000];
  sprintf(command,"WHITELIST \"%s\" \"%s\" \"%s\" \"%s\"\r\n",passwd, whiteHost1, whiteHost2, whiteHost3);
  writeCommand(s, command);
  printf("\r\nsent string :%s", command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"WHITELIST executed")) {
    printf("\r\nLicel_TCPIP_SetWhiteList - Error 5890 :%s", command);
    return -5890;
  }
  return rv; 
}

/* Power Meter*/

int Licel_TCPIP_PowerSelectChannel(SOCKET s, int Channel)
{
  char command[1000];
  sprintf(command,"POW CHANNEL %d\r\n", Channel);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"POW CHANNEL executed"))
    return -5801;
  return rv; 
}

int Licel_TCPIP_PowerStart(SOCKET s)
{
  char command[1000];
  sprintf(command,"POW START\r\n");
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
   if (strcmp(command,"POW START executed"))
    return -5801;
  return rv; 
}

int Licel_TCPIP_PowerTrace(SOCKET s, int *readings, int *numReadings)
{
   char command[1000];
  sprintf(command,"POW TRACE\r\n");
  writeCommand(s, command);
  int rv = readResponse(s, command, 990, 1000); 
  if (rv < 0)
    return rv;
  char *token;

  token = strtok(command, " ");
  if (token) {
    sscanf(token," %d", numReadings);
  }
  else {
      return -1;
  }
  // Get next token: 
  token = strtok( NULL, " "); 
  int i = 0;
  while (token && i < *numReadings) {
    sscanf(token," %d", &readings[i]);
    i++;
    token = strtok( NULL, " "); 
  }
  if (i == *numReadings) {
      return rv;
  }
  else {
      return -1;
  }
}


int Licel_TCPIP_PowerGetData(SOCKET s, int *milliSeconds, double *reading)
{
  char command[1000];
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  sscanf(command,"%d %lf", milliSeconds, reading);   
  return rv;
}

int Licel_TCPIP_PowerStop(SOCKET s)
{
  char command[1000];
  sprintf(command,"POW STOP\r\n");
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (!strcmp(command,"POW STOP executed"))
    return -5801;
  return rv; 
}


// Alignment sensor

int Licel_TCPIP_BoreSetRanges(SOCKET s, int backgroundStart, int backgroundStop,
                              int signalStart, int signalStop) {
  char command[1000];
  sprintf(command,"ALIGNTIME %d %d %d %d\r\n",backgroundStart, backgroundStop,
          signalStart, signalStop);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"ALIGNTIME executed"))
    return -5801;
  return rv; 
}

int Licel_TCPIP_BoreSign(SOCKET s)
{
  char command[1000];
  sprintf(command,"ALIGNSIGN\r\n");
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
   if (strcmp(command,"ALIGNSIGN executed"))
    return -5801;
  return rv; 
}


int Licel_TCPIP_BoreStart(SOCKET s, int shots, int cycles) {
  char command[1000];
  sprintf(command,"ALIGNDATA START EVERY %d SHOTS %d CYCLES\r\n", shots, cycles);
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"ALIGNDATA START executed"))
    return -5801;
  return rv; 
}


int Licel_TCPIP_BoreStop(SOCKET s)
{
  char command[1000];
  sprintf(command,"ALIGNDATA STOP\r\n");
  writeCommand(s, command);
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  if (strcmp(command,"ALIGNDATA STOP executed"))
    return -5801;
  return rv; 
}

int Licel_TCPIP_BoreGetData(SOCKET s, float *countrates, long int *counter)
{
  char command[1000];
  int rv = readResponse(s, command, 200, 1000);
  if (rv < 0)
    return rv;
  sscanf(command,"%ld %g %g %g %g %g %g %g %g", counter, &countrates[0], 
         &countrates[1], &countrates[2], &countrates[3], &countrates[4],
         &countrates[5], &countrates[6], &countrates[7]);   
 return rv;
}
