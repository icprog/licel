// algcpp.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "algcpp.h"
#include "math.h"

ALGCPP_API void RegularCalc(double* signal,int DataNum, double* regularSignal)
{
	unsigned long* uSignal = new unsigned long[DataNum];
	for(int i=0;i<DataNum;i++)
		uSignal[i] = signal[i];
	SignalPreprocess(uSignal,DataNum,regularSignal);
	delete[]  uSignal;
}

ALGCPP_API void DepolarCalc(double kfactor,double DataNum,double* regularSignal1,double* regularSignal2,double* depolar)
{
	for(int i=0;i<DataNum;i++)
		(*(depolar+i)) = kfactor * (*(regularSignal1+i)) / (*(regularSignal2+i));
}

ALGCPP_API void BackScatterCalc(int DataNum, double* regularSignal, double rangeResolution, double RangeRef, double Wavelength, double* depolar, double* backScatter)
{
	double* overlap = new double[DataNum];
	for(int i=0;i<DataNum;i++)
		overlap[i] = 1.0;

	GetBackscatter(regularSignal,DataNum,rangeResolution,RangeRef,Wavelength,depolar,overlap,backScatter);
	delete[] overlap;
}

ALGCPP_API void ExtinctionCalc(int DataNum, double* backScatter, double* extinction)
{
	GetExtinction(backScatter,DataNum,extinction);
}

ALGCPP_API void CloudCalc(double rangeResolution, double* regularSignal,int DataNum,double* cloud_base,double* cloud_top,int& cloud_num)
{
	//GetCloudInfo(regularSignal,DataNum,rangeResolution,cloud_base,cloud_top,cloud_num);
}

ALGCPP_API void OpticDepthCalc(double rangeResolution, double* Extinction,int DataNum,double* depth)
{
	//去除盲区后对消光系数求积分
	(*depth) = 0;
	for(unsigned int i=0;i<DataNum;i++)
	{
		(*depth) += (Extinction[i] * rangeResolution);
	}
}

void alg_HeightAverge(double* Range_inst,double* Signal_inst,int DataNum)
{
	//输入数据为距离平方之后的信号
	//对500m以内的回波信号求平均，步长为5
	unsigned int i=0;
	for(;i<DataNum;i++)
	{
		if(Range_inst[i] > 500)
			break;

		if(i > 1)
			Signal_inst[i] = (Signal_inst[i-2] + Signal_inst[i-1] + Signal_inst[i] + Signal_inst[i+1] + Signal_inst[i+2])/5.0;
	}

	//500m~1000m以内的回波信号求平均，步长为9
	for(;i<DataNum;i++)
	{
		if(Range_inst[i] > 1000)
			break;

		Signal_inst[i] = 0.0;
		for(int j=0;j<9;j++)
			Signal_inst[i] += Signal_inst[i+j-4];
		Signal_inst[i] /= 9.0;
	}

	//1000m~2000m以内的回波信号求平均，步长为13
	for(;i<DataNum;i++)
	{
		if(Range_inst[i] > 2000)
			break;

		Signal_inst[i] = 0.0;
		for(int j=0;j<13;j++)
			Signal_inst[i] += Signal_inst[i+j-6];
		Signal_inst[i] /= 13.0;
	}

	//对2000m以上的回波信号求平均，步长为17
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

ALGCPP_API void BoundaryCalc(double* Range_inst,double** regularSignal,int DataNum, int FrameNum, int period, double* boundary)
{
	unsigned int reso = 10*60;
	unsigned int interval = reso / period;

	double **signal = new double*[FrameNum];
	for(int i=0;i<FrameNum;i++)
	{
		signal[i] = new double[DataNum];
		memcpy(signal[i],regularSignal[i],sizeof(double) * DataNum);
	}

	for(unsigned int k=0;k<FrameNum;k++)
	{
		alg_HeightAverge(Range_inst,signal[k],DataNum);

		if(k < interval)
			continue;

		for(unsigned int i=0;i<DataNum;i++)
		{
			signal[k][i]=0;
			for(unsigned int j=0;j<interval;j++)
				signal[k][i] += signal[k][i-j];

			signal[k][i] /= interval;
		}
	}

	//求梯度
	double dis = Range_inst[1] - Range_inst[0]; 
	for(unsigned int k=0;k<FrameNum;k++)
	{
		for(unsigned int i=0;i<DataNum;i++)
		{
			if(i == 0)
				signal[k][i] = (signal[k][i+1] - signal[k][i]) / dis;
			else if(i == DataNum -1)
				signal[k][i] = (signal[k][i] - signal[k][i-1]) / dis;
			else signal[k][i] = (signal[k][i+1] - signal[k][i-1]) / (2*dis);
		}
		alg_HeightAverge(Range_inst,signal[k],DataNum);
	}

	//求二阶梯度
	for(unsigned int k=0;k<FrameNum;k++)
	{
		for(unsigned int i=0;i<DataNum;i++)
		{
			if(i == 0)
				signal[k][i] = (signal[k][i+1] - signal[k][i]) / dis;
			else if(i == DataNum -1)
				signal[k][i] = (signal[k][i] - signal[k][i-1]) / dis;
			else signal[k][i] = (signal[k][i+1] - signal[k][i-1]) / (2*dis);
		}
	}

	for(unsigned int k=0;k<FrameNum;k++)
	{
		for(unsigned int i=0;i<DataNum;i++)
		{
			if(i < 1)
				continue;

			if(signal[k][i] > 0 &&
				signal[k][i-1] < 0 &&
				Range_inst[i] > 300 &&
				Range_inst[i] < 3000 )
			{
				boundary[k] = Range_inst[i];
				break;
			}
		}
	}

	for(int i=0;i<FrameNum;i++)
		delete[] signal[i];
	delete[] signal;
}

ALGCPP_API void VisiblityCalc(double* Extinction,int DataNum,double* visiblity)
{
	double a = 0.02;
	for(unsigned int i=0;i<DataNum;i++)
		visiblity[i] = -log(a) / Extinction[i];
}

ALGCPP_API void MixCalc(double rangeResolution,double* Extinction,int DataNum,double* mix)
{
	double a = 0.06;
	double b = 0.7;

	(*mix) = 0;
	for(unsigned int i=0;i<DataNum;i++)
		(*mix) += (Extinction[i] * rangeResolution * i * a * b);
}

ALGCPP_API void GasoloidPotencyCalc(double* Extinction,int DataNum,double* gasoloid)
{
	double a = 200;//532nm波长的参数
	double b = 157;

	for(unsigned int i=0;i<DataNum;i++)
		gasoloid[i] = a * Extinction[i] + b;
}