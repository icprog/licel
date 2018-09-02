#include "licel_tcpip.h"


/* Input Ranges */

#define MILLIVOLT500	0
#define MILLIVOLT100	1
#define MILLIVOLT20		2

/* Threshold Modes */

#define THRESHOLD_LOW	0
#define THRESHOLD_HIGH	1

/* Datasets*/

#define PHOTON          0
#define LSW	            1
#define MSW	            2
#define PHM             3

/* Memory */
#define MEM_A 0
#define MEM_B 1

#include <math.h>

/**
 * configure the controller for static IP configuration. Set the new IP
 * address, the basic port number, the subnet mask and the gateway
 * @param s       - connection
 * @param newHost - string The IP address that the  controller will be set to
 * @param mask    - string with the subnet mask that the controller should use
 *                  for TCPIP communication
 * @param newPort   - new Port after reboot of the controller one should
 *                 connect to this port
 * @param gateway - string with the gateway that should be used by the
 *                  controller for TCPIP communication
 * @param passwd  - string containing the current password for the controller
 * @return        - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetIPParameter(SOCKET s, char* newHost, char* mask,
                               int  newPort,char* gateway, char*  passwd);


/**
 * Activate the DHCP mode of the transient recorder controller.
 * In order to do so, the user must enter the proper password and port number
 * for the controller. After DHCP mode has been set, communication will be lost
 * until the acquisition computer is configured for DHCP communication as well.

 * @param s      - connection
 * @param nPort  - new Port after reboot of the controller one should
 *                 connect to this port
 * @param passwd - string containing the current password for the controller
 * @return       - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_ActivateDHCP(SOCKET s, int nPort, const char* passwd);

/**
 * Get the identification string from the controller
 * @param s            - connection
 * @param buffer       - string to hold the identification information
 * @param bufferLength - max capacity of the result string
 * @return             - number of bytes in the result string, -1 if read fails
 */
int Licel_TCPIP_GetID(SOCKET s, char* buffer, int bufferLength);

/**
 * Get the available subcomponents of the controller like:
 * TR  - for controlling transient recorder
 * APD - for APD remote control
 * PMT - for PMT remote control
 * TIMER - for the trigger timing controller
 * CLOUD - for transient recorder controller cloud mode
 * BORE  - Boresight alignment system
 * @param s            - connection
 * @param cap          - string with the available capabilities
 * @param bufferLength - max capacity of the result string
 * @return             - number of bytes in the result string, -1 if read fails
 */
int Licel_TCPIP_GetCapabilities(SOCKET s, char* cap, int bufferLength);


/***************** Transient recorder ************************/

/** 
 * Waits for return of the device from the armed state. If the waiting time
 * is longer than the time specified by delay than the device remains armed
 * and will be return to the idle state with next reading of binary data
 * @param s     - connection
 * @param delay - max. time to wait for returning to the idle state in ms
 * @return      - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_WaitForReady(SOCKET s, int Delay);

/**
 * Clear both memories (A and B) of the previously selected device.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_ClearMemory(SOCKET s);

/**
 * Continue the recording process for the previously specified device without 
 * reinitializing the memory.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure  
 */
int Licel_TCPIP_ContinueAcquisition(SOCKET s);

/**
 * Reading the raw data sets ( analog LSW, analog MSW or photon counting) from
 * the specified device.
 *
 * @param s            - connection
 * @param TR           - transient recorder
 * @param dataset      - which part of the raw information should be transfered
 *                       from the TR to the computer. Use the constants PHOTON
 *                       LSW MSW, !!! PRxx -xx recorder need to read LSW and MSW
 *                       instead of PHOTON
 * @param numberToRead - number of 16 bit wide data points
 * @param memory       - summation memory to be retrieved.
 * @param data         - byte array containing the data, use routines from
 *                       licel_ut to convert the byte array into physical
 *                       values
 * @return             - 0 if successful. Negative values indicate failure
 */
int  Licel_TCPIP_GetDatasets(SOCKET s, int device,  int dataSet,
                             int numberToRead, int Memory, unsigned char* data);

