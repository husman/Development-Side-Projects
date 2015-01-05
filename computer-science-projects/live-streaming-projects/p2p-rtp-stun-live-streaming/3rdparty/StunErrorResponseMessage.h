#pragma once
#include "stunmessage.h"

class CStunErrorResponseMessage :
	public CStunMessage
{
public:
	CStunErrorResponseMessage(STUN_MESSAGE_TYPE nMessageType, unsigned short nErrorCode);
	CStunErrorResponseMessage(char *pBuffer);

	virtual ~CStunErrorResponseMessage(void);

	bool AddUnknownAttributes (unsigned short nAttribute);
	
	bool GetErrorCode(unsigned short nErrorCode);
};
