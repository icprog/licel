// LicelAcq.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "LicelAcqCtl.h"

#include "licel_tcpip.h"
#include "licel_tr_tcpip.h"
#include "licel_util.h"

#include "afxmt.h"

//licel卡设备序号
#define Licel_Card_TR_Count 3
int Licel_Card_TR[Licel_Card_TR_Count] = { 0, 1 , 2};

typedef struct LicalAcqParams
{
	HWND m_hMsgWnd;
	unsigned int m_acccount;
	unsigned int m_curvepoint;
}LicalAcqParams,*PTR_LicalAcqParams;

double* Licel_analog_pBuf[Licel_Card_TR_Count];
double* Licel_photon_pBuf[Licel_Card_TR_Count];
double* Licel_pSwapBuf[Licel_Card_TR_Count * 2];
CCriticalSection gb_Licel_CriticalSection;
PTR_LicalAcqParams gb_Licel_pLicelAcqParams;
bool gb_Licel_bAcqCtl;

SOCKET Licel_ctrl_socket;

unsigned short *Licel_analog_MSW,*Licel_analog_LSW,*Licel_analog_PHM,*Licel_photon_LSW;
unsigned long *Licel_ananlog_acc;
unsigned long *Licel_photon_c;
short *Licel_clips;
double *Licel_normaleized;

UINT __cdecl LicelAcq_Loop(LPVOID lpThreadParam);

void GetAcq(int cycles, int num_point, int memory,double* photon[], double*anolog[])
{
	for(int i=0;i<Licel_Card_TR_Count;i++)
	{
		//模拟信号
		Licel_TCPIP_GetDatasets(Licel_ctrl_socket, Licel_Card_TR[i], LSW, num_point+1, memory, (unsigned char*)Licel_analog_LSW);
		Licel_TCPIP_GetDatasets(Licel_ctrl_socket, Licel_Card_TR[i], MSW, num_point+1, memory, (unsigned char*)Licel_analog_MSW);
		Licel_TCPIP_GetDatasets(Licel_ctrl_socket, Licel_Card_TR[i], PHM, num_point+1, memory, (unsigned char*)Licel_analog_PHM);

		Licel_Combine_Analog_Datasets_16_bit(Licel_analog_LSW, Licel_analog_MSW, Licel_analog_PHM,num_point+1, Licel_ananlog_acc, Licel_clips);
		Licel_Normalize_Data(Licel_ananlog_acc, num_point+1, 1/*cycles*/, Licel_normaleized);
		Licel_Scale_Analog_Data(Licel_normaleized, num_point, MILLIVOLT100, 16, anolog[i]);

		//光子计数
		Licel_TCPIP_GetDatasets(Licel_ctrl_socket, Licel_Card_TR[i], PHOTON, num_point+4, memory, (unsigned char*)Licel_photon_LSW);
		//Licel_Convert_Photoncounting_FullWord(Licel_photon_LSW, Licel_analog_PHM, num_point, Licel_photon_c);
		for(int j=0;j<num_point;j++)
		{
			photon[i][j] = Licel_photon_LSW[j+4];
		}
		//Licel_Normalize_Data(Licel_photon_c, num_point, cycles, photon[i]);
	}
}

int ConfigAcq()
{
	const int discriminator_thresh_hold = 1;
	const char host[255] = "10.49.234.234";
	const int port = 2055;

	Licel_ctrl_socket = openConnection(host, port);
	if (INVALID_SOCKET == Licel_ctrl_socket)
		return -1;

	Licel_TCPIP_SetSlaveMode(Licel_ctrl_socket);
	Licel_TCPIP_SetShotLimit(Licel_ctrl_socket,0);

	//配置licel卡
	for(int i=0;i<Licel_Card_TR_Count;i++)
	{
		Licel_TCPIP_SelectTR(Licel_ctrl_socket, Licel_Card_TR[i]);
		Licel_TCPIP_SetInputRange(Licel_ctrl_socket, MILLIVOLT100); // 100mV
		Licel_TCPIP_SetThresholdMode(Licel_ctrl_socket, THRESHOLD_LOW); //high threshold range.
		Licel_TCPIP_SetDiscriminatorLevel(Licel_ctrl_socket, discriminator_thresh_hold); // threshold 4
	}

	return 0;
}

