////////////////////////////////// HttpClient.cpp
#include <afxwin.h>
#include <string>
#include "HttpClient.h"
#include "DebugLog.h"

#define  BUFFER_SIZE       1024

#define  NORMAL_CONNECT             INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT             NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST             INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define  SECURE_REQUEST             NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID

CHttpClient::CHttpClient()
{

}


CHttpClient::~CHttpClient(void)
{

}

//感谢这者两位作者,我将他们两个的代码整合了一下,本来以为用第一个链接里面的代码拷贝过来修改就可以用了,
//可是却一直提示服务器无法访问,第二个链接里面的代码虽然可以连接到服务器,但是却无法上传文件,
//所以将两个代码整合了一下,如果有纰漏,希望大家给出建议:

//http://blog.csdn.NET/angellove156/article/details/41045583
//http://www.docin.com/p-660772334.html
void CHttpClient::MakePreFileData(LPCTSTR strBoundary, LPCTSTR strFileName, CString &strData) 
{
	CString strFormat;  
	
	strFormat += _T("--%s");  

	strFormat += _T("\r\n");  
	strFormat += _T("Content-Disposition: form-data; name=\"file\"; filename=\"%s\"");  

	strFormat += _T("\r\n");  
	strFormat += _T("Content-Type: text/plain");  

	strFormat += _T("\r\n\r\n");  
	strData.Format(strFormat,  strBoundary, strFileName); 
}

void CHttpClient::MakeRequestHeaders(LPCTSTR strBoundary, CString & strData) 
{
	CString strFormat;  
  
	strFormat = _T("Content-Type: multipart/form-data; boundary=%s\r\n");  
	strData.Format(strFormat, strBoundary);  
}

void CHttpClient::MakePostFileData(LPCTSTR strBoundary, CString &strData) 
{
 	// TODO: Add your dispatch handler code here
	CString strFormat;  
	
    strFormat = _T("\r\n");   
    strFormat += _T("--%s");  
    strFormat += _T("\r\n");  
    strFormat += _T("Content-Disposition: form-data; name=\"upload\"");  
    strFormat += _T("\r\n\r\n");  
    strFormat += _T("hello");  
    strFormat += _T("\r\n");  
    strFormat += _T("--%s--");  
    strFormat += _T("\r\n");  
    strData.Format(strFormat, strBoundary, strBoundary);   
}

