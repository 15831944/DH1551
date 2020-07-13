#ifndef CONAX_PERSONALIZE_H_
#define CONAX_PERSONALIZE_H_

#include "JobFileManage.h"

class ConaxPersonalize
{
public:
	ConaxPersonalize();
	~ConaxPersonalize();

public:
	int SetNetworkInfo(char *pIP, int nLen, int nPort);

	void SetInputCSVFilePath(char *path);

	int InitPersonalize(void);

	int GetHomePage(void);

	int InitJobFile(char *pFileName, int nLen);//VscPersoCardNotPresent, UnknownFileFormat

	int DeleteJobFile(void);//ParameterError,UnknownJob, VscPersoCardNotPresent, FunctionNotAvailable

	int RequestEncrytionKey(unsigned int unChipID, unsigned char *pBuffer, unsigned int *pLen);//ParameterError,UnknownJob, UnknowChipID, JobFinished, VscPersoCardNotPresent

	int RequestVSCData(unsigned char *pvscdata, unsigned int *punvscdataLen);

	int PrepareReportFile(unsigned char *pReport, unsigned int *punLen, char *filename);//ParameterError,UnknownJob, UnknowFileFormat

	int LoadJobfile(void);

private:
	int RecordPersonalizeData(unsigned unChipID, unsigned int unJobID, unsigned int unVSCID);

private:
	char m_acIP[32];
	unsigned int m_unPort;

	unsigned int m_unChipID; // text--%u
	unsigned int m_unJobID; //text--%u
	unsigned int m_unVscID; //text--%u
	unsigned int m_unFirstVscID;
	unsigned int m_unLastVscID;
	unsigned int m_unNumberofIssuedUnits;

	char m_pJobfilepath[256];
	char m_pInputCSVFilePath[256];

	JobFileManage m_JobFileManage;
};

#endif/*CONAX_PERSONALIZE_H_*/