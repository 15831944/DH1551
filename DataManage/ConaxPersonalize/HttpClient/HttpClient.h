//////////////////////////////////// HttpClient.h
#ifndef HTTPCLIENT_H_34__
#define HTTPCLIENT_H_34__

#include <afxinet.h>
#include <string>
using namespace std;

//#define  IE_AGENT  "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)"

// 操作成功
#define SUCCESS        0
// 操作失败
#define FAILURE        1
// 操作超时 www.it165.net
#define OUTTIME        2

class CHttpClient
{
public:
	CHttpClient( );
	virtual ~CHttpClient( );

	//int HttpPost(char* pIPAddr, INTERNET_PORT nPort, char *pObj, char *pRequestSendData, CString &strResponse);


	void MakePreFileData(LPCTSTR strBoundary, LPCTSTR strFileName, CString & strData);

	void MakePostFileData(LPCTSTR strBoundary, CString & strData);

	int UploadFile(char* pIPAddr, INTERNET_PORT nPort, char *pObj, LPCTSTR strFileName, unsigned char *pucBuffer, unsigned int *punLen, char *pHeaderInfo);

	void MakeRequestHeaders(LPCTSTR strBoundary, CString & head) ;

	int HttpURLGet(char *cURL, unsigned char *response, unsigned int *punLen);
	
private:
	
	//int ExecuteRequest(int strMethod, char* pIPAddr, INTERNET_PORT nPort, char *pObj, char *pRequestSendData, CString &strResponse);
	

private:

};

#endif // HTTPCLIENT_H

