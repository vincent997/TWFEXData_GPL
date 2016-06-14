/*
 * TaiFexFutureTick
 *
 * Authors:	WLDTW2008 <wldtw2008@googlemail.com>
 *              Please send support related email to: wldtw2008@googlemail.com
 *              TaiFexFutureTick Home Page: http://wldtw2008.at.3322.org
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

// TWFEXDataEdit_GPL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TWFEXDataEdit_GPL.h"
#include "../common/DataClass_GPL.h"
#include "../common/FileDir_GPL.h"

#include <io.h>
#include <vector>
#include <map>
#include <algorithm>




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

#define DF_NO_DATA "(null)"
CString cszGetParam(char* pszKey, int iShift, int argc, TCHAR* argv[])
{
    CString cszKey = pszKey;
    for (int i=0;i<argc-iShift;i++)
    {
        if (cszKey.CompareNoCase(argv[i]) == 0)
        {
            CString cszReturn = CString(argv[i+iShift]);
            cszReturn.MakeLower();
            return cszReturn;
        }
    }
    return CString(DF_NO_DATA);
}
void vLog(int iAlias, const char* cmd, ...)
{
    char szAlias[512];
    memset(szAlias, ' ', iAlias*2);
    szAlias[iAlias*2] = '\0';
    char* pszline = new char[1024];
    va_list args;                 //�ŧiargs
    va_start(args, cmd);          //�Ұ�args(���t�O����), �ĤG�ӰѼƬO���n�q���ӰѼƶ}�largs
    vsprintf(pszline, cmd, args);  //vsprintf  v�}�Y�����O�Φbargs
    va_end(args);                 //����args
    printf("%s%s\n", szAlias, pszline);
}

CString cszGetDataFilePathBySymbol(const int iRequestType, const char* pszComodifyName, const char* pszSymbol)
{
	CString cszReturn;
    char* pTypeRootDir = NULL;
    if (iRequestType == BI_TICKS)
    {
        pTypeRootDir = WLDTW_DATA_TICK_DIR;
        CreateDirectory(WLDTW_DATA_ROOT_DIR, NULL);
        CreateDirectory(pTypeRootDir, NULL);
		cszReturn.Format("%s\\%s", pTypeRootDir, pszComodifyName);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s", pTypeRootDir, pszComodifyName, pszSymbol);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s\\", pTypeRootDir, pszComodifyName, pszSymbol);
    }
	else if (iRequestType == BI_SECONDS)
    {
        pTypeRootDir = WLDTW_DATA_1SEC_DIR;
        CreateDirectory(WLDTW_DATA_ROOT_DIR, NULL);
        CreateDirectory(pTypeRootDir, NULL);
		cszReturn.Format("%s\\%s", pTypeRootDir, pszComodifyName);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s", pTypeRootDir, pszComodifyName, pszSymbol);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s\\", pTypeRootDir, pszComodifyName, pszSymbol);
    }
	else if (iRequestType == BI_MINUTES)
    {
        pTypeRootDir = WLDTW_DATA_1MIN_DIR;
        CreateDirectory(WLDTW_DATA_ROOT_DIR, NULL);
        CreateDirectory(pTypeRootDir, NULL);
		cszReturn.Format("%s\\%s", pTypeRootDir, pszComodifyName);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s", pTypeRootDir, pszComodifyName, pszSymbol);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s\\", pTypeRootDir, pszComodifyName, pszSymbol);
    }
	else if (iRequestType == BI_DAILY)
    {
        pTypeRootDir = WLDTW_DATA_DAILY_DIR;
        CreateDirectory(WLDTW_DATA_ROOT_DIR, NULL);
        CreateDirectory(pTypeRootDir, NULL);
		cszReturn.Format("%s\\%s", pTypeRootDir, pszComodifyName);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s", pTypeRootDir, pszComodifyName, pszSymbol);
        CreateDirectory(cszReturn, NULL);
		cszReturn.Format("%s\\%s\\%s\\", pTypeRootDir, pszComodifyName, pszSymbol);
    }

	return cszReturn;
}

CString cszGetFullFilenameByRuns(int iRuns, CString& cszCommodity, CString& cszSymbolID, CString& cszDateFileName)
{
    CString cszFullFileName;
    switch(iRuns)
    {
    case 0://ticks
        cszFullFileName.Format("%s%s", cszGetDataFilePathBySymbol(BI_TICKS, cszCommodity, cszSymbolID), cszDateFileName);
        break;
    case 1://secs
        cszFullFileName.Format("%s%s", cszGetDataFilePathBySymbol(BI_SECONDS, cszCommodity, cszSymbolID), cszDateFileName);
        break;
    case 2://mins
        cszFullFileName.Format("%s%s", cszGetDataFilePathBySymbol(BI_MINUTES, cszCommodity, cszSymbolID), cszDateFileName);
        break;
    case 3://daily
        cszFullFileName.Format("%s%s", cszGetDataFilePathBySymbol(BI_DAILY, cszCommodity, cszSymbolID), cszDateFileName);
        break;
    }
    return cszFullFileName;
}
int iSaveTickData(CString& cszFullFileName, CTaiFexFeatureTick* pCTaiFexFeatureTick, int iTradeDate)
{
    FILE* fp = fopen(cszFullFileName, "wb");
    if (fp == NULL)
    {
    	return -1;
    }
	for (int j=0;j<pCTaiFexFeatureTick->vecTickInfo.size();++j)
    {
		if (1)
        {
            pCTaiFexFeatureTick->vecTickInfo[j].vWriteToFilePt(iTradeDate, fp, 0);//bin
        }
        else
        {
            pCTaiFexFeatureTick->vecTickInfo[j].vWriteToFilePt(iTradeDate, fp, 1);//csv
        }
    }
    fclose(fp);
    return 1;
}
int iSaveNIntervalBar(CString& cszFullFileName, vector<CBarData>* pvecNIntervalBar, int iTradeDate)
{
    FILE* fp = fopen(cszFullFileName, "wb");
    if (fp == NULL)
    {
        return -1;
    }
	
    for (int j=0;j<pvecNIntervalBar->size();++j)
    {
		if (1)
        {
            (*pvecNIntervalBar)[j].vWriteToFilePt(iTradeDate, fp, 0);//bin
        }
        else
        {
            (*pvecNIntervalBar)[j].vWriteToFilePt(iTradeDate, fp, 1);//csv
        }
    }
    fclose(fp);
    return 1;
}
void fnDoDel(int iMarketOpenTime, int iMarketCloseTime, CString& cszCommodity, CString& cszSymbolID, int iDelDate1, int iDelDate2, int iDelTime1, int iDelTime2, int iDoTicks, int iDoSecs, int iDoMins, int iDoDailys)
{
    //���o�Ӱӫ~���Ҧ��ɮצC��
	vector<CString> vecAllFiles;
	int iTotalFiles;
    int iRuns = 0;

    CTaiFexFeatureTick TaiFexFeatureSourceTicks;
    CTaiFexTickInfo tempTaiFexTickInfo;

    vector<CBarData> vecSourceBar;
    vector<CBarData> vecNIntervalBar;
    CBarData tempBar;

    for (iRuns=0;iRuns<4;iRuns++)
    {
        iTotalFiles = 0;
        switch(iRuns)
        {
        case 0:
            if (iDoTicks == 1)
	        {
                vLog(0, "���b�B�z Ticks��:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_TICKS, cszCommodity, cszSymbolID), "*.tick");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        case 1:
	        if (iDoSecs == 1)
	        {
                vLog(0, "���b�B�z����:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_SECONDS, cszCommodity, cszSymbolID), "*.1sec");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        case 2:
	        if (iDoMins == 1)
	        {
                vLog(0, "���b�B�z����:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_MINUTES, cszCommodity, cszSymbolID), "*.1min");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        case 3:
	        if (iDoDailys == 1)
	        {
                vLog(0, "���b�B�z����:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_DAILY, cszCommodity, cszSymbolID), "*.daily");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        }

	    for (int iFileIndex = 0;iFileIndex<iTotalFiles;++iFileIndex)
	    {
            CString cszFullFileName;
            int iTradeDate = atoi(vecAllFiles[iFileIndex].Left(8));
            if (iDelDate1 > iTradeDate || iTradeDate > iDelDate2)
                continue;

            cszFullFileName = cszGetFullFilenameByRuns(iRuns, cszCommodity, cszSymbolID, vecAllFiles[iFileIndex]);

            vLog(1, "���b�B�z�ɮ�: %s", cszFullFileName);
            if (iDelTime1 == -1)//�S���]�w�ɶ� ���N�u���]�w��� ���� �����R��
            {
                vLog(1, "�R���ɮ�: %s", cszFullFileName);
                _unlink(cszFullFileName);
            }
            else if (iRuns == 3)//�p�G�O����
            {
                if (iDelTime1 == -1)//�S���]�w�ɶ�, ���N�����R��
                {
                    vLog(1, "�R���ɮ�: %s", cszFullFileName);
                    _unlink(cszFullFileName);
                }
                else//���M�N����Ƥ���
                {                    
                }
            }
            else
            {
			    FILE* fp = fopen(cszFullFileName, "rb");
			    if (fp == NULL)
                {
    				vLog(1, "�ɮ׶}�ҿ��~!");
                    continue;
                }

                //Ū��
                switch(iRuns)
                {
                case 0://ticks
                    TaiFexFeatureSourceTicks.vecTickInfo.clear();
			        while(tempTaiFexTickInfo.bReadFromBinFilePt(fp) == 1)
			        {
                        if (iDelTime1 != -1 && 
                            iDelTime1 <= tempTaiFexTickInfo.iTradeTime && tempTaiFexTickInfo.iTradeTime <= iDelTime2)
                            continue;
				        TaiFexFeatureSourceTicks.vecTickInfo.push_back(tempTaiFexTickInfo);
			        }
			        fclose(fp);
                    break;
                case 1://secs
                case 2://mins
                case 3://daily
                    vecSourceBar.clear();
			        while(tempBar.bReadFromBinFilePt(fp) == 1)
			        {
                        if (iDelTime1 != -1 && 
                            iDelTime1 <= tempBar.iTradeTime && tempBar.iTradeTime <= iDelTime2)
                            continue;
				        vecSourceBar.push_back(tempBar);
			        }
			        fclose(fp);
                    break;
                }

                if (0 == iRuns)//tick
                {
                    if (-1 == iSaveTickData(cszFullFileName, &TaiFexFeatureSourceTicks, iTradeDate))
                    {
    				    vLog(1, "�R����ƥX��!");
                        continue;
                    }
                }
                else
                {
                    vecNIntervalBar.clear();
                    if (1 == iRuns)//sec
        			    vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, 1);
                    else if (2 == iRuns)//min
        			    vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, 60);
                    else if (3 == iRuns)//day
        			    vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, iMarketCloseTime-iMarketOpenTime*10000);

                    if (-1 == iSaveNIntervalBar(cszFullFileName, &vecNIntervalBar, iTradeDate))
                    {
    				    vLog(1, "�R����ƥX��!");
                        continue;
                    }
                }
    			vLog(1, "�R����Ƨ���!");
            }
	    }
        switch(iRuns)
        {
        case 0:
            if (iDoTicks == 1)
	        {
                vLog(1, "�B�z Ticks�ɵ���.");
	        }
            break;
        case 1:
	        if (iDoSecs == 1)
	        {
                vLog(1, "�B�z���ɵ���.");
	        }
            break;
        case 2:
	        if (iDoMins == 1)
	        {
                vLog(1, "�B�z���ɵ���.");
	        }
            break;
        case 3:
	        if (iDoDailys == 1)
	        {
                vLog(1, "�B�z���ɵ���.");
	        }
            break;
        }
    }
}
void fnDoList(int iMarketOpenTime, int iMarketCloseTime, CString& cszCommodity, CString& cszSymbolID, int iDate1, int iDate2, int iTime1, int iTime2, int iDoTicks, int iDoSecs, int iDoMins, int iDoDailys)
{
    //���o�Ӱӫ~���Ҧ��ɮצC��
	vector<CString> vecAllFiles;
	int iTotalFiles;
    int iRuns = 0;

    CTaiFexFeatureTick TaiFexFeatureSourceTicks;
    CTaiFexTickInfo tempTaiFexTickInfo;

    vector<CBarData> vecSourceBar;
    vector<CBarData> vecNIntervalBar;
    CBarData tempBar;

    for (iRuns=0;iRuns<4;iRuns++)
    {
        iTotalFiles = 0;
        switch(iRuns)
        {
        case 0:
            if (iDoTicks == 1)
	        {
                vLog(0, "���b�B�z Ticks��:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_TICKS, cszCommodity, cszSymbolID), "*.tick");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        case 1:
	        if (iDoSecs == 1)
	        {
                vLog(0, "���b�B�z����:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_SECONDS, cszCommodity, cszSymbolID), "*.1sec");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        case 2:
	        if (iDoMins == 1)
	        {
                vLog(0, "���b�B�z����:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_MINUTES, cszCommodity, cszSymbolID), "*.1min");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        case 3:
	        if (iDoDailys == 1)
	        {
                vLog(0, "���b�B�z����:");
		        iTotalFiles = iGetAllFile(&vecAllFiles, cszGetDataFilePathBySymbol(BI_DAILY, cszCommodity, cszSymbolID), "*.daily");
                if (iTotalFiles != 0)
                {
                    vLog(1, "�ؿ��U�@�� %d���ɮ׫ݳB�z.", iTotalFiles);
                }
                else
                {
                    vLog(1, "�L�i�B�z���ɮ�.");
                    continue;
                }
	        }
            break;
        }

	    for (int iFileIndex = 0;iFileIndex<iTotalFiles;++iFileIndex)
	    {
            CString cszFullFileName;
            int iTradeDate = atoi(vecAllFiles[iFileIndex].Left(8));
            if (iDate1 > iTradeDate || iTradeDate > iDate2)
                continue;

            cszFullFileName = cszGetFullFilenameByRuns(iRuns, cszCommodity, cszSymbolID, vecAllFiles[iFileIndex]);

            vLog(1, "���b�B�z�ɮ�: %s", cszFullFileName);
			FILE* fp = fopen(cszFullFileName, "rb");
			if (fp == NULL)
            {
    			vLog(1, "�ɮ׶}�ҿ��~!");
                continue;
            }

            //Ū��
            switch(iRuns)
            {
            case 0://ticks
                TaiFexFeatureSourceTicks.vecTickInfo.clear();
			    while(tempTaiFexTickInfo.bReadFromBinFilePt(fp) == 1)
			    {
                    if (iTime1 == -1 ||
                        (iTime1 <= tempTaiFexTickInfo.iTradeTime && tempTaiFexTickInfo.iTradeTime <= iTime2))
                    {
    				    TaiFexFeatureSourceTicks.vecTickInfo.push_back(tempTaiFexTickInfo);
                    }
                    else
                    {
                        continue;
                    }
			    }
			    fclose(fp);
                break;
            case 1://secs
            case 2://mins
            case 3://daily
                vecSourceBar.clear();
			    while(tempBar.bReadFromBinFilePt(fp) == 1)
			    {
                    if (iRuns == 3)//����
                    {
				        vecSourceBar.push_back(tempBar);
                    }
                    else if (iTime1 == -1 ||
                             (iTime1 <= tempBar.iTradeTime && tempBar.iTradeTime <= iTime2))
                    {
				        vecSourceBar.push_back(tempBar);
                    }
                    else
                    {
                        continue;
                    }
			    }
			    fclose(fp);
                break;
            }

            //�C�L
            vLog(1, "Filename=%s", cszFullFileName); 
            switch(iRuns)
            {
            case 0://ticks
                vLog(1, "====Ticks List Begin===="); 
                break;
            case 1://secs
                vLog(1, "====Secs List Begin===="); 
                break;
            case 2://mins
                vLog(1, "====Mins List Begin===="); 
                break;
            case 3://daily
                vLog(1, "====Daily List Begin===="); 
                break;
            }

            vLog(1, "TradeDate=%d", iTradeDate); 
            int i=0;
            switch(iRuns)
            {
            case 0://ticks
                for (i=0;i<TaiFexFeatureSourceTicks.vecTickInfo.size();++i)
                {
                    int iTradeTime = TaiFexFeatureSourceTicks.vecTickInfo[i].iTradeTime;
                    vLog(1, "%04d-%02d-%02d, %02d:%02d:%02d, %f, %d", 
                         iTradeDate/10000, iTradeDate/100%100, iTradeDate%100,
                         iTradeTime/1000000, iTradeTime/10000%100, iTradeTime/100%100,
                         TaiFexFeatureSourceTicks.vecTickInfo[i].dbPrice, TaiFexFeatureSourceTicks.vecTickInfo[i].dbVol);
                }
                break;
            case 1://secs
            case 2://mins
            case 3://daily
                for (i=0;i<vecSourceBar.size();++i)
                {
                    int iTradeTime = vecSourceBar[i].iTradeTime;
                    vLog(1, "%04d-%02d-%02d, %02d:%02d:%02d, %f, %f, %f, %f, %d", 
                         iTradeDate/10000, iTradeDate/100%100, iTradeDate%100,
                         iTradeTime/1000000, iTradeTime/10000%100, iTradeTime/100%100,
                         vecSourceBar[i].dbOpen, vecSourceBar[i].dbHigh, vecSourceBar[i].dbLow, vecSourceBar[i].dbClose, vecSourceBar[i].dbVol);
                }
                break;
            }
            vLog(1, "====List End===="); 

        }
    }
}

int iGetStrCharCount(char*pszCSVDate, int iLen, char cChar)
{
    int iCount = 0;
    int i=0;
    for (i=0;i<iLen;++i)
    {
        if (pszCSVDate[i] == cChar)
        {
            iCount++;
        }
    }
    return iCount;
}
int iGetTradeDateByCSV(char* pszCSVDate)
{
    int iStrLen = strlen(pszCSVDate);
    int iCount = 0;
    int iTradeDate = -1;
    if (iStrLen == 0)
        return iTradeDate;
    //���-��
    if (iGetStrCharCount(pszCSVDate, iStrLen, '-') == 2 &&
        pszCSVDate[0] != '-' && pszCSVDate[iStrLen-1] != '-')
    {
        char* psz1 = pszCSVDate;
        char* psz2 = strstr(psz1, "-");
        char* psz3 = strstr(psz2+1, "-");
        *psz2 = '\0';
        psz2++;
        *psz3 = '\0';
        psz3++;
        iTradeDate = atoi(psz1)*10000 + atoi(psz2)*100 + atoi(psz3);
    }
    return iTradeDate;
}
int iGetTradeTimeByCSV(char* pszCSVDate)
{
    int iStrLen = strlen(pszCSVDate);
    int iCount = 0;
    int iTradeTime = -1;
    if (iStrLen == 0)
        return iTradeTime;
    //���:��
    if (iGetStrCharCount(pszCSVDate, iStrLen, ':') == 2 &&
        pszCSVDate[0] != ':' && pszCSVDate[iStrLen-1] != ':')
    {
        char* psz1 = pszCSVDate;
        char* psz2 = strstr(psz1, ":");
        char* psz3 = strstr(psz2+1, ":");
        *psz2 = '\0';
        psz2++;
        *psz3 = '\0';
        psz3++;
        iTradeTime = atoi(psz1)*1000000 + atoi(psz2)*10000 + atoi(psz3)*100;
    }
    return iTradeTime;
}

void vModifyTradeDateIfCrossDay(int* piTradeDate, int* piTradeTime, int iMarketOpenTime, int iMarketCloseTime)
{
    if (iMarketOpenTime < iMarketCloseTime)
        return;//�D���ӫ~

    if (iTradeTime2DaySec(*piTradeTime) > iMarketOpenTime)
        return;//���ӫ~ ���O�����

    //�ŦX���� ����N�n�N��������1
    unsigned int iTimeT = iTradeDateTime2Time_T(*piTradeDate, *piTradeTime);
    iTimeT -= 86400;//����

    struct tm* tmNow = localtime((const long*)&iTimeT);
    //���s�]�w�ӫ~������ �M�ɶ�
    *piTradeDate = (tmNow->tm_year+1900)*10000 + (tmNow->tm_mon+1)*100 + tmNow->tm_mday;
    *piTradeTime = tmNow->tm_hour*1000000 + tmNow->tm_min*10000 + tmNow->tm_sec*100 + 24000000;
}

typedef struct _VEC_BAR{
    std::vector<CBarData> vecBar;
}VEC_BAR;
void fnDoAddBar(int iMarketOpenTime, int iMarketCloseTime, CString& cszCommodity, CString& cszSymbolID, int iFileType, CString& cszFileNameOrData, 
                int iAddDate1, int iAddDate2, int iAddTime1, int iAddTime2, 
                int iDoTicks, int iDoSecs, int iDoMins, int iDoDailys)
{
    char szData[1024];
    FILE* fp = NULL;
    char* token = NULL;
    int iSplit = 0;
    std::map<int, VEC_BAR> mapBarVec;
    std::map<int, VEC_BAR>::iterator iter;

    CBarData tempBar;
    vector<CBarData> vecSourceBar;
    vector<CBarData> vecNIntervalBar;

    int iTradeDate;
    int iLogAlias = 0;
    if (iFileType == 1)
    {
        fp = fopen(cszFileNameOrData, "rb");
    }

    iLogAlias = 1;

    while(1)
    {
        if (iFileType == 1)//�ɮ�
        {
            if (NULL == fgets(szData, 1024, fp))
                break;
        }
        else// ��r��J
        {
            strcpy(szData, cszFileNameOrData);
        }

        iSplit = 0;
        iTradeDate = 0;
        tempBar.vClear();
        token = NULL;
        while(1)
        {
            if (token == NULL)//first
                token = strtok(szData, ",");
            else
                token = strtok(NULL, ",");

            if (token == NULL)
                break;

            switch(iSplit)
            {
            case 0://date
                iTradeDate = iGetTradeDateByCSV(token);
                iSplit++;
                if (iTradeDate == -1)
                    iSplit = -1;
                break;
            case 1://time
                tempBar.iTradeTime = iGetTradeTimeByCSV(token);
                iSplit++;
                if (tempBar.iTradeTime == -1)
                    iSplit = -1;
                break;
            case 2://O
                tempBar.dbOpen = atof(token);
                iSplit++;
                if (tempBar.dbOpen <= 0)
                    iSplit = -1;
                break;
            case 3://H
                tempBar.dbHigh = atof(token);
                iSplit++;
                if (tempBar.dbHigh <= 0)
                    iSplit = -1;
                break;
            case 4://L
                tempBar.dbLow = atof(token);
                iSplit++;
                if (tempBar.dbLow <= 0)
                    iSplit = -1;
                break;
            case 5://C
                tempBar.dbClose = atof(token);
                iSplit++;
                if (tempBar.dbClose <= 0)
                    iSplit = -1;
                break;
            case 6://V
                tempBar.dbVol = atof(token);
                iSplit++;
                if (tempBar.dbVol <= 0)
                    iSplit = -1;
                break;
            }
            if (iSplit == -1)//�X���F
                break;
            if (iSplit == 7)//���`!
            {
                int iAdd = 1;
                if (iAddDate1 != 0)
                {
                    if (iTradeDate <= iAddDate1 && iTradeDate <= iAddDate2)
                    {
                        //good
                    }
                    else
                    {
                        iAdd = 0;
                    }
                }
                if (iAddTime1 != -1)
                {                    
                    if (iAddTime1 <= tempBar.iTradeTime && tempBar.iTradeTime <= iAddTime2)
                    {
                        //good
                    }
                    else
                    {
                        iAdd = 0;
                    }
                }
                if (iAdd == 1)
                {
                    vModifyTradeDateIfCrossDay(&iTradeDate, &tempBar.iTradeTime, iMarketOpenTime, iMarketCloseTime);
                    mapBarVec[iTradeDate].vecBar.push_back(tempBar);
                }
                break;
            }
        }

        if (iFileType == 1)//�ɮ�
        {
            continue;
        }
        else// ��r��J
        {
            break;//��r�u�|���@��
        }
    }
    if (iFileType == 1)//�ɮ�
    {
        fclose(fp);
    }
    else// ��r��J
    {
        //do nothing
    }

    CString cszDateFileName;
    //��ƥ����� mapBarVec�F!
    vLog(iLogAlias, "�@���o�F %d�骺���.", mapBarVec.size());

    iter = mapBarVec.begin();
    iLogAlias = 2;
    while (iter != mapBarVec.end())
    {
        int iTradeDate = (iter->first);
        VEC_BAR* tempVecBar = &(iter->second);
        for (int iRuns=1;iRuns<4;++iRuns)
        {
            switch(iRuns)
            {
            case 1:
	            if (iDoSecs == 1)
	            {
                    cszDateFileName.Format("%d.1sec", iTradeDate);
                    vLog(iLogAlias, "���b�B�z����:");
	            }
                else
                    continue;
                break;
            case 2:
	            if (iDoMins == 1)
	            {
                    cszDateFileName.Format("%d.1min", iTradeDate);
                    vLog(iLogAlias, "���b�B�z����:");
	            }
                else
                    continue;
                break;
            case 3:
	            if (iDoDailys == 1)
	            {
                    cszDateFileName.Format("%d.daily", iTradeDate);
                    vLog(iLogAlias, "���b�B�z����:");
	            }
                else
                    continue;
                break;
            }

            CString cszFullFileName = cszGetFullFilenameByRuns(iRuns, cszCommodity, cszSymbolID, cszDateFileName);
            vLog(iLogAlias, cszFullFileName);
            vecSourceBar.clear();
			FILE* fp = fopen(cszFullFileName, "rb");
			if (fp != NULL)
            {
			    while(tempBar.bReadFromBinFilePt(fp) == 1)
			    {
				    vecSourceBar.push_back(tempBar);
			    }
			    fclose(fp);
            }

    		vLog(iLogAlias, "�즳 %d�����, �ǳƷs�W %d��.", vecSourceBar.size(), tempVecBar->vecBar.size());

            vecNIntervalBar.clear();
            for (int i=0;i<tempVecBar->vecBar.size();++i)
            {
                vecSourceBar.push_back(tempVecBar->vecBar[i]);
            }

            //std::sort(vecSourceBar.begin(), vecSourceBar.end());

            if (1 == iRuns)//sec
        		vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, 1);
            else if (2 == iRuns)//min
        		vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, 60);
            else if (3 == iRuns)//day
        		vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, iMarketCloseTime-iMarketOpenTime);

            if (-1 == iSaveNIntervalBar(cszFullFileName, &vecNIntervalBar, iTradeDate))
            {
    			vLog(iLogAlias, "�s�W��ƥX��!");
            }
            else
            {
            	vLog(iLogAlias, "�s�W��Ƨ���!");
            }
        }
        iter++;
    }

}
int iGetParamDate(CString& cszParam, int& iModifyDate1, int& iModifyDate2) 
{
    int iSplitPosi = cszParam.Find(":");
    if (iSplitPosi > 0)
    {
        iModifyDate1 = atoi(cszParam.Mid(0, iSplitPosi));
        iModifyDate2 = atoi(cszParam.Mid(iSplitPosi + 1, cszParam.GetLength() - iSplitPosi - 1));
        if (iModifyDate1 > iModifyDate2)
        {
            int iTemp = iModifyDate1;
            iModifyDate1 = iModifyDate2;
            iModifyDate2 = iTemp;
        }
    }
    else
    {
        iModifyDate1 = atoi(cszParam);
    }
    if (iModifyDate2 == 0 && iModifyDate1 != 0)
    {
        iModifyDate2 = iModifyDate1;
    }

    return 1;
}

int iGetParamTime(CString& cszParam, int& iModifyTime1, int& iModifyTime2) 
{
    int iSplitPosi = cszParam.Find(":");
    if (iSplitPosi > 0)
    {
        iModifyTime1 = atoi(cszParam.Mid(0, iSplitPosi));
        iModifyTime2 = atoi(cszParam.Mid(iSplitPosi + 1, cszParam.GetLength() - iSplitPosi - 1));
        if (iModifyTime1 > iModifyTime2)
        {
            int iTemp = iModifyTime1;
            iModifyTime1 = iModifyTime2;
            iModifyTime2 = iTemp;
        }
    }
    else
    {
        iModifyTime1 = atoi(cszParam);
    }
    if (iModifyTime2 == -1 && iModifyTime1 != -1)
    {
        iModifyTime2 = iModifyTime1;
    }

    return 1;
}
typedef struct _VEC_TICK
{
    vector<CTaiFexTickInfo> vecTick;
}VEC_TICK;
void fnDoAddTick(int iMarketOpenTime, int iMarketCloseTime, CString& cszCommodity, CString& cszSymbolID, int iFileType, CString& cszFileNameOrData, 
                 int iAddDate1, int iAddDate2, int iAddTime1, int iAddTime2, 
                 int iDoTicks, int iDoSecs, int iDoMins, int iDoDailys)
{
    char szData[1024];
    FILE* fp = NULL;
    char* token = NULL;
    int iSplit = 0;
    std::map<int, VEC_TICK> mapTickVec;
    std::map<int, VEC_TICK>::iterator iter;

    CTaiFexTickInfo tempTick;
    CBarData tempBar;
    CTaiFexFeatureTick TaiFexFeatureSourceTicks;
    vector<CBarData> vecSourceBar;
    vector<CBarData> vecNIntervalBar;

    int iTradeDate;
    int iLogAlias = 0;
    if (iFileType == 1)
    {
        fp = fopen(cszFileNameOrData, "rb");
    }

    iLogAlias = 1;

    while(1)
    {
        if (iFileType == 1)//�ɮ�
        {
            if (NULL == fgets(szData, 1024, fp))
                break;
        }
        else// ��r��J
        {
            strcpy(szData, cszFileNameOrData);
        }

        iSplit = 0;
        iTradeDate = 0;
        tempTick.vClear();
        while(1)
        {
            if (token == NULL)//first
                token = strtok(szData, ",");
            else
                token = strtok(NULL, ",");

            if (token == NULL)
                break;

            switch(iSplit)
            {
            case 0://date
                iTradeDate = iGetTradeDateByCSV(token);
                iSplit++;
                if (iTradeDate == -1)
                    iSplit = -1;
                break;
            case 1://time
                tempTick.iTradeTime = iGetTradeTimeByCSV(token);
                iSplit++;
                if (tempTick.iTradeTime == -1)
                    iSplit = -1;
                break;
            case 2://price
                tempTick.dbPrice = atof(token);
                iSplit++;
                if (tempTick.dbPrice <= 0)
                    iSplit = -1;
                break;
            case 3://V
                tempTick.dbVol = atof(token);
                iSplit++;
                if (tempTick.dbVol <= 0)
                    iSplit = -1;
                break;
            }
            if (iSplit == -1)//�X���F
                break;
            if (iSplit == 4)//���`!
            {
                int iAdd = 1;
                if (iAddDate1 != 0)
                {
                    if (iTradeDate <= iAddDate1 && iTradeDate <= iAddDate2)
                    {
                        //good
                    }
                    else
                    {
                        iAdd = 0;
                    }
                }
                if (iAddTime1 != -1)
                {                    
                    if (iAddTime1 <= tempTick.iTradeTime && tempTick.iTradeTime <= iAddTime2)
                    {
                        //good
                    }
                    else
                    {
                        iAdd = 0;
                    }
                }
                if (iAdd == 1)
                {
                    mapTickVec[iTradeDate].vecTick.push_back(tempTick);
                }
                break;
            }
        }

        if (iFileType == 1)//�ɮ�
        {
            continue;
        }
        else// ��r��J
        {
            break;//��r�u�|���@��
        }
    }
    if (iFileType == 1)//�ɮ�
    {
        fclose(fp);
    }
    else// ��r��J
    {
        //do nothing
    }

    CString cszDateFileName;
    //��ƥ����� mapBarVec�F!
    vLog(iLogAlias, "�@���o�F %d�骺���.", mapTickVec.size());

    iter = mapTickVec.begin();
    iLogAlias = 2;
    while (iter != mapTickVec.end())
    {
        int iTradeDate = (iter->first);
        VEC_TICK* tempVecTick = &(iter->second);
        for (int iRuns=0;iRuns<4;++iRuns)
        {
            switch(iRuns)
            {
            case 0:
	            if (iDoTicks == 1)
	            {
                    cszDateFileName.Format("%d.tick", iTradeDate);
                    vLog(iLogAlias, "���b�B�z Tick��:");
	            }
                else
                    continue;
                break;
            case 1:
	            if (iDoSecs == 1)
	            {
                    cszDateFileName.Format("%d.1sec", iTradeDate);
                    vLog(iLogAlias, "���b�B�z����:");
	            }
                else
                    continue;
                break;
            case 2:
	            if (iDoMins == 1)
	            {
                    cszDateFileName.Format("%d.1min", iTradeDate);
                    vLog(iLogAlias, "���b�B�z����:");
	            }
                else
                    continue;
                break;
            case 3:
	            if (iDoDailys == 1)
	            {
                    cszDateFileName.Format("%d.daily", iTradeDate);
                    vLog(iLogAlias, "���b�B�z����:");
	            }
                else
                    continue;
                break;
            }

            CString cszFullFileName = cszGetFullFilenameByRuns(iRuns, cszCommodity, cszSymbolID, cszDateFileName);
            vLog(iLogAlias, cszFullFileName);
            TaiFexFeatureSourceTicks.vClear();
            vecSourceBar.clear();
			FILE* fp = fopen(cszFullFileName, "rb");
			if (fp != NULL)
            {
                switch(iRuns)
                {
                case 0:
			        while(tempTick.bReadFromBinFilePt(fp) == 1)
			        {
                        TaiFexFeatureSourceTicks.vecTickInfo.push_back(tempTick);
			        }
                    break;
                case 1:
                case 2:
                case 3:
			        while(tempBar.bReadFromBinFilePt(fp) == 1)
			        {
				        vecSourceBar.push_back(tempBar);
			        }
                    break;
                }
    			fclose(fp);
            }

            vecNIntervalBar.clear();
            int i;
            switch(iRuns)
            {
            case 0:
            	vLog(iLogAlias, "�즳 %d�����, �ǳƷs�W %d��.", TaiFexFeatureSourceTicks.vecTickInfo.size(), tempVecTick->vecTick.size());
                for (i=0;i<tempVecTick->vecTick.size();++i)
                {
                    TaiFexFeatureSourceTicks.vecTickInfo.push_back(tempVecTick->vecTick[i]);
                }
                std::sort(TaiFexFeatureSourceTicks.vecTickInfo.begin(), TaiFexFeatureSourceTicks.vecTickInfo.end());
                if (-1 == iSaveTickData(cszFullFileName, &TaiFexFeatureSourceTicks, iTradeDate))
                {
    			    vLog(iLogAlias, "�s�W��ƥX��!");
                }
                else
                {
            	    vLog(iLogAlias, "�s�W��Ƨ���!");
                }
                break;
            case 1:
            case 2:
            case 3:
            	vLog(iLogAlias, "�즳 %d�����, �ǳƷs�W %d��.", vecSourceBar.size(), tempVecTick->vecTick.size());
                for (i=0;i<tempVecTick->vecTick.size();++i)
                {
                    tempBar.vClear();
                    tempBar.iTradeTime = tempVecTick->vecTick[i].iTradeTime;
                    tempBar.dbOpen = tempVecTick->vecTick[i].dbPrice;
                    tempBar.dbHigh = tempVecTick->vecTick[i].dbPrice;
                    tempBar.dbLow = tempVecTick->vecTick[i].dbPrice;
                    tempBar.dbClose = tempVecTick->vecTick[i].dbPrice;
                    tempBar.dbVol = tempVecTick->vecTick[i].dbVol;
                    vecSourceBar.push_back(tempBar);
                }
                if (1 == iRuns)//sec
        		    vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, 1);
                else if (2 == iRuns)//min
        		    vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, 60);
                else if (3 == iRuns)//day
        		    vBuildNSecFromBar(iMarketOpenTime, iMarketCloseTime, &vecSourceBar, &vecNIntervalBar, iMarketCloseTime-iMarketOpenTime);

                if (-1 == iSaveNIntervalBar(cszFullFileName, &vecNIntervalBar, iTradeDate))
                {
    			    vLog(iLogAlias, "�s�W��ƥX��!");
                }
                else
                {
            	    vLog(iLogAlias, "�s�W��Ƨ���!");
                }
                
                break;
            }
        }
        iter++;
    }

}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
        /*
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
        */
	}

	//���v�ŧi
	printf(pszMakeLicenseNote());
	printf("\n");

    int iAddMode = 0; // 1=Bar data, 2=Tick data
    int iDelMode = 0;
    int iListMode = 0;
