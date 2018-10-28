#ifdef ALGCPP_EXPORTS
#define ALGCPP_API __declspec(dllexport)
#else
#define ALGCPP_API __declspec(dllimport)
#endif

//ȥ���롢ƽ��Ԥ����
extern "C" ALGCPP_API void RegularCalc(double* signal,int DataNum, double* regularSignal);

//��ƫ���
extern "C" ALGCPP_API void DepolarCalc(double kfactor,double DataNum,double* regularSignal1,double* regularSignal2,double* depolar);

//ɢ��ϵ��
extern "C" ALGCPP_API void BackScatterCalc(int DataNum, double* regularSignal, double rangeResolution, double RangeRef, double Wavelength, double* depolar, double* backScatter);

//����ϵ��
extern "C" ALGCPP_API void ExtinctionCalc(int DataNum,double* backScatter,double* extinction);

//����Ϣ
extern "C" ALGCPP_API void CloudCalc(double rangeResolution,double* regularSignal,int DataNum,double* cloud_base,double* cloud_top,int& cloud_num);

//��ѧ���
extern "C" ALGCPP_API void OpticDepthCalc(double rangeResolution,double* Extinction,int DataNum,double* depth);

//�����߽��߶�
void alg_HeightAverge(double* Range_inst,double* Signal_inst,int DataNum);
extern "C" ALGCPP_API void BoundaryCalc(double* Range_inst,double** Signal_inst,int DataNum,int FrameNum,int period,double* boundary);

//��ֱ�ܼ���
extern "C" ALGCPP_API void VisiblityCalc(double* Extinction,int DataNum,double* visiblity);

//��Ⱦ���ϲ�߶�
extern "C" ALGCPP_API void MixCalc(double rangeResolution,double* Extinction,int DataNum,double* mix);

//���ܽ�Ũ��
extern "C" ALGCPP_API void GasoloidPotencyCalc(double* Extinction,int DataNum,double* gasoloid);