/**
 * Return the status information for the specified device (cycles,memory,
 * acquisition state and whether the device is just recording).
 * @param s                - connection
 * @param shotNumber       - number of shots already acquired. This
 *                           shotnumber has an offset of 2 as the two
 *                           initial clearing cycles advance the shotnumber
 *                           to 2
 * @param memory           - to which summation memory the last acquisition
 *                           was added.
 * @param acquisitionState - is FALSE when the transient returns from the
 *                           armed state, and is TRUE, when an aquisition is
 *                           running
 * @param recording        - TRUE during acquisition-time, e.g. the ADC or the
 *                           photon counting is aquiring data. Recording is
 *                           FALSE during summation and when the TR is waiting
 *                           for a new trigger.
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_GetStatus(SOCKET s, long int* shotNumber, int* memory,
                          int* acquisitionState, int* recording);

/**
  * Return the various Transient Recorder Parameter information for the
  * specified device. If this command is not supported default values 
  * for a TR20-160 are filled in
  * @param s                - Socket connection ID
  * @param trfifoLength     - This gives the set FIFO Length of the TR
  * @param trPCbits         - This corresponds to the number of Photon Counting
  *                           Bits of the Transient Recorder
  * @param trADCBits        - This corresponds to the number of ADC Bits of the
  *                           Transient Recorder ( 12 bits or 16 bits )
  * @param trSamplingRate   - This corresponds to the Sampling Rate of the
  *                           Transient Recorder. Currently not Set.
  *
  *
  * @return                 - number of bytes received. Negative values indicate failure                       
  */

int Licel_TCPIP_GetTRTYPE(SOCKET s, unsigned long int* trfifoLength,
                          int* trPCbits, int* trADCBits,
                          float* trSamplingRate);
/**
 * Clears both memories (A and B) of the currently selected devices.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_MultipleClearMemory(SOCKET s);

/**
 * The acquisition process of the selected multiple devices will be restarted
 * without clearing their memories.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_MultipleContinueAcquisition(SOCKET s);

/**
 * The acquisition process will be started after the next received trigger for
 * multiple devices
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_MultipleStartAcquisition(SOCKET s);

/**
 * The acquisition process will be stoped after the next received trigger for
 * multiple devices
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_MultipleStopAcquisition(SOCKET s);

/**
  * Wait until all devices returned from the armed state.
  * @param s - connection
  * @param delay - max. time to wait for returning to the idle state in ms
  * @return  - number of bytes received. Negative values indicate failure
  */
int Licel_TCPIP_MultipleWaitForReady(SOCKET s, int milliSecond);

/**
 * Wait until the the number of scans defined by Number to Read is available
 * and reads them or returns a timeout error if the timeout ms is exceeded.
 * Read binary data into a byte array. Transient recorder data is internally
 * 16bits wide so for every data point two bytes need to be fetched.
 * @param s            - connection
 * @param numberToRead - number of 16 bit wide data points
 * @param data         - byte array containing the data, use routines from
 *                       licel_ut to convert the byte array into physical
 *                       values
 * @return             - 0 if successful. negative values indicate
 *                       failure
 */
int Licel_TCPIP_ReadData(SOCKET s, int numberToRead, unsigned char* data);

/**
 * Selects the TR specified by the input TR. Selecting a transient recorder
 * makes it active for all future commands that do not have a required TR
 * number input. The previously selected TR become deselected when this
 * command is issued.
 * @param s  - connection
 * @param TR - transient recorder, all following single TR commands
 *             will access this transient recorder
 * @return   - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SelectTR(SOCKET s, int TR);

/**
  * The TR corresponding to the numbers in the device list array will be
  * selected which means that they will become sensitive to all future commands
  * that do not require a device number input. The devices will be deselected
  * if another select command is issued.
  * @param s        - connection
  * @param TRList   - transient recorder list, all following multipe TR
  *                   commands will access this transient recorder
  * @param trNumber - number of transient recorder in TRList
  * @return         - number of bytes received. Negative values indicate failure
  */
int Licel_TCPIP_SelectMultipleTR(SOCKET s, int* TRList, int trNumber);

