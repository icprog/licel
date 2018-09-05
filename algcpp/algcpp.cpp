// algcpp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "algcpp.h"
#include "math.h"
#include <fstream>
#include <iostream>
using namespace std;

typedef struct tagATMOSPHEREMODEL
{
	unsigned int modelNum;
	double *ModelAltitude;
	double *ModelTemperature;
	double *ModelPressure;
}ATMOSPHEREMODEL;

ATMOSPHEREMODEL gb_AtmosphereModel;
ATMOSPHEREMODEL gb_AtmosphereModel_Interp;
double gb_SineOFAngle = 0.44;

double S1 = 50; 
double S2 = 8 * 3.14159265 / 3; 

int iw = 0;

int alg_Smooth(double* Data, int DataNum, int Span)
{
	double *TempData = new double[DataNum];
	memset(TempData, 0, sizeof(double)*DataNum);

	double TempSum = 0;
	double TempDif = 0;
	int i,j;

	if (Span % 2 == 0)
	{
		int nSegment = Span >> 1;

		for (i = 0; i<DataNum; i++)
		{
			if (i<nSegment)
			{
				for (j = 0; j <= i + nSegment; j++)
					TempSum += Data[j];
				TempData[i] = TempSum / (i + 1 + nSegment)*Span;
				TempSum = 0;
			}
			else if (i<DataNum - nSegment)
				TempData[i] = TempData[i - 1] + Data[i + nSegment] - Data[i - nSegment];
			else
			{
				for (j = i - nSegment - 1; j<DataNum; j++)
					TempSum += Data[j];
				TempData[i] = TempSum / (DataNum - i - 1 + nSegment + 2)*Span;
				TempSum = 0;
			}
		}
	}
	else 
	{
		int nSegmentRight = (Span - 1) / 2;
		int nSegmentLeft = (Span + 1) / 2;

		for (i = 0;i<DataNum;i++)
		{
			if (i<nSegmentLeft)
			{
				for (j = 0; j <= i + nSegmentRight; j++)
					TempSum += Data[j];
				TempData[i] = TempSum / (i + 1 + nSegmentRight)*Span;
				TempSum = 0;
			}
			else if (i<DataNum - nSegmentRight)
				TempData[i] = TempData[i - 1] + Data[i + nSegmentRight] - Data[i - nSegmentLeft];
			else
			{
				for (j = i - nSegmentRight; j<DataNum; j++)
					TempSum += Data[j];
				TempData[i] = TempSum / (DataNum - (i + 1 - nSegmentRight) + 1)*Span;
				TempSum = 0;
			}
		}
	}
	for (i = 0; i<DataNum; i++)
		Data[i] = TempData[i] / Span;
	delete[] TempData;
	return 0;
}

int alg_Interp(double* pX, double* pY, int PointNum, double x, double& y)
{
	int i;
	for (i = 1; i<PointNum; i++)
	{
		if (x <= *(pX + i))
		{
			y = (x - pX[i]) / (pX[i - 1] - pX[i])*pY[i - 1] +
				(x - pX[i - 1]) / (pX[i] - pX[i - 1])*pY[i];
			break;
		}
		if (i == PointNum)
			return 0;
	}
	return 0;
}

int ExtinctionMolCalc(double* Temperature, double* Pressure, int DataNum, double Wavelength, double* Data)
{
	double StdTemperature = 296; // K
	double StdPressure = 1.013e5; // Pa
	double LoschmidtNum = 2.479e25; // molecules per m^3
	double StdWavelength = 550; // nm
	double ExtBackRatio = 8 * 3.14159265 / 3; // extinction-to-backscatter ratio
	double CrossSectionMol; // Rayleigh backscatter cross section per molecule, m^2 sr^-1
	double CrossSectionMolRatio = 5.45e-32;

	CrossSectionMol = pow(StdWavelength / Wavelength, 4)*CrossSectionMolRatio;

	int i;
	for (i = 0; i<DataNum; i++)
	{
		Data[i] = (StdTemperature / Temperature[i])*(Pressure[i] / StdPressure)*LoschmidtNum*CrossSectionMol*ExtBackRatio;
	}
	return 0;
}

int InterpCalc(double* pRange,int DataNum)
{
	return 0;
}

ALGCPP_API int BackgroundNoise(double* Range_inst,double* Signal_inst,int DataNum,double* averge,double* deviation)
{
	unsigned int idx = 0;
	bool bRangeFind = false;
	for(;idx<DataNum;idx++)
	{
		if(Range_inst[idx] >= 10000)//10km处
		{
			bRangeFind = true;
			break;
		}
	}

	if(bRangeFind = false)
		return 0;

	unsigned int i;
	double sum = 0;
	for(i=0;(i<50) && (i+idx < DataNum);i++)
		sum += Signal_inst[idx + i];
	(*averge) = sum / (double)i;

	unsigned int j;
	double dsum = 0;
	for(j=0;j<i;j++)
		dsum = pow((Signal_inst[idx + j] - *averge),2.0);
	(*deviation) = sqrt(dsum / (double)i);

	return 1;
}

