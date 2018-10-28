#ifdef ALGCPP_EXPORTS
#define ALGCPP_API __declspec(dllexport)
#else
#define ALGCPP_API __declspec(dllimport)
#endif

//去底噪、平滑预处理
extern "C" ALGCPP_API void RegularCalc(double* signal,int DataNum, double* regularSignal);

//退偏振比
extern "C" ALGCPP_API void DepolarCalc(double kfactor,double DataNum,double* regularSignal1,double* regularSignal2,double* depolar);

//散射系数
extern "C" ALGCPP_API void BackScatterCalc(int DataNum, double* regularSignal, double rangeResolution, double RangeRef, double Wavelength, double* depolar, double* backScatter);

//消光系数
extern "C" ALGCPP_API void ExtinctionCalc(int DataNum,double* backScatter,double* extinction);

//云信息
extern "C" ALGCPP_API void CloudCalc(double rangeResolution,double* regularSignal,int DataNum,double* cloud_base,double* cloud_top,int& cloud_num);

//光学厚度
extern "C" ALGCPP_API void OpticDepthCalc(double rangeResolution,double* Extinction,int DataNum,double* depth);

//大气边界层高度
void alg_HeightAverge(double* Range_inst,double* Signal_inst,int DataNum);
extern "C" ALGCPP_API void BoundaryCalc(double* Range_inst,double** Signal_inst,int DataNum,int FrameNum,int period,double* boundary);

//垂直能见度
extern "C" ALGCPP_API void VisiblityCalc(double* Extinction,int DataNum,double* visiblity);

//污染物混合层高度
extern "C" ALGCPP_API void MixCalc(double rangeResolution,double* Extinction,int DataNum,double* mix);

//气溶胶浓度
extern "C" ALGCPP_API void GasoloidPotencyCalc(double* Extinction,int DataNum,double* gasoloid);