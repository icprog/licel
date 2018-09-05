/*******************************************
Copyright(c) 2017  Advanced Lidar Studios
All rights reserved.

版    本：1.0.0.1（版本更新以DLL文件为准）
创建日期：2017年10月24日
修改日期：2017年10月24日
作    者：Advanced Lidar Studios
功    能：用于米散射激光雷达的云信息反演
********************************************/

/*
* 功能描述：
* GetCloudInfo --- 由回波信号获取云底、云高、云层信息

*/
#ifdef ALGORITHM_EXPORTS
#define ALGORITHM_API __declspec(dllexport)
#else
#define ALGORITHM_API __declspec(dllimport)
#endif


//功    能：由回波信号获取云底、云高、云层信息
//输入参量：
//		signal		回波信号；data_num个double型数组
//		range		距离，单位米；data_num个double数组；
//					考虑盲区，如盲区为40米，分辨率为3.75米，则该数组数值依次为43.75，47.5，51.25，。。。
//		data_num	数据点数
//		type		标志位，保留

//输出参量：
//		cloud_base	云底数组，存放各个云层的云底高度信息，单位米
//					该数组元素个数应大于20（也即最大云层数为20）
//		cloud_top	云高数组，存放各个云层的云高高度信息，单位米
//					该数组元素个数应大于20（也即最大云层数为20）
//		layer_num	云层数

//返回值：
//		0			正确
//		-1			错误	
extern "C" ALGORITHM_API int GetCloudInfo(double* signal, double* range, int data_num, int type, double* cloud_base, double* cloud_top, int& layer_num);

//功    能：平滑
extern "C" ALGORITHM_API int Smooth(double* Data, int DataNum, int Span);

//功    能：插值
extern "C" ALGORITHM_API int Interp(double* pX, double* pY, int PointNum, double x, double& y);