/**
  * Set the discriminator level between 0 and 63 for the selected transient
  * recorders.
  * @param s                  - connection
  * @param discriminatorLevel - For a transient recorder with a photon
  *                             counting unit, the discriminator threshold
  *                             can be set in 64 steps between 0 and 63.
  * @return                   - number of bytes received. Negative values indicate failure
  */
int Licel_TCPIP_SetDiscriminatorLevel(SOCKET s, int discriminatorLevel);

/**
 * Change the input voltage range. 
 * @param s          - connection
 * @param inputRange - analog input range, use the predefined constants like
 *                     MILLIVOLT500 as arguments
 * @return           - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetInputRange(SOCKET s, int inputRange);

/**
 * Stop the push mode and sets the transient recorder controller back in to
 * the slave mode.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetSlaveMode(SOCKET s);

/**
 * Activate the push mode for the currently selected transient recorder
 * @param s            - connection
 * @param shots        - transfer data every #shots
 * @param dataset      - which part of the raw information should be transfered
 *                       from the TR to the computer. Use the constants PHOTON
 *                       LSW, !!! PRxx -xx recorder need to read LSW
 *                       instead of PHOTON
 * @param numberToRead - number of 16 bit wide data points
 * @param memory       - summation memory to be retrieved.
 * @return             - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetPushMode(SOCKET s, int shots,int dataset,int numberToRead,
                            int memory);

/** 
 * Set Threshold Mode sets the scale of the discriminator level. In the low 
 * threshold mode the disciminator level 63 corresponds to -25mV while in the
 * high threshold mode it corresponds to -100mV.
 * @param s             - connection
 * @param thresholdMode - The threshold mode defines the increment between
 *                        discriminator levels. In the low threshold mode
 *                        the disciminator level 63 corresponds to -25mV
 *                        while in the high threshold mode it corresponds
 *                        to -100mV
 * @return              - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetThresholdMode(SOCKET s, int thresholdMode);

/**
 * Acquire one shot with the currently selected device.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SingleShot(SOCKET s);

/**
 * Start the currently selected transient recorder.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_StartAcquisition(SOCKET s);

/**
 * Increase the shotnumber of the TR without adding data, this can be used to
 * make a fixed number of acquisitions based on the internal 4094 shot limit
 * @param s     - connection
 * @param shots - number of shots to advance
 * @return  - number of bytes received. Negative values indicate failure
 */

int Licel_TCPIP_IncreaseShots(SOCKET s, int shots );



int Licel_TCPIP_SetShotLimit(SOCKET s, int mode);

/**
 * Stop the acquisition process after the next received trigger.
 * @param s - connection
 * @return  - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_StopAcquisition(SOCKET s);

/****************** APD   **********************************/

/**
 * Get the status of the APD with the corresponding APD number.
 * @param s            - connection
 * @param APD          - The physical device number of the APD. 
 *                       Valid values are 0-3
 * @param ThermoCooler - is true is the thermocooler is on
 * @param TempInRange  - is true if the thermocooler is on and the detector
 *                       temperature is very close to the target temperature
 * @param HV           - the applied HV to the detector
 * @param HVControl    - is true if remote control is active
 * @return             - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_APDGetStatus(SOCKET s, int APD, bool* ThermoCooler,
                             bool* TempInRange, float* HV, bool* HVControl);
/**
 * Set the cooling state for the APD with the corresponding APD number.
 * @param s            - connection
 * @param APD          - The physical device number of the APD. Valid values
 *                       are 0-3
 * @param ThermoCooler - true turns thermocooler on, if it is false the
 *                       detector is only passively cooled
 * @return             - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_APDSetCoolingState(SOCKET s, int APD, bool ThermoCooler);

/**
 * Set the Gain Voltage for the APD
 * @param s   - connection
 * @param APD - The physical device number of the APD. Valid values are 0-3
 * @param HV  - the applied HV to the detector
 * @return    - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_APDSetGain(SOCKET s, int APD, float HV);

/****************** PMT **********************************/

