/*******************************************
Copyright(c) 2017  Advanced Lidar Studios
All rights reserved.

��    ����1.0.0.1���汾������DLL�ļ�Ϊ׼��
�������ڣ�2018��8��28��
�޸����ڣ�2018��8��28��
��    �ߣ�Advanced Lidar Studios
��    �ܣ�������ɢ�伤���״������ϵ��������Ϣ����
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

#define RANGE_START			(30)		//��ʼ���룬��λ����
#define SMOOTH_SPAN_NUM		(7)		//ƽ������
#define NOISE_INDEX_START	(20)	//������������±꣨������
#define NOISE_INDEX_END		(10)	//���������յ��±꣨������
#define FACTOR_POSITION		(300)	// �����ص��������򣬵�λ����
#define TEMPERATURE_ON_GROUD (22.1312)	// �����¶�,��λ:���϶�
#define PRESSURE_ON_GROUD	(1009.46) // ������ѹ,��λ:����mbar
#define REFERANCE_POSITION_START (10000)	//�ο���λ����ʼ��
#define REFERANCE_POSITION_END (12000)		//�ο���λ���յ�
#define EXTINCTION_BACKSCATTER_RATIO (40)		//ɢ�������

//��    �ܣ���Ԥ�����Ļز��źŻ�ȡ�Ƶס��Ƹߡ��Ʋ���Ϣ
//���������
//		Data		Ԥ�����ز��źţ�Datanum��double������
//		Datanum		���ݵ���
//		RangeResolution	����ֱ��ʣ���λ��
//���������
//		CloudBase	�Ƶ����飬��Ÿ����Ʋ���Ƶ׸߶���Ϣ����λ��
//					������Ԫ�ظ���Ӧ����10��Ҳ������Ʋ���Ϊ10��
//		CloudTop	�Ƹ����飬��Ÿ����Ʋ���Ƹ߸߶���Ϣ����λ��
//					������Ԫ�ظ���Ӧ����10��Ҳ������Ʋ���Ϊ10��
//		LayerNum	�Ʋ���
//����ֵ��
//		0			��ȷ
//		-1			����	
extern "C" ALGORITHM_API int GetCloudInfo(double* Data, int DataNum, double RangeResolution, double* CloudBase, double* CloudTop, int& LayerNum);



//��    �ܣ��Իز��źŽ���Ԥ��������ȥ�롢ƽ��;
//���������
//		Signal		�ز��źţ�DataNum��unsigned long������
//		DataNum		���ݵ���
//���������
//		Data		Ԥ���������
//����ֵ��
//		0			��ȷ
//		-1			����	
extern "C" ALGORITHM_API int SignalPreprocess(unsigned long* Signal, int DataNum, double* Data);



//��    �ܣ���Ԥ������źŻ�ȡ���ܽ��ĺ���ɢ��ϵ��;
//���������
//		Data		Ԥ�����ز��źţ�Datanum��double������
//		DataNum		���ݵ���
//		RangeResolution ����ֱ��ʣ���λ����
//		RangeRef	�ο�����λ�ã���λ����
//		Wavelength	�ź�ͨ����������λ������
//		DepolarizeRatio ��ƫ��ȣ������٣�0��100��Χ�ڡ�Datanum��double������
//		Overlap		�������ӣ������٣�0��1��Χ��Datanum��double�����顣��������Ϊȫ��1
//���������
//		AerosolBackscatter	���ܽ�����ɢ��ϵ��
//����ֵ��
//		0			��ȷ
//		-1			����	
extern "C" ALGORITHM_API int GetBackscatter(double* Data, int DataNum, double RangeResolution, double RangeRef, double Wavelength, double* DepolarizeRatio, double* Overlap, double* AerosolBackscatter);


//��    �ܣ������ܽ��ĺ���ɢ��ϵ����ȡ����ϵ��;
//���������
//		AerosolBackscatter	���ܽ�����ɢ��ϵ����Datanum��double������
//		DataNum		���ݵ���
//���������
//		AerosolExtinction	���ܽ�����ϵ��
//����ֵ��
//		0			��ȷ
//		-1			����	
extern "C" ALGORITHM_API int GetExtinction(double* AerosolBackscatter, int DataNum, double* AerosolExtinction);

//��    �ܣ�ƽ��
extern "C" ALGORITHM_API int Smooth(double* Data, int DataNum, int Span);

//��    �ܣ���ֵ
extern "C" ALGORITHM_API int Interp(double* pX, double* pY, int PointNum, double x, double& y);



