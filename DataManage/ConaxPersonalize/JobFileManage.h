#ifndef JOB_FILE_MANAGE_H_
#define JOB_FILE_MANAGE_H_

#define JOBDIR ("Source")

class JobFileManage
{
public:
	JobFileManage();
	~JobFileManage();

public:
	int InitConfig(void);
	int GetCurJobFileName(char *pBuffer, int nLen);
	int GetNextJobFileName(char *pBuffer, int nLen);

private:
	int ReadConfigData(char *pFileDir, int nLen);
	int SaveConfigData(char *pFileDir, int nLen);

private:
	unsigned int m_unJobIndex;
};

#endif/*JOB_FILE_MANAGE_H_*/