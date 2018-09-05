#include "StdAfx.h"
#include "StandardFormatter.h"
#include <fstream>
#include <vector>

CStandardFormatter::CStandardFormatter(void)
{
}

CStandardFormatter::~CStandardFormatter(void)
{
}

// void CStandardFormatter::Serial2File(CString path,FormatType type,CDatRecord* pRecord,CRadarStation* pStation)
// {
//   path += _T("\\") + pStation->m_model + _T("_") + pStation->m_id;
//   if(!PathFileExists(path))
//   CreateDirectory(path,NULL);

//   CString formatString;
//   formatString.Append((LPWSTR)(m_FormatTypeString[type]));
//   path += _T("\\") + formatString;
//   if(!PathFileExists(path))
//   CreateDirectory(path,NULL);

//   CTime tCur = CTime::GetCurrentTime();
//   CString temp;
//   temp.Format(_T("\\%0004d年"),tCur.GetYear());
// 	path = path + temp;
// 	if(!PathFileExists(path))
// 		CreateDirectory(path,NULL);
	
// 	temp.Format(_T("\\%02d月"),tCur.GetYear(),tCur.GetMonth());
// 	path = path + temp;
// 	if(!PathFileExists(path))
// 		CreateDirectory(path,NULL);
	
// 	temp.Format(_T("\\%02d日"),tCur.GetYear(),tCur.GetMonth(),tCur.GetDay());
// 	path = path + temp;
// 	if(!PathFileExists(path))
// 		CreateDirectory(path,NULL);

//   temp.Format(_T("%0004d%02d%02d%02d%02d%02d"),
//                   tCur.GetYear(),tCur.GetMonth(),tCur.GetDay(),tCur.GetHour(),tCur.GetMinute(),tCur.GetSecord());
//   CString filename = pStation->m_model + _T("_") 
//                       + pStation->m_id + _T("_") 
//                       + formatStr + _T("_") 
//                       + temp
//                       + _T(".bin");
//   std::ofstream f;
// 	f.open(file,std::ios_base::binary);
//   if(f.is_bad())
//   {

//   }

//   unsigned char *pBuf = new unsigned char[StandardFormat_Size];
//   delete[] pBuf;
// }

void CStandardFormatter::SerialHeader(unsigned char* pBuf,CDatRecord* pRecord)
{
  // 保留
  memset(pBuf,0x00,14);

  // 记录类型
  pBuf[14] = (pRecord->m_Type & 0x0000ffff) >> 8;
  pBuf[15] = (pRecord->m_Type & 0x000000ff);

  // 格式版本号
  pBuf[16] = (StandardFormat_Version & 0x0000ffff) >> 8;
  pBuf[17] = (StandardFormat_Version & 0x000000ff);
}

void CStandardFormatter::SerialStationInfo(unsigned char* pBuf,CRadarStation* pStation)
{
  // 区站号

  // 经度
  unsigned short lon = pStation->m_position.longitude / 8.0 * (180.0 / 4096.0);
  pBuf[22] = (lon & 0x0000ffff) >> 8;
  pBuf[23] = (lon & 0x000000ff);

  // 纬度
  unsigned short lat = pStation->m_position.latitude / 8.0 * (180.0 / 4096.0);
  pBuf[24] = (lat & 0x0000ffff) >> 8;
  pBuf[25] = (lat & 0x000000ff);

  // 海拔高度
  unsigned short alt = pStation->m_position.altitude;
  pBuf[26] = (alt & 0x0000ffff) >> 8;
  pBuf[27] = (alt & 0x000000ff);

  // 发射波长
  memset(pBuf+46,0x00,6);
  unsigned int wave_count = pStation->m_waves.size();
  for(unsigned int i=0;i<min(3,wave_count);i++)
  {
    pBuf[46+i*2] = (pStation->m_waves[i] & 0x0000ffff) >> 8;
    pBuf[47+i*2] = (pStation->m_waves[i] & 0x000000ff);
  }

  // 接收通道数
  pBuf[52] = (pStation->m_chCount & 0x0000ffff) >> 8;
  pBuf[53] = (pStation->m_chCount & 0x000000ff);
}

