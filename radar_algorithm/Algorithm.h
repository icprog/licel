/*******************************************
Copyright(c) 2017  Advanced Lidar Studios
All rights reserved.

版    本：1.0.0.1（版本更新以DLL文件为准）
创建日期：2018年8月28日
修改日期：2018年8月28日
作    者：Advanced Lidar Studios
功    能：用于米散射激光雷达的消光系数及云信息反演
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

#define RANGE_START			(30)		//起始距离，单位：米
#define SMOOTH_SPAN_NUM		(7)		//平滑点数
#define NOISE_INDEX_START	(20)	//噪声部分起点下标（倒数）
#define NOISE_INDEX_END		(10)	//噪声部分终点下标（倒数）
#define FACTOR_POSITION		(300)	// 几何重叠因子区域，单位：米
#define TEMPERATURE_ON_GROUD (22.1312)	// 地面温度,单位:摄氏度
#define PRESSURE_ON_GROUD	(1009.46) // 地面气压,单位:毫巴mbar
#define REFERANCE_POSITION_START (10000)	//参考点位置起始点
#define REFERANCE_POSITION_END (12000)		//参考点位置终点
#define EXTINCTION_BACKSCATTER_RATIO (40)		//散射消光比

//功    能：由预处理后的回波信号获取云底、云高、云层信息
//输入参量：
//		Data		预处理后回波信号；Datanum个double型数组
//		Datanum		数据点数
//		RangeResolution	距离分辨率，单位米
//输出参量：
//		CloudBase	云底数组，存放各个云层的云底高度信息，单位米
//					该数组元素个数应大于10（也即最大云层数为10）
//		CloudTop	云高数组，存放各个云层的云高高度信息，单位米
//					该数组元素个数应大于10（也即最大云层数为10）
//		LayerNum	云层数
//返回值：
//		0			正确
//		-1			错误	
extern "C" ALGORITHM_API int GetCloudInfo(double* Data, int DataNum, double RangeResolution, double* CloudBase, double* CloudTop, int& LayerNum);



//功    能：对回波信号进行预处理，包括去噪、平滑;
//输入参量：
//		Signal		回波信号；DataNum个unsigned long型数组
//		DataNum		数据点数
//输出参量：
//		Data		预处理后数据
//返回值：
//		0			正确
//		-1			错误	
extern "C" ALGORITHM_API int SignalPreprocess(unsigned long* Signal, int DataNum, double* Data);



//功    能：由预处理后信号获取气溶胶的后向散射系数;
//输入参量：
//		Data		预处理后回波信号；Datanum个double型数组
//		DataNum		数据点数
//		RangeResolution 距离分辨率，单位：米
//		RangeRef	参考距离位置，单位：米
//		Wavelength	信号通道波长，单位：纳米
//		DepolarizeRatio 退偏振比，无量纲，0～100范围内。Datanum个double型数组
//		Overlap		几何因子，无量纲，0～1范围。Datanum个double型数组。可以设置为全是1
//输出参量：
//		AerosolBackscatter	气溶胶后向散射系数
//返回值：
//		0			正确
//		-1			错误	
extern "C" ALGORITHM_API int GetBackscatter(double* Data, int DataNum, double RangeResolution, double RangeRef, double Wavelength, double* DepolarizeRatio, double* Overlap, double* AerosolBackscatter);


//功    能：由气溶胶的后向散射系数求取消光系数;
//输入参量：
//		AerosolBackscatter	气溶胶后向散射系数；Datanum个double型数组
//		DataNum		数据点数
//输出参量：
//		AerosolExtinction	气溶胶消光系数
//返回值：
//		0			正确
//		-1			错误	
extern "C" ALGORITHM_API int GetExtinction(double* AerosolBackscatter, int DataNum, double* AerosolExtinction);

//功    能：平滑
extern "C" ALGORITHM_API int Smooth(double* Data, int DataNum, int Span);

//功    能：插值
extern "C" ALGORITHM_API int Interp(double* pX, double* pY, int PointNum, double x, double& y);



