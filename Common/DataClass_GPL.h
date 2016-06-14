/*
 * TaiFexFutureTick_GPL
 *
 * Authors:	WLDTW2008 <wldtw2008@googlemail.com>
 *              Please send support related email to: wldtw2008@googlemail.com
 *              Home Page: http://wldtw2008.at.3322.org
 *
 * Fixes:
 *      Fix xxxx - yourname <yourmail@yourmail.com>;
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Please read the file 'COPYING���v�ŧi.txt' for full GPL License.
 *
 * ���{���X�ĥ� GNU ��´�� GPL���}���v�ŧi�A�w��ƻs�B��g�B���G�A�ín�D�A���ӥH�P�˪�
 * �v�Q���ڡA�¤�����n��}�o�ϥΪ̡C�� GPL ���������A�Ьd�\ 'COPYING���v�ŧi.txt'
 * �γX��: http://www.linux.org.tw/CLDP/OLD/doc/GPL.html
 */
#if !defined(AFX_DATACLASS_H__INCLUDED_)
#define AFX_DATACLASS_H__INCLUDED_

#include <iostream>
#include <vector>
using namespace std;

#define VERSION_STR "v1.1.15"

static char gs_szLicenseNote[1024];
static char* pszMakeLicenseNote()
{
	//�s�@���v�ŧi
    sprintf(gs_szLicenseNote, 
			" =============================================================\r\n"
            " ���{������ %s �s�@���: %s\r\n"
            " \r\n"
            " ���v�ŧi:\r\n"
			" ���{���X�ĥ� GNU ��´�� GPL���}���v�ŧi�A�w��ƻs�B��g�B���G�A\r\n"
            " �ín�D�A���ӥH�P�˪��v�Q���ڡA�¤�����n��}�o�ϥΪ̡C\r\n"
            " �� GPL ���������A�Ьd�\ 'COPYING���v�ŧi.txt'\r\n"
            " �γX��: http://www.linux.org.tw/CLDP/OLD/doc/GPL.html\r\n"
            " \r\n"
            " ���{�����@�̤δ����̤��t�����O�d���A�ϥΪ̭Y�]�ϥΥ��{���l�ͪ�\r\n"
            " ����Φ����l�����ݦۦ�t�d(�]�A��������: �y���z�q���l�a�B��ƿ�\r\n"
            " �Φ]�ϥΥ��{���l�ͤ����l����)�C\r\n"
            " \r\n"
            " �̪쪩�@��: WLDTW2008 <wldtw2008@googlemail.com>\r\n"
			"             �Цܺ���: http://wldtw2008.at.3322.org\r\n"
            " \r\n"
			" =============================================================\r\n"
            , VERSION_STR, __DATE__);

	return gs_szLicenseNote;
}
static int iTradeTime2DaySec(int iTradeTime)
{
    //13450000
    return (iTradeTime/1000000)*3600 + ((iTradeTime/10000)%100)*60 + ((iTradeTime/100)%100);
}
unsigned int iTradeDateTime2Time_T(int iTradeDate, int iTradeTime);//���otime_t

enum _BAR_INTERVAL_ENUM
{
	BI_MINUTES = 0,
	BI_DAILY = 1,
	BI_TICKS = 2,
	BI_SECONDS = 3,
	BI_WEEKLY = 4,
	BI_MONTHLY = 5
};

class CBarData
{
public:
	int iTradeTime;
	double dbOpen;
	double dbHigh;
	double dbLow;
	double dbClose;
	double dbVol;
	CBarData();
    void vClear();
	void vSetValueByBar(CBarData& BarData);
	void vSetValueByTick(double& dbPrice, double& dbTickVol);
    void vWriteToFilePt(int iTradeDate, FILE* fp, int iTextMode);
	bool bReadFromBinFilePt(FILE* pt);
    bool operator==(CBarData& RHS);
    bool operator>(CBarData& RHS);
    bool operator<(CBarData& RHS);
};

class CTaiFexTickInfo
{
public:
    void vClear();
    CTaiFexTickInfo();
    int iTradeTime; //HHMMSSmm
    double dbPrice;
    double dbVol;
    void vWriteToFilePt(int iTradeDate, FILE* fp, int iTextMode);
	bool bReadFromBinFilePt(FILE* fp);
    bool operator==(CTaiFexTickInfo& RHS);
    bool operator>(CTaiFexTickInfo& RHS);
    bool operator<(CTaiFexTickInfo& RHS);
};

//�� CTaiFexTickInfo �]�_��
class CTaiFexFeatureTick
{
public:
    CTaiFexFeatureTick();
    void vClear();
    int iTradeDate;
    char szSymbol[32]; //ie EXF
    int iMonth; //ie 200806
    std::vector<CTaiFexTickInfo> vecTickInfo;
};
typedef struct _TickDataFileStruct
{
	int iTradeTime;
	double dbPrice;
	double dbVol;
}TickDataFileStruct;

void vBuildNSecFromTick(int iMarketOpenTime, int iMarketCloseTime, std::vector<CTaiFexTickInfo>* pvecSource, std::vector<CBarData>* pvecBarInfo, int iNSec);
void vBuildNSecFromBar(int iMarketOpenTime, int iMarketCloseTime, std::vector<CBarData>* pvecSource, std::vector<CBarData>* pvecBarInfo, int iNSec);

bool bParseTaiFexFeatureTickData(char* pszData, int* piDate, 
                                 int* piTradeTime, 
                                 char* szSymbolID1, int* piMonth1, double* pdbPrice1, double* pdbVol1,
                                 char* szSymbolID2, int* piMonth2, double* pdbPrice2, double* pdbVol2);
typedef struct _BarDataFileStruct
{
	int iTradeTime;
	double dbOpen;
	double dbHigh;
	double dbLow;
	double dbClose;
	double dbVol;
}BarDataFileStruct;
int iGetAllFile(vector<CString>* pvecAllFiles, const CString& cszPath, const CString& cszFindFilter);

#define MARKET_OPEN_TIME (8*3600 + 45*60 + 0)
#define MARKET_CLOSE_TIME (13*3600 + 45*60 + 0)

#endif // !defined(AFX_TAIFEXFUTURETICK_GPL_H__A9A1A274_DA74_4484_8DB2_4A0B417B7A99__INCLUDED_)