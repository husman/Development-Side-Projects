#pragma once
#include "stunattribute.h"

class CStunErrorCodeAttribute :
	public CStunAttribute
{
private:
	unsigned char m_byClass;
	unsigned char m_byNumber;
	unsigned short m_nErrorCode;
	char *m_pszReasonPhrase;

public:
	CStunErrorCodeAttribute(unsigned short nErrorCode, const char *pszReasonPhrase = NULL);
	CStunErrorCodeAttribute(char *pBuffer);
	const char* GetReasonPhrase();

	~CStunErrorCodeAttribute(void);

	unsigned char GetClass(void);
	unsigned char GetNumber(void);

	void GetBuffer (char *Buffer);
	unsigned short GetErrorCode(void);
	string ToString ();
};