ALGCPP_API int Fernald(double* Range_inst, double* Signal_inst, int DataNum, double* Extinction)
{
	double *Range = new double[DataNum];
	double *Signal = new double[DataNum];
	memcpy(Range,Range_inst,sizeof(double)*DataNum);
	memcpy(Signal,Signal_inst,sizeof(double)*DataNum);

	for (unsigned int i = 0; i<DataNum; i++)
		Range[i] *= gb_SineOFAngle;

	double RangeStep = Range[1] - Range[0]; // 
	double RangeRef = 9e3;  
	int OrderRef = 150/*(int)ceil(RangeRef / RangeStep)*/; // config

	for(unsigned int i=0;i<DataNum;i++)
		Extinction[i] = 0;
	
	alg_Smooth(Signal, DataNum, 3); 

	double SignalNoise = 0;
	for (unsigned int i = DataNum - 200; i<DataNum; i++) 
		SignalNoise += Signal[i];
	SignalNoise /= 201;
	for (unsigned int i = 0; i<DataNum; i++)
		Signal[i] -= SignalNoise;

	double *Temperature = new double[DataNum];
	memset(Temperature, 0, sizeof(double)*DataNum);
	double *Pressure = new double[DataNum];
	memset(Pressure, 0, sizeof(double)*DataNum);

	for (unsigned int i = 0; i<DataNum; i++)
	{
		alg_Interp(gb_AtmosphereModel.ModelAltitude, gb_AtmosphereModel.ModelTemperature, 2001, Range[i], Temperature[i]);
		alg_Interp(gb_AtmosphereModel.ModelAltitude, gb_AtmosphereModel.ModelPressure, 2001, Range[i], Pressure[i]);
	}

	double *ExtinctionMol = new double[DataNum];
	memset(ExtinctionMol, 0, sizeof(double)*DataNum);
	ExtinctionMolCalc(Temperature, Pressure, DataNum, 532, ExtinctionMol); 
	double ExtinctionRef = ExtinctionMol[OrderRef - 1];

	double *TempExtMolInt = new double[OrderRef]; 
	for(unsigned int i=0;i<OrderRef;i++)
		TempExtMolInt[i] = 0.0;

	for (unsigned int i = 0; i < OrderRef; i++)
	{
		for (unsigned int j = i; j < OrderRef; j++)
		{
			TempExtMolInt[i] += (ExtinctionMol[j] * RangeStep);
		}
	}

	double *Temp1 = new double[OrderRef];
	memset(Temp1, 0, sizeof(double)*OrderRef);
	for (unsigned int i = 0; i<OrderRef; i++)
		Temp1[i] = Signal[i] * pow(Range[i], 2)*exp(2 * (S1 / S2 - 1)*TempExtMolInt[i]);
		
	double SignalRef = 0; //
	for (unsigned int i = OrderRef - 19;i<OrderRef + 19;i++) // 
		SignalRef += Signal[i]*pow(Range[i],2);
	SignalRef /= 39;
	double Temp2 = abs(SignalRef) / (0 + S1 / S2*ExtinctionRef);

	double *Temp3 = new double[OrderRef];
	memset(Temp3, 0, sizeof(double)*OrderRef);
	for (unsigned int i = 0; i<OrderRef; i++)
	{
		for (unsigned int j = i; j < OrderRef; j++)
		{
			Temp3[i] += Temp1[j] * RangeStep;
		}
	}

	for (unsigned int i = 0; i<OrderRef; i++)
	{
		Extinction[i] = -S1 / S2*ExtinctionMol[i] + Temp1[i] / (Temp2 + Temp3[i]);
		Extinction[i] *= 1000;
	}

	delete[] Temperature;
	delete[] Pressure;
	delete[] TempExtMolInt;
	delete[] Temp1;
	delete[] Temp3;

	return 0;
}

ALGCPP_API void DepolarCalc(double kfactor,double DataNum,double* InData1,double* InData2,double* OutData)
{
	for(int i=0;i<DataNum;i++)
		(*(OutData+i)) = kfactor * (*(InData1+i)) / (*(InData2+i));
}

ALGCPP_API void OpticDepthCalc(double* Range_inst,double* Extinction,int DataNum,double* depth)
{
	//去盲区后对消光系数求积分
	double dis = Range_inst[1] - Range_inst[0];
	(*depth) = 0;
	for(unsigned int i=0;i<DataNum;i++)
	{
		(*depth) += Extinction[i] * dis;
	}
}

void alg_HeightAverge(double* Range_inst,double* Signal_inst,int DataNum)
{
	//输入数据为距离平方之后的信号
	//500以内高度求平均
	unsigned int i=0;
	for(;i<DataNum;i++)
	{
		if(Range_inst[i] > 500)
			break;

		if(i > 1)
			Signal_inst[i] = (Signal_inst[i-2] + Signal_inst[i-1] + Signal_inst[i] + Signal_inst[i+1] + Signal_inst[i+2])/5.0;
	}

	//500~1000以内高度求平局
	for(;i<DataNum;i++)
	{
		if(Range_inst[i] > 1000)
			break;

		Signal_inst[i] = 0.0;
		for(int j=0;j<9;j++)
			Signal_inst[i] += Signal_inst[i+j-4];
		Signal_inst[i] /= 9.0;
	}

	//1000~2000以内高度求平局
	for(;i<DataNum;i++)
	{
		if(Range_inst[i] > 2000)
			break;

		Signal_inst[i] = 0.0;
		for(int j=0;j<13;j++)
			Signal_inst[i] += Signal_inst[i+j-6];
		Signal_inst[i] /= 13.0;
	}

	//
	for(;i<DataNum;i++)
	{
		if(DataNum - i > 8)
			break;

		Signal_inst[i] = 0.0;
		for(int j=0;j<17;j++)
			Signal_inst[i] += Signal_inst[i+j-8];
		Signal_inst[i] /= 17.0;
	}
}

