#include "StdAfx.h"
#include "StunSharedSecretRequestMessage.h"

CStunSharedSecretRequestMessage::CStunSharedSecretRequestMessage(void):CStunMessage (SHARED_SECRET_REQUEST)
{
}

CStunSharedSecretRequestMessage::CStunSharedSecretRequestMessage(char *pBuffer):
	CStunMessage (pBuffer)
{
}

CStunSharedSecretRequestMessage::~CStunSharedSecretRequestMessage(void)
{
}