/**
 * Get the status of the PMT with the corresponding PMT number.
 * @param s       - connection
 * @param PMT    - The physical device number of the PMT. Valid values are 0-7
 * @param HVOn   - is true is the high voltage is on
 * @param HV     - the applied HV to the detector
 * @param remote - is true if remote control is active
 * @return       - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_PMTGetStatus(SOCKET s, int PMT, bool* on, float* HV,
                             bool* remote);

/**
 * Set the gain voltage for the PMT specified by the PMT Number
 * @param s   - connection
 * @param PMT - The physical device number of the PMT. Valid values are 0-7
 * @param HV  - the applied HV to the detector
 * @return    - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_PMTSetGain(SOCKET s, int PMT, int HV);

/***************** TIMING *********************************/

/**
 * Enable/Disable the trigger in and outputs
 * @param s                - connection
 * @param boardID          - timing board valid values are 1,2,3
 * @param LaserActive      - if true a trigger for the laser lamp will be 
 *                           generated
 * @param PretriggerActive - if true a trigger for the transient recorder will
 *                           be generated
 * @param QSwitchActive    - if true a trigger for the laser Q-Switch will be
 *                           generated
 * @param GatingActive     - if true a gating pulse will be generated. The
 *                           gating pulse starts with the raising edge of
 *                           the pretrigger and ends
 *                           with the falling edge of the Q-Switch Pulse
 * @param MasterTrigger    - if true an external trigger will be accepted,
 *                           if false the internal trigger will be used. The
 *                           internal trigger will be controlled via the
 *                           repetitionRate
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetTriggerMode(SOCKET s, int boardID, bool LaserActive,
                               bool PretriggerActive, bool QSwitchActive,
                               bool GatingActive, bool MasterTrigger);

/**
 * Enable/Disable the trigger in and outputs
 * @param s                - connection
 * @param LaserActive      - if true a trigger for the laser lamp will be 
 *                           generated
 * @param PretriggerActive - if true a trigger for the transient recorder will
 *                           be generated
 * @param QSwitchActive    - if true a trigger for the laser Q-Switch will be
 *                           generated
 * @param GatingActive     - if true a gating pulse will be generated. The
 *                           gating pulse starts with the raising edge of
 *                           the pretrigger and ends
 *                           with the falling edge of the Q-Switch Pulse
 * @param MasterTrigger    - if true an external trigger will be accepted,
 *                           if false the internal trigger will be used. The
 *                           internal trigger will be controlled via the
 *                           repetitionRate
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetTriggerMode(SOCKET s, bool LaserActive,
                               bool PretriggerActive, bool QSwitchActive,
                               bool GatingActive, bool MasterTrigger);

/**
 * Set the delays at the delay generator
 * @param s                - connection
 * @param boardID          - timing board valid values are 1,2,3
 * @param repetitionRate   - in internal mode delay between two pulses in ns,
 *                           in external mode start delay between external 
 *                           trigger and lamp in ns
 * @param Pretrigger       - delay between internal or external trigger
 *                           and pretrigger in ns
 * @param PretriggerLength - length in  ns of the pretrigger pulse
 *
 * @param QSwitch          - delay between pretrigger start and Q-Switch
 *                           start in ns
 * @param QswitchLength    - length in  ns of the Q-Switch pulse
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetTriggerTiming(SOCKET s, int boardID, long repetitionRate,
                                 long Pretrigger, long PretriggerLength,
                                 long QSwitch, long QswitchLength);

/**
 * Set the delays at the delay generator
 * @param s                - connection
 * @param repetitionRate   - in internal mode delay between two pulses in ns.
 * @param Pretrigger       - delay between internal or external trigger
 *                           and pretrigger in ns
 * @param PretriggerLength - length in  ns of the pretrigger pulse
 *
 * @param QSwitch          - delay between pretrigger start and Q-Switch
 *                           start in ns
 * @param QswitchLength    - length in  ns of the Q-Switch pulse
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_SetTriggerTiming(SOCKET s, long repetitionRate, long Pretrigger,
                                 long PretriggerLength, long QSwitch, 
                                 long QswitchLength);


/************************ SECURITY ***************************/