ALGCPP_API void AtmosphereBoundaryCalc(double* Range_inst,double** Signal_inst,int DataNum,int FrameNum,int period,double* boundary)
{
	unsigned int reso = 10*60;
	unsigned int interval = reso / period;

	for(unsigned int k=0;k<FrameNum;k++)
	{
		alg_HeightAverge(Range_inst,Signal_inst[k],DataNum);

		if(k < interval)
			continue;

		for(unsigned int i=0;i<DataNum;i++)
		{
			Signal_inst[k][i]=0;
			for(unsigned int j=0;j<interval;j++)
				Signal_inst[k][i] += Signal_inst[k][i-j];

			Signal_inst[k][i] /= interval;
		}
	}

	//求梯度
	double dis = Range_inst[1] - Range_inst[0]; 
	for(unsigned int k=0;k<FrameNum;k++)
	{
		for(unsigned int i=0;i<DataNum;i++)
		{
			if(i == 0)
				Signal_inst[k][i] = (Signal_inst[k][i+1] - Signal_inst[k][i]) / dis;
			else if(i == DataNum -1)
				Signal_inst[k][i] = (Signal_inst[k][i] - Signal_inst[k][i-1]) / dis;
			else Signal_inst[k][i] = (Signal_inst[k][i+1] - Signal_inst[k][i-1]) / (2*dis);
		}
		alg_HeightAverge(Range_inst,Signal_inst[k],DataNum);
	}

	//求二阶梯度
	for(unsigned int k=0;k<FrameNum;k++)
	{
		for(unsigned int i=0;i<DataNum;i++)
		{
			if(i == 0)
				Signal_inst[k][i] = (Signal_inst[k][i+1] - Signal_inst[k][i]) / dis;
			else if(i == DataNum -1)
				Signal_inst[k][i] = (Signal_inst[k][i] - Signal_inst[k][i-1]) / dis;
			else Signal_inst[k][i] = (Signal_inst[k][i+1] - Signal_inst[k][i-1]) / (2*dis);
		}
	}

	for(unsigned int k=0;k<FrameNum;k++)
	{
		for(unsigned int i=0;i<DataNum;i++)
		{
			if(i < 1)
				continue;

			if(Signal_inst[k][i] > 0 &&
				Signal_inst[k][i-1] < 0 &&
				Range_inst[i] > 300 &&
				Range_inst[i] < 3000 )
			{
				boundary[k] = Range_inst[i];
				break;
			}
		}
	}
}

ALGCPP_API void VisiblityCalc(double* Range_inst,double* Extinction,int DataNum,double* visiblity)
{
	double a1 = 0.02;
	double a2 = 0.05;
	for(unsigned int i=0;i<DataNum;i++)
		visiblity[i] = -log(a1)/Extinction[i];
}

ALGCPP_API int LoadAtmosphereModel(char* path)
{
	const unsigned int MODEL_MAX = 2100;
	ifstream f(path);
	if (f.bad())
		return -1;

	double *ModelAltitude = new double[MODEL_MAX];
	double *ModelTemperature = new double[MODEL_MAX];
	double *ModelPressure = new double[MODEL_MAX];
	double range;
	unsigned int idx = 0;
	while (!f.eof())
	{
		f >> ModelAltitude[idx] >> ModelTemperature[idx] >> ModelPressure[idx]>>range;
		idx++;
	}

	gb_AtmosphereModel.modelNum = idx;
	gb_AtmosphereModel.ModelTemperature = ModelTemperature;
	gb_AtmosphereModel.ModelAltitude = ModelAltitude;
	gb_AtmosphereModel.ModelPressure = ModelPressure;

	f.close();
	return idx;
}

ALGCPP_API void ReleaseAtmosphereModel()
{
	delete[] gb_AtmosphereModel.ModelTemperature;
	delete[] gb_AtmosphereModel.ModelAltitude;
	delete[] gb_AtmosphereModel.ModelPressure;
}

ALGCPP_API void CloudCalc(double* Range_inst,double* Signal_inst,int DataNum,double* cloud_base,double* cloud_top,int& cloud_num)
{
	double* signal = new double[DataNum];
	double* range = new double[DataNum];
	memcpy(signal,Signal_inst,sizeof(double) * DataNum);
	memcpy(range,Range_inst,sizeof(double) * DataNum);

	GetCloudInfo(signal,range,DataNum,1,cloud_base,cloud_top,cloud_num);
}