void CStandardFormatter::SerialRecordInfo(unsigned char* pBuf,CRecordProperty* pRecordProperty)
{
  // 探测模式
  pBuf[30] = (pRecordProperty->m_ModeType & 0xff00) >> 8;
  pBuf[31] = pRecordProperty->m_ModeType & 0x00ff;

  //数据开始收集时间
  CTimeSpan tSpan;
  CTime ts(pRecordProperty->m_sTime.GetYear(),pRecordProperty->m_sTime.GetMonth(),pRecordProperty->m_sTime.GetDay(),0,0,0);
  tSpan = pRecordProperty->m_sTime - ts;
  unsigned int totalSeconds = tSpan.GetTotalSeconds();
  pBuf[32] = (totalSeconds & 0xff000000) >> 24;
  pBuf[33] = (totalSeconds & 0x00ff0000) >> 16;
  pBuf[34] = (totalSeconds & 0x0000ff00) >> 8;
  pBuf[35] = (totalSeconds & 0x000000ff);

  //数据结束收集时间
  tSpan = pRecordProperty->m_eTime - ts;
  totalSeconds = tSpan.GetTotalSeconds();
  pBuf[36] = (totalSeconds & 0xff000000) >> 24;
  pBuf[37] = (totalSeconds & 0x00ff0000) >> 16;
  pBuf[38] = (totalSeconds & 0x0000ff00) >> 8;
  pBuf[39] = (totalSeconds & 0x000000ff);

  //儒略日
  CTime tGreely(1970,1,1,0,0,0);
  tSpan = pRecordProperty->m_sTime - tGreely;
  unsigned short totalDays = tSpan.GetDays();
  pBuf[40] = (totalDays & 0x0000ffff) >> 8;
  pBuf[41] = (totalDays & 0x000000ff);

  // 仰角
  unsigned short fy = pRecordProperty->m_FyAngle / 8.0 * (180.0 / 4096.0);
  pBuf[42] = (fy & 0x0000ffff) >> 8;
  pBuf[43] = (fy & 0x000000ff);

  // 方位角
  unsigned short fw = pRecordProperty->m_FwAngle / 8.0 * (180.0 / 4096.0);
  pBuf[44] = (fw & 0x0000ffff) >> 8;
  pBuf[45] = (fw & 0x000000ff);
}

void CStandardFormatter::SerialChannelInfo(unsigned char* pBuf,std::vector<CChannel *>& channels)
{
  std::vector<CChannel *>::iterator it = channels.begin();
  unsigned int idx = 54;
  unsigned int i=0;
  while(it != channels.end())
  {
    // 通道号标示
    pBuf[idx + i * 16] =  ((i+1) & 0xff00) >> 8;
    pBuf[idx + i * 16 + 1] = (i+1) & 0x00ff;

    // 采集方式
    pBuf[idx + i * 16 + 2] = ((*it)->m_pChannelProperty->m_acqType & 0xff00) >> 8;
    pBuf[idx + i * 16 + 3] = (*it)->m_pChannelProperty->m_acqType & 0x00ff;

    // 回波信号类型
    pBuf[idx + i * 16 + 4] = ((*it)->m_pChannelProperty->m_waveType & 0xff00) >> 8;
    pBuf[idx + i * 16 + 5] = (*it)->m_pChannelProperty->m_waveType & 0x00ff;

    // 距离分辨率
    pBuf[idx + i * 16 + 6] = ((unsigned short)(((*it)->m_pChannelProperty->m_distResolution * 100)) & 0xff00) >> 8;
    pBuf[idx + i * 16 + 7] = ((unsigned short)((*it)->m_pChannelProperty->m_distResolution * 100)) & 0x00ff;

    // 盲区高度
    pBuf[idx + i * 16 + 8] = ((unsigned short)(((*it)->m_pChannelProperty->m_deadZone * 10)) & 0xff00) >> 8;
    pBuf[idx + i * 16 + 9] = ((unsigned short)((*it)->m_pChannelProperty->m_deadZone * 10)) & 0x00ff;

    // 通道数据指针
    unsigned int chDataPos = 311 + i * 32000;
    pBuf[idx + i * 16 + 10] = (chDataPos & 0xff000000) >> 24;
    pBuf[idx + i * 16 + 11] = (chDataPos & 0x00ff0000) >> 16;
    pBuf[idx + i * 16 + 12] = (chDataPos & 0x0000ff00) >> 8;
    pBuf[idx + i * 16 + 13] = (chDataPos & 0x000000ff);

    // 通道距离库数
    pBuf[idx + i * 16 + 14] = ((*it)->m_pChannelProperty->m_distCount & 0xff00) >> 8;
    pBuf[idx + i * 16 + 15] = (*it)->m_pChannelProperty->m_distCount & 0x00ff;

    it++;
	i++;
  }
}

