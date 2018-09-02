/*******************************************
Copyright(c) 2017  Advanced Lidar Studios
All rights reserved.

��    ����1.0.0.1���汾������DLL�ļ�Ϊ׼��
�������ڣ�2017��10��24��
�޸����ڣ�2017��10��24��
��    �ߣ�Advanced Lidar Studios
��    �ܣ�������ɢ�伤���״������Ϣ����
********************************************/

/*
* ����������
* GetCloudInfo --- �ɻز��źŻ�ȡ�Ƶס��Ƹߡ��Ʋ���Ϣ

*/
#ifdef ALGORITHM_EXPORTS
#define ALGORITHM_API __declspec(dllexport)
#else
#define ALGORITHM_API __declspec(dllimport)
#endif


//��    �ܣ��ɻز��źŻ�ȡ�Ƶס��Ƹߡ��Ʋ���Ϣ
//���������
//		signal		�ز��źţ�data_num��double������
//		range		���룬��λ�ף�data_num��double���飻
//					����ä������ä��Ϊ40�ף��ֱ���Ϊ3.75�ף����������ֵ����Ϊ43.75��47.5��51.25��������
//		data_num	���ݵ���
//		type		��־λ������

//���������
//		cloud_base	�Ƶ����飬��Ÿ����Ʋ���Ƶ׸߶���Ϣ����λ��
//					������Ԫ�ظ���Ӧ����20��Ҳ������Ʋ���Ϊ20��
//		cloud_top	�Ƹ����飬��Ÿ����Ʋ���Ƹ߸߶���Ϣ����λ��
//					������Ԫ�ظ���Ӧ����20��Ҳ������Ʋ���Ϊ20��
//		layer_num	�Ʋ���

//����ֵ��
//		0			��ȷ
//		-1			����	
extern "C" ALGORITHM_API int GetCloudInfo(double* signal, double* range, int data_num, int type, double* cloud_base, double* cloud_top, int& layer_num);

//��    �ܣ�ƽ��
extern "C" ALGORITHM_API int Smooth(double* Data, int DataNum, int Span);

//��    �ܣ���ֵ
extern "C" ALGORITHM_API int Interp(double* pX, double* pY, int PointNum, double x, double& y);
