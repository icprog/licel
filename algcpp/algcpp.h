#ifdef ALGCPP_EXPORTS
#define ALGCPP_API __declspec(dllexport)
#else
#define ALGCPP_API __declspec(dllimport)
#endif

//ƽ��
int alg_Smooth(double* Data, int DataNum, int Span);
//�ڲ�
int alg_Interp(double* pX, double* pY, int PointNum, double x, double& y);
int InterpCalc(double* pRange,int DataNum);
int ExtinctionMolCalc(double* Temperature, double* Pressure, int DataNum, double Wavelength, double* Data);
//��������
extern "C" ALGCPP_API double BackgroundNoise(double* Range_inst,double* Signal_inst,int DataNum);
//����ϵ��,ȥ������Ծ���ƽ����Ϊ����
extern "C" ALGCPP_API int Fernald(double* Range_inst, double* Signal_inst, int DataNum, double* Extinction);
//��ƫ���
extern "C" ALGCPP_API void DepolarCalc(double kfactor,double DataNum,double* InData1,double* InData2,double* OutData);
//����Ϣ
extern "C" ALGCPP_API void CloudCalc(double* Range_inst,double* Signal_inst,int DataNum,double* cloud_base,double* cloud_top,int& cloud_num);
//��ѧ���
extern "C" ALGCPP_API void OpticDepthCalc(double* Range_inst,double* Extinction,int DataNum,double* depth);
//�����߽��
void alg_HeightAverge(double* Range_inst,double* Signal_inst,int DataNum);
extern "C" ALGCPP_API void AtmosphereBoundaryCalc(double* Range_inst,double** Signal_inst,int DataNum,int FrameNum,int period,double* boundary);
//��ֱ�ܼ���
extern "C" ALGCPP_API void VisiblityCalc(double* Range_inst,double* Extinction,int DataNum,double* visiblity);
//��Ⱦ��Ũ��

//����ģ��
extern  "C" ALGCPP_API int LoadAtmosphereModel(char* path);
extern  "C" ALGCPP_API void ReleaseAtmosphereModel();