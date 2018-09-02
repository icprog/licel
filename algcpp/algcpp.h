#ifdef ALGCPP_EXPORTS
#define ALGCPP_API __declspec(dllexport)
#else
#define ALGCPP_API __declspec(dllimport)
#endif

//平滑
int alg_Smooth(double* Data, int DataNum, int Span);
//内插
int alg_Interp(double* pX, double* pY, int PointNum, double x, double& y);
int InterpCalc(double* pRange,int DataNum);
int ExtinctionMolCalc(double* Temperature, double* Pressure, int DataNum, double Wavelength, double* Data);
//背景计算
extern "C" ALGCPP_API double BackgroundNoise(double* Range_inst,double* Signal_inst,int DataNum);
//消光系数,去底噪乘以距离平方作为输入
extern "C" ALGCPP_API int Fernald(double* Range_inst, double* Signal_inst, int DataNum, double* Extinction);
//退偏振比
extern "C" ALGCPP_API void DepolarCalc(double kfactor,double DataNum,double* InData1,double* InData2,double* OutData);
//云信息
extern "C" ALGCPP_API void CloudCalc(double* Range_inst,double* Signal_inst,int DataNum,double* cloud_base,double* cloud_top,int& cloud_num);
//光学厚度
extern "C" ALGCPP_API void OpticDepthCalc(double* Range_inst,double* Extinction,int DataNum,double* depth);
//大气边界层
void alg_HeightAverge(double* Range_inst,double* Signal_inst,int DataNum);
extern "C" ALGCPP_API void AtmosphereBoundaryCalc(double* Range_inst,double** Signal_inst,int DataNum,int FrameNum,int period,double* boundary);
//垂直能见度
extern "C" ALGCPP_API void VisiblityCalc(double* Range_inst,double* Extinction,int DataNum,double* visiblity);
//污染物浓度

//大气模型
extern  "C" ALGCPP_API int LoadAtmosphereModel(char* path);
extern  "C" ALGCPP_API void ReleaseAtmosphereModel();