void CStandardFormatter::SerialChannelSample(unsigned char* pBuf,std::vector<CChannel *>& channels)
{
  unsigned int chDataPos,i=0;
  std::vector<CChannel *>::iterator it = channels.begin();
  while(it != channels.end())
  {
    chDataPos = 311 + i * 32000;

    CSample* pSample = (*it)->m_pSample;
    for(unsigned int j=0; j<(*it)->m_pChannelProperty->m_distCount; j++)
      memcpy(pBuf + chDataPos - 1 + j * 4,(unsigned char*)&(pSample->m_pData[0][j]),4);

    it++;
    i++;
  }
}

int CStandardFormatter::ReserialHeader(unsigned char* pBuf,CDatRecord* pRecord)
{
  unsigned short usTemp;

  // 记录类型
  usTemp = pBuf[14];
  usTemp = (usTemp << 8);
  usTemp += pBuf[15];
  if(usTemp != (CDatRecord::RecordType)usTemp)
    return 15;
  pRecord->m_Type = (CDatRecord::RecordType)usTemp;

  // 格式版本号
  usTemp = pBuf[16];
  usTemp = (usTemp << 8);
  usTemp += pBuf[17];
  if(usTemp != StandardFormat_Version)
    return 17;
 
  return 0;
}

int CStandardFormatter::ReserialStationInfo(unsigned char* pBuf,CRadarStation* pStation)
{
  unsigned short usTemp;
  double fTemp;
  // 区站号

  // 经度
  usTemp = pBuf[22];
  usTemp = (usTemp << 8);
  usTemp += pBuf[23];
  fTemp = ((double)usTemp) / (180.0 / 4096.0) * 8.0;
  if(fTemp > 180.0 || fTemp < -180.0)
    return 23;
  pStation->m_position.longitude = fTemp;

  // 纬度
  usTemp = pBuf[24];
  usTemp = (usTemp << 8);
  usTemp += pBuf[25];
  fTemp = ((double)usTemp) / (180.0 / 4096.0) * 8.0;
  if(fTemp > 90.0 || fTemp < -90.0)
    return 25;
  pStation->m_position.latitude = fTemp;

  // 海拔高度
  usTemp = pBuf[26];
  usTemp = (usTemp << 8);
  usTemp += pBuf[27];
  pStation->m_position.altitude = usTemp;

  // 发射波长
  memset(pBuf+46,0x00,6);
  unsigned int wave_count = pStation->m_waves.size();
  for(unsigned int i=0;i<3;i++)
  {
    usTemp = pBuf[46+i*2];
    usTemp = (usTemp << 8);
    usTemp += pBuf[47+i*2];
    if(usTemp == 0)
      return 46+i*2 + 1;

    pStation->m_waves.push_back(usTemp);
  }

  // 接收通道数
  usTemp = pBuf[52];
  usTemp = (usTemp << 8);
  usTemp += pBuf[53];
  if(usTemp == 0)
    return 53;
  pStation->m_chCount = usTemp;

  return 0;
}

int CStandardFormatter::ReserialRecordInfo(unsigned char* pBuf,CRecordProperty* pRecordProperty)
{
  unsigned short usTemp;
  unsigned int iTemp,iTempc;
  double fTemp;

  // 探测模式
  usTemp = pBuf[30];
  usTemp = (usTemp << 8);
  usTemp += pBuf[31];
  if(usTemp != (CRecordProperty::ModeType)usTemp)
    return 31;
  pRecordProperty->m_ModeType = (CRecordProperty::ModeType)usTemp;

  //儒略日
  unsigned short totalDays;
  totalDays = pBuf[30];
  totalDays = (totalDays << 8);
  totalDays += pBuf[31];

  //数据开始收集时间
  iTemp = pBuf[35];
  iTempc = pBuf[34];
  iTempc = (iTempc << 8);
  iTemp += iTempc;
  iTempc = pBuf[33];
  iTempc = (iTempc << 16);
  iTemp += iTempc;
  iTempc = pBuf[32];
  iTempc = (iTempc << 24);
  iTemp += iTempc;
  CTime t(1970,1,1,0,0,0);
  CTimeSpan tSpanD(totalDays,0,0,0);
  CTimeSpan tSpanS = (CTimeSpan)iTemp;
  pRecordProperty->m_sTime = t + tSpanD + tSpanS;

  //数据结束收集时间
  iTemp = pBuf[39];
  iTempc = pBuf[38];
  iTempc = (iTempc << 8);
  iTemp += iTempc;
  iTempc = pBuf[37];
  iTempc = (iTempc << 16);
  iTemp += iTempc;
  iTempc = pBuf[36];
  iTempc = (iTempc << 24);
  iTemp += iTempc;
  CTimeSpan tSpanE = (CTimeSpan)iTemp;
  pRecordProperty->m_eTime = t + tSpanD + tSpanE;

  // 仰角
  usTemp = pBuf[42];
  usTemp = (usTemp << 8);
  usTemp += pBuf[43];
  fTemp = ((double)usTemp) / (180.0 / 4096.0) * 8.0;
  if(fTemp > 90.0 || fTemp < -90.0)
    return 43;
  pRecordProperty->m_FyAngle = fTemp;

  // 方位角
  usTemp = pBuf[44];
  usTemp = (usTemp << 8);
  usTemp += pBuf[45];
  fTemp = ((double)usTemp) / (180.0 / 4096.0) * 8.0;
  if(fTemp > 360.0 || fTemp < 0.0)
    return 45;
  pRecordProperty->m_FwAngle = fTemp;

  return 0;
}