/**
 * Activate the access limitation, that means only whitelisted hosts can access
 * the controller and need to verify them self by properly encoding with the
 * connectionPasswd a two 8 byte numbers.Make sure that you called
 * Licel_TCPIP_SetWhiteList before otherwise no host will be authorized to
 * access the controller.
 * @param s                - connection
 * @param connectionPasswd - the controller will use this password to verify
 *                           that the PC is authorized to open a connection
 * @param passwd           - string containing the current password for the
 *                           controller
 */
int Licel_TCPIP_SetAccessLimited(SOCKET s, char* connectionPasswd,
                                char* passwd);

/**
 * dectivate the access limitation, that means every  hosts can access the
 * controller
 * @param s       - connection
 * @param passwd  - string containing the current password for the controller
 */
int Licel_TCPIP_SetAccessUnLimited(SOCKET s, char* passwd);

/**
 * List hosts that be allowed to to access the controller after
 * Licel_TCPIP_SetAccessLimited has been called. One can list three different
 * hosts. Specifying a 255 as the last number activates the whole range,
 * e.g. 10.49.234.255 as host will make all hosts from 10.49.234.1 to
 * 10.49.234.254 whitelisted hosts
 * @param s          - connection
 * @param whiteHost1 - string for a host in IP presentation, can be a empty string
 * @param whiteHost2 - string for a host in IP presentation, can be a empty string
 * @param whiteHost3 - string for a host in IP presentation, can be a empty string
 * @param passwd     - string containing the current password for the controller
 */
int Licel_TCPIP_SetWhiteList(SOCKET s, char* whiteHost1, char*  whiteHost2,
                             char*  whiteHost3, char* passwd);

/************************** Power Meter ***********************************/

/**
 * Set the ADC input channel
 * @param s                - connection
 * @param Channel          - ADC Input
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_PowerSelectChannel(SOCKET s, int Channel);

/**
 * Start the power meter data acquisition
 * @param s                - connection 
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_PowerStart(SOCKET s);

/**
 * Start a single power meter data acquisition
 * @param s                - connection 
 * @param readings         - array with 250 trace points
 * @param numReadings      - number of valid data points
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_PowerTrace(SOCKET s, int *readings, int *numReadings);
/**
 * Stop the power meter data acquisition
 * @param s                - connection 
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_PowerStop(SOCKET s);

/**
 * get the data from the second socket the port number which is one above the
 * command socket
 * @param s                - connection 
 * @param milliSeconds     - milliseconds since start
 * @param reading         -  power meter reading
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_PowerGetData(SOCKET s, int *milliSeconds, double *reading);




/************************** Bore *******************************************/

/**
 * Define the signal regions, there is a background and a signal region, the
 * signal will be reduced by the background level.
 * @param s                - connection 
 * @param backgroundStart  - first background bin
 * @param backgroundStop   - last background bin
 * @param signalStart      - first signal bin
 * @param signalStop       - last signal bin
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_BoreSetRanges(SOCKET s, int backgroundStart, int backgroundStop,
                              int signalStart, int signalStop);

/**
 * toggle the sign of the counter transmitted with the counter, this can be used to
 * synchronize a drive movement with the bore data by sending the toggle command and
 * waiting for the counter in the bore data to change its sign.
 * @param s                - connection 
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_BoreSign(SOCKET s);

/**
 * start the bore aquisition
 * @param s                - connection 
 * @param shots            - number of shots per cycle
 * @param cycles           - number of cycles, -1 for infinite cycles
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_BoreStart(SOCKET s, int shots, int cycles);

/**
 * stop the bore aquisition
 * @param s                - connection 
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_BoreStop(SOCKET s);

/**
 * get the data from the second socket the port number is one above the command
 * socket
 * @param s                - connection 
 * @param countrates       - array with 8 count rates (4 signal and 4 background)
 * @param counter          - number of transmitted packages, the sign might be negative
 * @return                 - number of bytes received. Negative values indicate failure
 */
int Licel_TCPIP_BoreGetData(SOCKET s, float *countrates, long int *counter);