int LicelAcq_Start(unsigned int acccount,unsigned int curvepoint,HWND hMsgWnd)
{
	Licel_analog_MSW = new unsigned short[curvepoint+1];
	Licel_analog_LSW = new unsigned short[curvepoint+1];
	Licel_analog_PHM = new unsigned short[curvepoint+1];
	Licel_photon_LSW = new unsigned short[curvepoint+4];
	Licel_ananlog_acc = new unsigned long[curvepoint+1];
	Licel_photon_c = new unsigned long[curvepoint+1];
	Licel_clips = new short[curvepoint+1];
	Licel_normaleized = new double[curvepoint+1];

	for(int i=0;i<Licel_Card_TR_Count;i++)
	{
		Licel_analog_pBuf[i] = new double[curvepoint];
		Licel_photon_pBuf[i] = new double[curvepoint];
		for(int j=0;j<curvepoint;j++)
		{
			Licel_analog_pBuf[i][j] = 0.0;
			Licel_photon_pBuf[i][j] = 0.0;
		}
	}

	for(int i=0;i<Licel_Card_TR_Count * 2;i++)
	{
		Licel_pSwapBuf[i] = new double[curvepoint];
		for(int j=0;j<curvepoint;j++)
		{
			Licel_pSwapBuf[i][j] = 0.0;
		}
	}
	
	gb_Licel_pLicelAcqParams = new LicalAcqParams;
	gb_Licel_pLicelAcqParams->m_acccount = acccount;
	gb_Licel_pLicelAcqParams->m_curvepoint = curvepoint;
	gb_Licel_pLicelAcqParams->m_hMsgWnd = hMsgWnd;

	gb_Licel_bAcqCtl = TRUE;
	AfxBeginThread(LicelAcq_Loop,(LPVOID)gb_Licel_pLicelAcqParams,THREAD_PRIORITY_NORMAL,0,0,NULL);
	return 0;
}

UINT __cdecl LicelAcq_Loop(LPVOID lpThreadParam)
{
	long iCycles;
	int iMemory,iAcq_State,iRecording;

	while(gb_Licel_bAcqCtl)
	{
		ConfigAcq();

		//选中所有licel卡
		Licel_TCPIP_SelectMultipleTR(Licel_ctrl_socket, Licel_Card_TR, 3);

		iCycles = 0;
		Licel_TCPIP_MultipleStartAcquisition(Licel_ctrl_socket);
		while (iCycles < ((PTR_LicalAcqParams)lpThreadParam)->m_acccount + 1)
		{
			Licel_TCPIP_GetStatus(Licel_ctrl_socket, &iCycles, &iMemory, &iAcq_State, &iRecording);
		}
		Licel_TCPIP_MultipleStopAcquisition(Licel_ctrl_socket);

		if (iCycles > 1)
			iCycles -= 2;

		GetAcq(iCycles,
				gb_Licel_pLicelAcqParams->m_curvepoint,
				iMemory,
				Licel_photon_pBuf,
				Licel_analog_pBuf);
		
		gb_Licel_CriticalSection.Lock();
		for(int i=0;i<Licel_Card_TR_Count;i++)
			//0，1，2光子计数信号
			memcpy(Licel_pSwapBuf[i],Licel_photon_pBuf[i],gb_Licel_pLicelAcqParams->m_curvepoint*sizeof(double));
		for(int i=0;i<Licel_Card_TR_Count;i++)
			//3，4，5模拟信号
			memcpy(Licel_pSwapBuf[i+3],Licel_analog_pBuf[i],gb_Licel_pLicelAcqParams->m_curvepoint*sizeof(double));
		gb_Licel_CriticalSection.Unlock();

		TRACE(_T("LicelAcq: licel acquire completed!\n"));
		::PostMessage(gb_Licel_pLicelAcqParams->m_hMsgWnd,WM_CAPTUREEVENT,1,0);

		closeConnection(Licel_ctrl_socket);
	}

	TRACE(_T("LicelAcq: acquire thread quit!\n"));
	return 0;
}

void LicelAcq_Stop()
{
	gb_Licel_bAcqCtl = false;

	for(int i=0;i<Licel_Card_TR_Count;i++)
	{
		delete Licel_photon_pBuf[i];
		delete Licel_analog_pBuf[i];
	}

	for(int i=0;i<Licel_Card_TR_Count * 2;i++)
	{
		delete Licel_pSwapBuf[i];
	}
}

bool LicelAcq_GetData(double** pData,int arraysize)
{
	gb_Licel_CriticalSection.Lock();
	for(int i=0;i<Licel_Card_TR_Count;i++)
		//NOTE: first column is reserved for distance data
		memcpy(pData[i*2+1],Licel_pSwapBuf[i],arraysize * sizeof(double));
	for(int i=0;i<Licel_Card_TR_Count;i++)
		//NOTE: first column is reserved for distance data
		memcpy(pData[i*2+2],Licel_pSwapBuf[i + 3],arraysize * sizeof(double));
	gb_Licel_CriticalSection.Unlock();

	return true;
}