int CStandardFormatter::ReserialChannelInfo(unsigned char* pBuf,unsigned short chCount,std::vector<CChannel *>& channels)
{
  unsigned short usTemp;
  unsigned int idx = 54;
  for(int i = 0; i < chCount; i++)
  {
    CChannelProperty* pChannelProperty = new CChannelProperty();
    // 通道号标示
    usTemp = pBuf[idx + i*16];
    usTemp = (usTemp << 8);
    usTemp += pBuf[idx + i * 16 + 1];
    pChannelProperty->m_id = usTemp;

    // 采集方式
    usTemp = pBuf[idx + i*16 + 2];
    usTemp = (usTemp << 8);
    usTemp += pBuf[idx + i * 16 + 3];
    if(usTemp != (CChannelProperty::AcqType)usTemp)
      return idx + i*16 + 3;
    pChannelProperty->m_acqType = (CChannelProperty::AcqType)usTemp;

    // 回波信号类型
    usTemp = pBuf[idx + i*16 + 4];
    usTemp = (usTemp << 8);
    usTemp += pBuf[idx + i * 16 + 5];
    if(usTemp != (CChannelProperty::WaveType)usTemp)
      return idx + i*16 + 5;
    pChannelProperty->m_waveType = (CChannelProperty::WaveType)usTemp;

    // 距离分辨率
    usTemp = pBuf[idx + i*16 + 6];
    usTemp = (usTemp << 8);
    usTemp += pBuf[idx + i * 16 + 7];
    pChannelProperty->m_distResolution = ((double)usTemp) / 100.0;

    // 盲区高度
    usTemp = pBuf[idx + i*16 + 8];
    usTemp = (usTemp << 8);
    usTemp += pBuf[idx + i * 16 + 9];
    pChannelProperty->m_deadZone = ((double)usTemp) / 10.0;

    // // 通道数据指针
    // unsigned int chDataPos = 311 + i * 32000;
    // pBuf[idx + i * 16 + 10] = (chDataPos & 0xff000000) >> 24;
    // pBuf[idx + i * 16 + 11] = (chDataPos & 0x00ff0000) >> 16;
    // pBuf[idx + i * 16 + 12] = (chDataPos & 0x0000ff00) >> 8;
    // pBuf[idx + i * 16 + 13] = (chDataPos & 0x000000ff);

    // 通道距离库数
    usTemp = pBuf[idx + i*16 + 14];
    usTemp = (usTemp << 8);
    usTemp += pBuf[idx + i * 16 + 15];
    pChannelProperty->m_distCount = usTemp;

	CChannel *pChannel = new CChannel();
	pChannel->m_pChannelProperty = pChannelProperty;
    channels.push_back(pChannel);
  }

  return 0;
}

int CStandardFormatter::ReserialChannelSample(unsigned char* pBuf,std::vector<CChannel *>& channels)
{
  unsigned int chDataPos,distCount;
  std::vector<CChannel *>::iterator it = channels.begin();

  while(it != channels.end())
  {
    chDataPos = 311 + ((*it)->m_pChannelProperty->m_id - 1) * 32000;
    distCount = (*it)->m_pChannelProperty->m_distCount;

    if((*it)->m_pSample == NULL)
      (*it)->m_pSample = new CSample(1,distCount,TRUE);

    for(unsigned int i=0; i<distCount; i++)
    {
      ASSERT(chDataPos + i * 4 + 4 <= StandardFormat_Size);
      memcpy(pBuf + chDataPos - 1 + i * 4,(unsigned char*)&((*it)->m_pSample->m_pData[0][i]),4);
    }

    it++;
  }
}