int CHttpClient::UploadFile(char* pIPAddr, INTERNET_PORT nPort, char *pObj, LPCTSTR strFileName, unsigned char *pucBuffer, unsigned int *punLen, char *pHeaderInfo)
{
	int nRt = -1;
	DWORD dwRet = 0;
	CInternetSession Session;  
	CHttpConnection *pHttpConnection = NULL;  
	CFile fTrack;  
	CHttpFile* pHTTP = NULL;  
	CString strHTTPBoundary;  
	CString strPreFileData;  
	CString strPostFileData;
	CString strHeader;
	DWORD dwTotalRequestLength = 0;  
	DWORD dwChunkLength = 0;  
	DWORD dwReadLength = 0;  
	DWORD dwResponseLength = 0;  
	TCHAR szError[MAX_PATH] = {0};  
	void* pBuffer = NULL;      
	CString strDebugMessage; 
	DWORD dwType = 0;
	unsigned int unOffset = 0;
	unsigned int contentLen = 0;

	//读取文件
	do
	{
		if (FALSE == fTrack.Open(strFileName, CFile::modeRead | CFile::shareDenyWrite))  
		{  
		    //PrintfLog((char*) (_T("Open File Failed path") + CString(strFileName)) );  
			PrintfLog("Open File Failed path:%s\r\n", strFileName); 
		    nRt = -1;
			break;
		}

		int iRecordID = 1;  
		strHTTPBoundary = _T("---------------------------7b4a6d158c9");//定义边界值  
		CString pcmname = strFileName;  
		pcmname = pcmname.Mid(pcmname.ReverseFind('\\') + 1);//获取要上传的文件的名字

		PrintfLog("FileName:%s \r\n", pcmname);  

		MakePreFileData(strHTTPBoundary, pcmname, strPreFileData); 
		
		MakePostFileData(strHTTPBoundary, strPostFileData);  
		
		dwTotalRequestLength = strPreFileData.GetLength() + strPostFileData.GetLength() + fTrack.GetLength();//计算整个包的总长度  

		dwChunkLength = fTrack.GetLength(); 

		pBuffer = malloc(dwChunkLength);  
		if (NULL == pBuffer)  
		{  
		    PrintfLog("Fun_UploadPic malloc failed len:%d= \r\n", dwChunkLength);    
		    nRt = -1;
			break; 
		}

		try  
		{  
			Session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);	   // 5秒的连接超时
		    pHttpConnection = Session.GetHttpConnection(pIPAddr, nPort);
		    pHTTP = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, pObj);  
		    if(NULL==pHTTP)
		    {	
		    	AfxMessageBox("pHTTP is ZERO error");  
				nRt = -1;
				break;
		    }

			MakeRequestHeaders(strHTTPBoundary, strHeader);
			
		    pHTTP->AddRequestHeaders(strHeader);//发送包头请求
			
		    pHTTP->SendRequestEx(dwTotalRequestLength, HSR_SYNC | HSR_INITIATE);
			// step1 header
		    pHTTP->Write((LPSTR)(LPCSTR)strPreFileData, strPreFileData.GetLength());  

			dwReadLength = fTrack.Read(pBuffer, dwChunkLength);
		    while (0 != dwReadLength)  
		    {  
		    	// step2 body
		        if (0 != dwReadLength)  
		        {  
		            pHTTP->Write(pBuffer, dwReadLength);//写入服务器本地文件，用二进制进行传送  
		        }

				dwReadLength = fTrack.Read(pBuffer, dwChunkLength);
		    }
	
			// step3 tail
		    pHTTP->Write((LPSTR)(LPCSTR)strPostFileData, strPostFileData.GetLength());  
			pHTTP->EndRequest(HSR_SYNC);

			//Sleep(300); // 300ms

			//-------------------------------------------------
			{
				CString cSTR;
				BOOL bRet = FALSE;
				char *p = 0;

				bRet = pHTTP->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, cSTR, 0);
				PrintfLog("QueryInfo HTTP_QUERY_CONTENT_LENGTH bRet:%d cSTR:%s \r\n", bRet, cSTR);

				contentLen = atoi(cSTR);

				bRet = pHTTP->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, cSTR, 0);
				PrintfLog("QueryInfo HTTP_QUERY_RAW_HEADERS_CRLF bRet:%d cSTR:%s \r\n", bRet, cSTR);
				
				p = cSTR.GetBuffer(0);
				strcpy(pHeaderInfo, p);
				cSTR.ReleaseBuffer();
			}

	    	while (1)  
		   	{  
				dwResponseLength = pHTTP->GetLength();
				if(dwResponseLength <= 0)
				{
					break; // read finish
				}
				
		        pHTTP->Read(&pucBuffer[unOffset], dwResponseLength);
				unOffset += dwResponseLength;
		    } 

			if(contentLen != unOffset)
			{
				PrintfLog("Uploadfile contentLen:%d unOffset:%d \r\n", contentLen, unOffset);
				AfxMessageBox("get data len ERROR");
				nRt = -1;
				break;
			}
			else
			{
				//AfxMessageBox("get data len OK");
			}
			
			*punLen = unOffset;

			pHTTP->QueryInfoStatusCode(dwRet); // status
			if(HTTP_STATUS_OK == dwRet)
			{
				nRt = 0;
			}
			else
			{
				strDebugMessage.Format("File:%s,\n Err:%d,\n msg:%s", strFileName, dwRet, pucBuffer);
				AfxMessageBox(strDebugMessage);
				nRt = -1;
				break;
			}
		}  
		catch (CException* e)  
		{  
		    e->GetErrorMessage(szError, MAX_PATH);  
		    e->Delete();  
		    AfxMessageBox(szError);  
		    nRt = -1;
			break; 
		}  		
	}while(0);

	if(0 != pHTTP)
	{
		pHTTP->Close();  
		delete pHTTP; 
		pHTTP = 0;
	}

	if(0 != pHttpConnection)
	{
		pHttpConnection->Close();
		delete pHttpConnection; 
		pHttpConnection = 0;
		
	}

	Session.Close();

	if (fTrack.m_hFile != CFile::hFileNull)
	fTrack.Close();  

	if (NULL != pBuffer)  
	{  
	    free(pBuffer);  
	}

	return nRt;
}

int CHttpClient::HttpURLGet(char *cURL, unsigned char *response, unsigned int *punLen)
{
	int nRst = -1;
	CInternetSession session1(NULL,0); 
	CHttpFile* pHTTPFile=NULL;
	TCHAR szError[MAX_PATH] = {0};
	unsigned int unOffset = 0; 

	try
	{ 
		session1.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);	   // 5秒的连接超时
		
		pHTTPFile=(CHttpFile*)session1.OpenURL(cURL,1,INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_TRANSFER_ASCII,NULL,0);  //清空缓存bug20170519 
	}
	catch(CInternetException *e)
	{
		DWORD dwErrorCode = e->m_dwError;
		DWORD dwError = GetLastError();
		
		PrintfLog("dwErrorCode = %d, dwError:%d\r\n", dwErrorCode, dwError);
		
		e->GetErrorMessage(szError, MAX_PATH);	
		e->Delete();  
		AfxMessageBox(szError); 
	} 

	if(pHTTPFile) 
	{ 
		DWORD dwRet = 0;
		unsigned int dwResponseLength = 0;

		while (1)  
		{  
			dwResponseLength = pHTTPFile->GetLength();
			if(dwResponseLength <= 0)
			{
				break; // read finish
			}
			
			pHTTPFile->Read(&response[unOffset], dwResponseLength);
			unOffset += dwResponseLength;
		}

		*punLen = unOffset;

		pHTTPFile->QueryInfoStatusCode(dwRet);
		PrintfLog("ExecuteRequest HTTP_STATUS_code:%d\r\n", dwRet);

		if(HTTP_STATUS_OK == dwRet)
		{
			nRst = 0;
		}
		else
		{
			CString msg;
			msg.Format("open:%s failed errorCode:%d,msg:%s", cURL, dwRet, response);
			AfxMessageBox(msg);
		}
		pHTTPFile->Close(); 
		session1.Close();
		delete pHTTPFile;
	}
	return nRst;	
}