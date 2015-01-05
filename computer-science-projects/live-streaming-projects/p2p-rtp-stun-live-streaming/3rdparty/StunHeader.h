#pragma once
#include "StdAfx.h"
#include <windows.h>
#include <cstdlib>
#include <psapi.h>
#include <winsock2.h>

typedef unsigned short STUN_MESSAGE_TYPE;

enum
{
	BINDING_REQUEST			= 0x0001,
	BINDING_RESPONSE		= 0x0101,
	BINDING_ERROR_RESPONSE	= 0x0111,
    SHARED_SECRET_REQUEST	= 0x0002,
    SHARED_SECRET_RESPONSE	= 0x0102,
    SHARED_SECRET_ERROR_RESPONSE = 0x0112
};

class CStunHeader
{
private:
	unsigned short m_nMessageLength;
	unsigned char  m_cTransactionId[16];

	STUN_MESSAGE_TYPE m_nMessageType;

public:
	CStunHeader (STUN_MESSAGE_TYPE MessageType);
	CStunHeader (char *pBuffer);
	~CStunHeader (void);
	void GetBuffer (char *pszBuffer);
	unsigned short GetHeaderLength ();
	STUN_MESSAGE_TYPE GetMessageType ();
	unsigned short GetMessageLength ();
	void SetMessageLength (unsigned short nLength);
	string ToString();

private:
	void CreateTransactionId ();
};