/*
        printf("TWFEXDataEdit_GPL [-add csv CSVFILE] [-t1] [-s1] [-m1] [-d1] [-symbol SID] [-srv IP:PORT]\n");
        printf("TWFEXDataEdit_GPL [-del (date YYYYMMDD1:YYYYMMDD2)|(time HHMMSSmm1:HHMMSSmm2)] [-symbol SID] [-srv IP:PORT]\n");
*/
    int iModifyDate1 = 0;
    int iModifyDate2 = 0;
    int iModifyTime1 = -1;
    int iModifyTime2 = -1;
    int iDoTicks = 0;
    int iDoSecs = 0;
    int iDoMins = 0;
    int iDoDailys = 0;
    CString cszFileNameOrData;
    int iFileType = 0;// 0=Unknown , 1=CSV, 2=direct input
    CString cszCommodity;
    CString cszSymbolID;
    CString cszServerIPandPort;
    int iMarketOpenTime = 0;//�}�L�ɶ�
    int iMarketCloseTime = 0;//���L�ɶ�


    if (cszGetParam("-add", 0, argc, argv) != DF_NO_DATA)
    {
        CString cszTemp;
        //Bar �Ҧ�
        if (cszGetParam("-add", 1, argc, argv) == "b")
        {
            if (cszGetParam("-t1", 0, argc, argv) != DF_NO_DATA)
            {
                vLog(0, "�L�k�q Bar��ƼW�[ Tick, �Ш��� -t1�ﶵ��A��.");
                return 0;
            }
            if (cszGetParam("-add", 2, argc, argv) == "csv")
            {
                iAddMode = 1;
                iFileType = 1;
                cszFileNameOrData = cszGetParam("-add", 3, argc, argv);
            }
            else if(cszGetParam("-add", 2, argc, argv) == "data")
            {
                iAddMode = 1;
                iFileType = 2;
                cszFileNameOrData = cszGetParam("-add", 3, argc, argv);
            }
        }
        //Tick�Ҧ�
        else if (cszGetParam("-add", 1, argc, argv) == "t")
        {
            if (cszGetParam("-add", 2, argc, argv) == "csv")
            {
                iAddMode = 2;
                iFileType = 1;
                cszFileNameOrData = cszGetParam("-add", 3, argc, argv);
            }
            else if(cszGetParam("-add", 2, argc, argv) == "data")
            {
                iAddMode = 2;
                iFileType = 2;
                cszFileNameOrData = cszGetParam("-add", 3, argc, argv);
            }
        }

        if (cszGetParam("date", 0, argc, argv) != DF_NO_DATA)
        {
            cszTemp = cszGetParam("date", 1, argc, argv);
            iGetParamDate(cszTemp, iModifyDate1, iModifyDate2);
        }
        if (cszGetParam("time", 0, argc, argv) != DF_NO_DATA)
        {
            cszTemp = cszGetParam("time", 1, argc, argv);
            iGetParamTime(cszTemp, iModifyTime1, iModifyTime2);
        }

        vLog(0 ,"<< �W�[��ƼҦ� >>");
        if (iModifyTime1 != -1)
        {
            if (iModifyDate1 == iModifyDate2)
            {
                vLog(0, "�ǳƷs�W����ư϶�: ���:%d �ɶ�:%d ~ %d", iModifyDate1, iModifyTime1, iModifyTime2);
            }
            else
            {
                vLog(0 ,"�ǳƷs�W����ư϶�: ���:%d ~ %d �ɶ�:%d ~ %d", iModifyDate1, iModifyDate2, iModifyTime1, iModifyTime2);
            }
        }
        else if (iModifyDate1 != 0)
        {
            if (iModifyDate1 == iModifyDate2)
            {
                vLog(0, "�ǳƷs�W����ư϶�: ���:%d", iModifyDate1);
            }
            else
            {
                vLog(0, "�ǳƷs�W����ư϶�: ���:%d ~ %d", iModifyDate1, iModifyDate2);
            }
        }
        if (iFileType == 1)
        {
            vLog(1 ,"�ɦW: %s", cszFileNameOrData);

            if (_access(cszFileNameOrData, 4) != 0)
            {
                vLog(1, "�ɮפ��s�b�εL�kŪ��, �{���פ�!");
                return 0;
            }
        }
        else if (iFileType == 2)
        {
            vLog(1, "���: %s", cszFileNameOrData);
        }
    }
    else if (cszGetParam("-del", 0, argc, argv) != DF_NO_DATA &&
             (cszGetParam("-del", 1, argc, argv) == "date" || cszGetParam("-del", 1, argc, argv) == "time"))
    {
        vLog(0 ,"<< �R����ƼҦ� >>");
        CString cszTemp;
        if (cszGetParam("-del", 1, argc, argv) == "date")
        {
            cszTemp = cszGetParam("-del", 2, argc, argv);
            iGetParamDate(cszTemp, iModifyDate1, iModifyDate2);
        }
        if (cszGetParam("-del", 3, argc, argv) == "time")
        {
            cszTemp = cszGetParam("-del", 4, argc, argv);
            iGetParamTime(cszTemp, iModifyTime1, iModifyTime2);
        }

        if ((iModifyDate1 == 0 && iModifyTime1 == -1) ||
            (iModifyDate1 == 0 && iModifyTime1 != -1))
        {
            vLog(0, "����ήɶ��Ѽƿ��~, �{���פ�!");
            return 0;
        }
        else
        {
            iDelMode = 1;
            if (iModifyTime1 != -1)
            {
                if (iModifyDate1 == iModifyDate2)
                {
                    vLog(0, "�ǳƧR������ư϶�: ���:%d �ɶ�:%d ~ %d", iModifyDate1, iModifyTime1, iModifyTime2);
                }
                else
                {
                    vLog(0 ,"�ǳƧR������ư϶�: ���:%d ~ %d �ɶ�:%d ~ %d", iModifyDate1, iModifyDate2, iModifyTime1, iModifyTime2);
                }
            }
            else
            {
                if (iModifyDate1 == iModifyDate2)
                {
                    vLog(0, "�ǳƧR������ư϶�: ���:%d", iModifyDate1);
                }
                else
                {
                    vLog(0, "�ǳƧR������ư϶�: ���:%d ~ %d", iModifyDate1, iModifyDate2);
                }
            }
        }
    }
    //�C�L���
    else if (cszGetParam("-list", 0, argc, argv) != DF_NO_DATA &&
             cszGetParam("-list", 1, argc, argv) == "date")
    {
        vLog(0 ,"<< �C�L��ƼҦ� >>");
        CString cszTemp;
        if (cszGetParam("-list", 1, argc, argv) == "date")
        {
            cszTemp = cszGetParam("-list", 2, argc, argv);
            iGetParamDate(cszTemp, iModifyDate1, iModifyDate2);
        }
        if (cszGetParam("-list", 3, argc, argv) == "time")
        {
            cszTemp = cszGetParam("-list", 4, argc, argv);
            iGetParamTime(cszTemp, iModifyTime1, iModifyTime2);
        }

        if ((iModifyDate1 == 0 && iModifyTime1 == -1) ||
            (iModifyDate1 == 0 && iModifyTime1 != -1))
        {
            vLog(0, "����ήɶ��Ѽƿ��~, �{���פ�!");
            return 0;
        }
        else
        {
            iListMode = 1;
            if (iModifyTime1 != -1)
            {
                if (iModifyDate1 == iModifyDate2)
                {
                    vLog(0, "�ǳƦC�L����ư϶�: ���:%d �ɶ�:%d ~ %d", iModifyDate1, iModifyTime1, iModifyTime2);
                }
                else
                {
                    vLog(0 ,"�ǳƦC�L����ư϶�: ���:%d ~ %d �ɶ�:%d ~ %d", iModifyDate1, iModifyDate2, iModifyTime1, iModifyTime2);
                }
            }
            else
            {
                if (iModifyDate1 == iModifyDate2)
                {
                    vLog(0, "�ǳƦC�L����ư϶�: ���:%d", iModifyDate1);
                }
                else
                {
                    vLog(0, "�ǳƦC�L����ư϶�: ���:%d ~ %d", iModifyDate1, iModifyDate2);
                }
            }
        }
        iMarketOpenTime = 1;//�}�L�ɶ� �H�K�]�@��
        iMarketCloseTime = 1;//���L�ɶ� �H�K�]�@��
    }
    cszCommodity = cszGetParam("-cid", 1, argc, argv);
    cszCommodity.MakeUpper();
    cszSymbolID = cszGetParam("-sid", 1, argc, argv);
    cszSymbolID.MakeUpper();

    if (cszGetParam("-mt", 1, argc, argv) != DF_NO_DATA)
    {
        CString cszTemp = cszGetParam("-mt", 1, argc, argv);
        int iSplitPosi = cszTemp.Find(":");
        if (iSplitPosi > 0)
        {
            iMarketOpenTime = atoi(cszTemp.Mid(0, iSplitPosi));//�}�L�ɶ�
            iMarketOpenTime = iTradeTime2DaySec(10000 * iMarketOpenTime);//�ন�����
            iMarketCloseTime = atoi(cszTemp.Mid(iSplitPosi + 1, cszTemp.GetLength() - iSplitPosi - 1));//���L�ɶ�
            iMarketCloseTime = iTradeTime2DaySec(10000 * iMarketCloseTime);//�ন�����
        }
    }

    if ((iAddMode == 0 && iDelMode == 0 && iListMode == 0) || 
        iMarketOpenTime == 0 ||
        cszSymbolID == DF_NO_DATA ||
        cszCommodity == DF_NO_DATA)
    {
        printf("�ϥλ���:\n");
        printf("�s�W���: -add b|t csv|data INPUT [date YYYYMMDD1[:YYYYMMDD2]] [time HHMM1:HHMM2]\n");
        printf("          -cid CID -id SID -mt HHMM1:HHMM2 \n");
        printf("          [-t1] [-s1] [-m1] [-d1] [-srv IP:PORT]\n");
        printf("�R�����: -del date YYYYMMDD1[:YYYYMMDD2] [time HHMM1:HHMM2] \n");
        printf("          -cid CID -sid SID  -mt HHMM1:HHMM2 \n");
        printf("          [-t1] [-s1] [-m1] [-d1] [-srv IP:PORT]\n");
        printf("�C�L���: -list date YYYYMMDD1[:YYYYMMDD2] [time HHMM1:HHMM2] \n");
        printf("          -cid CID -sid SID \n");
        printf("          [-t1] [-s1] [-m1] [-d1] [-srv IP:PORT]\n");
        printf(" -add b csv: �q CSV�ɼW�[ Bar���(��1). \n");
        printf("           �d��: -add csv \"c:\\abc\\2330.csv\"\n");
        printf(" -add t csv: �q CSV�ɼW�[ Tick���(��2). \n");
        printf("           �d��: -add csv \"c:\\abc\\2330.csv\"\n");
        printf(" -add b data: �����W�[ Bar���(��1). \n");
        printf("            �d��: -add data \"2009-1-6, 13:45:00, 100, 101, 99, 100, 1234\"\n");
        printf(" -add t data: �����W�[ Tick���(��2). \n");
        printf("            �d��: -add data \"2009-1-6, 13:45:00, 100, 2\"\n");
        printf(" -del : �R�����, �d��: -del date 20090101:20090105\n"
               " -date : ���ק諸�����, �d��: -date 20090102.\n"
               " -cid : ���ק諸�ӫ~, �d��: -cid TXF (�x����)\n"
               " -sid : ���ק諸�ӫ~�������W, �d��: -sid TXF200801 or -sid TXF_HOT\n"
               " -mt : �}���L�_�W�ɶ�, �d��: -mt 0845:1345\n"
               " -c : ��J�� CSV ��, �d��: -c 20080102.csv or -c \"c:\abc\20080102.csv\"\n"
			   " -t1 : �ק� Tick��\n"
			   " -s1 : �ק����\n"
			   " -m1 : �ק����\n"
			   " -d1 : �ק����\n"
			   " -srv : ��X Log�ܻ��� ip:port, �d��: -srv 172.0.0.1:5000\n"
			   " ��1 : Bar CSV�榡:  YYYY-MM-DD, HH:MM:SS, Open, High, Low, Close, Vol\n"
			   " ��2 : Tick CSV�榡: YYYY-MM-DD, HH:MM:SS, Price, Vol\n"
               " \n"
               " �W�[��ƪ��d��:\n"
               "  -add b data \"2009-1-6, 13:45:00, 100, 101, 99, 101, 1234\" \\\n"
               "       -cid TXF -sid TXF200806 -t1 -s1 -d1 -m1 -mt 0845:1345\n"
               "  -add t data \"2009-1-6, 13:44:59, 100, 10\" -cid TXF -sid TXF200806 \\\n"
               "       -t1 -s1 -d1 -m1 -mt 0845:1345\n"
               "  -add b csv \"c:\\try\\m1ec.csv\" -cid M1EC -sid M1EC_HOT \\\n"
               "       -s1 -d1 -m1 -mt 1700:1600\n"
               " �R����ƪ��d��:\n"
               "  -del date 20080612 time 13000000:13300000  -cid TXF -sid TXF200806 \\\n"
               "       -t1 -s1 -d1 -m1 -mt 0845:1345\n"
               "  -del date 20090129 time 25000000:26000000  -cid M1EC -sid M1EC_HOT \\\n"
               "       -m1 -mt 1700:1600\n"
               " �C�L��ƪ��d��:\n"
               "  -list date 20080612 time 13000000:13300000  -cid TXF -sid TXF200806 \\\n"
               "        -t1 -s1 -d1 -m1\n"
               "  -list date 20081201:20081202 -cid M1EC -sid M1EC_HOT -m1\n"
               "  -list date 20090129 time 24000000:26000000 -cid M1EC -sid M1EC_HOT -m1\n"
               );
        return 0;
    }

    if (cszGetParam("-t1", 0, argc, argv) != DF_NO_DATA)
    {
        iDoTicks = 1;
    }
    if (cszGetParam("-s1", 0, argc, argv) != DF_NO_DATA)
    {
        iDoSecs = 1;
    }
    if (cszGetParam("-m1", 0, argc, argv) != DF_NO_DATA)
    {
        iDoMins = 1;
    }
    if (cszGetParam("-d1", 0, argc, argv) != DF_NO_DATA)
    {
        iDoDailys = 1;
    }
    cszServerIPandPort = cszGetParam("-srv", 1, argc, argv);;


    if (iAddMode == 1)
    {
        fnDoAddBar(iMarketOpenTime, iMarketCloseTime, cszCommodity, cszSymbolID, iFileType, cszFileNameOrData, 
            iModifyDate1, iModifyDate2, iModifyTime1, iModifyTime2,
            iDoTicks, iDoSecs, iDoMins, iDoDailys);
    }
    else if (iAddMode == 2)
    {
        fnDoAddTick(iMarketOpenTime, iMarketCloseTime, cszCommodity, cszSymbolID, iFileType, cszFileNameOrData, 
            iModifyDate1, iModifyDate2, iModifyTime1, iModifyTime2,
            iDoTicks, iDoSecs, iDoMins, iDoDailys);
    }
    else if(iDelMode == 1)
    {
        fnDoDel(iMarketOpenTime, iMarketCloseTime, cszCommodity, cszSymbolID, 
            iModifyDate1, iModifyDate2, iModifyTime1, iModifyTime2,
            iDoTicks, iDoSecs, iDoMins, iDoDailys);
    }
    else if(iListMode == 1)
    {
        fnDoList(iMarketOpenTime, iMarketCloseTime, cszCommodity, cszSymbolID, 
            iModifyDate1, iModifyDate2, iModifyTime1, iModifyTime2,
            iDoTicks, iDoSecs, iDoMins, iDoDailys);
    }


