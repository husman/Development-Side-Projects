#pragma once
#include "stunmessage.h"

class CStunSharedSecretRequestMessage :
	public CStunMessage
{
public:
	CStunSharedSecretRequestMessage(void);
	CStunSharedSecretRequestMessage(char *pBuffer);
	~CStunSharedSecretRequestMessage(void);
};
