// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LICELACQ_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LICELACQ_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifdef LICELACQ_EXPORTS
#define LICELACQ_API __declspec(dllexport)
#else
#define LICELACQ_API __declspec(dllimport)
#endif

#define WM_CAPTUREEVENT WM_USER+0x02

extern "C" __declspec(dllexport) int LicelAcq_Start(unsigned int acccount,unsigned int curvepoint,HWND hMsgWnd);
extern "C" __declspec(dllexport) bool LicelAcq_GetData(double** pData,int arraysize);
extern "C" __declspec(dllexport) void LicelAcq_Stop();