/*
	///////////////////
    int iDoTicks = 1;
    int iDoSecs = 1;
    int iDoMins = 1;
    int iDoDailys = 1;
	int iDoHot = 0;

    int iBinaryOutputFlag = 0;
	int iZipInputFlag = 0;
    HWND hwndParent = NULL;
    CString cszCommandLine = GetCommandLine();
    cszCommandLine.MakeLower();
    if (strstr(cszCommandLine, "-f ") == NULL &&
		strstr(cszCommandLine, "-z ") == NULL)
    {
        printf("Please input *.Rpt or *.Zip\n"
			   "  ie: TaiFexFutureTick -f Daily_2008_06_12.rpt or \n"
			   "      TaiFexFutureTick -z Daily_2008_06_12.zip");
#ifdef _DEBUG
		printf("\n");
        printf("TaiFexFutureTick [-hwnd nnnn] [-t1] [-s1] [-m1] [-d1] [-hot1] [-z|-f] [-b] input filename\n");
        printf(" -hwnd nnnn: nnnn is the handle of the parent window.\n"
			   " -t1 : output ticks file.\n"
			   " -s1 : output seconds file.\n"
			   " -m1 : output minutes file.\n"
			   " -d1 : output daily file.\n"
			   " -hot1 : output hot month symbol.\n"
			   " -b : output binary file.\n");
        printf("  ie: TaiFexFutureTick -f Daily_2008_06_12.rpt <== rpt file \n"
			   "      TaiFexFutureTick -z Daily_2008_06_12.zip <== zip file \n");
#endif
        return 0;
    }

    char* szInputFile = NULL;
	if (strstr(cszCommandLine, "-f ") != NULL)
	{
		szInputFile = strstr(cszCommandLine, "-f ") + 3;
		iZipInputFlag = 0;
	}
	if (strstr(cszCommandLine, "-z ") != NULL)
	{
		szInputFile = strstr(cszCommandLine, "-z ") + 3;
		iZipInputFlag = 1;
	}

	CString cszTemp = szInputFile;
	cszTemp.TrimLeft();
	cszTemp.TrimRight();
    if (cszTemp.GetLength() == 0)
    {
        printf("Please input *.Rpt or *.Zip\n"
			   "  ie: TaiFexFutureTick -f Daily_2008_06_12.rpt or \n"
			   "      TaiFexFutureTick -z Daily_2008_06_12.zip");
#ifdef _DEBUG
		printf("\n");
        printf("TaiFexFutureTick [-hwnd nnnn] [-t1] [-s1] [-m1] [-d1] [-hot1] [-z|-f] [-b] input filename\n");
        printf(" -hwnd nnnn: nnnn is the handle of the parent window.\n"
			   " -t1 : output ticks file.\n"
			   " -s1 : output seconds file.\n"
			   " -m1 : output minutes file.\n"
			   " -d1 : output daily file.\n"
			   " -hot1 : output hot month symbol.\n"
			   " -b : output binary file.\n");
        printf("  ie: TaiFexFutureTick -f Daily_2008_06_12.rpt <== rpt file \n"
			   "      TaiFexFutureTick -z Daily_2008_06_12.zip <== zip file \n");
#endif
        return 0;
    }

    if (strstr(cszCommandLine, "-b") != NULL)
    {
        iBinaryOutputFlag = 1;
    }
    
    if (strstr(cszCommandLine, "-hwnd ") != NULL)
    {
        int iHwndStart = (strstr(cszCommandLine, "-hwnd ") - cszCommandLine) + 6;
        char szHWND[20];
        szHWND[0] = '\0';
        for (int k=iHwndStart;k<strlen(cszCommandLine);++k)
        {
            if (cszCommandLine[k] == ' ')
            {
                strncat(szHWND, ((LPCTSTR)cszCommandLine)+iHwndStart, k-iHwndStart);
                break;
            }
        }
        hwndParent = (HWND)atoi(szHWND);
    }

    if (strstr(cszCommandLine, "-t0") != NULL)
        iDoTicks = 0;
    if (strstr(cszCommandLine, "-s0") != NULL)
        iDoSecs = 0;
    if (strstr(cszCommandLine, "-m0") != NULL)
        iDoMins = 0;
    if (strstr(cszCommandLine, "-d0") != NULL)
        iDoDailys = 0;
    if (strstr(cszCommandLine, "-hot1") != NULL)
        iDoHot = 1;


    unsigned char* pszBuf;
    int iFileSize;

	if (iZipInputFlag == 1)
	{
		char szErrMsg[512];
		int iTotalFile = iGetZipBuffer(szInputFile, szErrMsg, 1024, NULL, NULL, NULL, 0);
		if (iTotalFile == -1)
		{
			printf("[%s]Zip file error! %s\n",szInputFile, szErrMsg);
            system("pause");
			return 0;
		}
		for (int i=0;i<iTotalFile;++i)
		{
			char* pszUnzipFilename;
			int iRet = iGetZipBuffer(szInputFile, szErrMsg, 1024, &pszUnzipFilename, &pszBuf, &iFileSize, i);
			if (iRet == -1)
			{
				printf("[%s]Unzip file error! %s\n", szErrMsg);
                system("pause");
			}
			else
			{
				CString cszUnzipFilename = pszUnzipFilename;
				delete pszUnzipFilename;
				cszUnzipFilename.MakeLower();
				if (cszUnzipFilename.Find(".rpt") > 0)
				{
					bStartParseRPT(pszBuf, iFileSize, iBinaryOutputFlag, 
                                    iDoTicks, iDoSecs, iDoMins, iDoDailys, iDoHot);
				}
				else
				{
					printf("File is not *.rpt");
				}
				delete pszBuf;
			}
		}
	}
	else
	{
		FILE* fpFile;
		fpFile = fopen(szInputFile, "rb");
		if (fpFile == NULL)
		{
			printf("File not exist!");
			return 0;
		}
		if (fpFile != NULL)
		{
			fseek(fpFile, 0, SEEK_END);
			iFileSize = ftell(fpFile);
			fseek(fpFile, 0, SEEK_SET);
			pszBuf = new unsigned char[iFileSize + 1];
			fread(pszBuf, sizeof(unsigned char), iFileSize, fpFile);
			pszBuf[iFileSize] = '\0';
			fclose(fpFile);
		}
		bStartParseRPT(pszBuf, iFileSize, iBinaryOutputFlag, 
                       iDoTicks, iDoSecs, iDoMins, iDoDailys, iDoHot);
		delete pszBuf;
	}

if (hwndParent != NULL)
{
    ::UpdateWindow(hwndParent);
//    ::InvalidateRect(
}
*/
	return nRetCode;
}


