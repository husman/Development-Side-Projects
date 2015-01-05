#include "StdAfx.h"
#include "StunSharedSecretErrorResponseMessage.h"

CStunSharedSecretErrorResponseMessage::CStunSharedSecretErrorResponseMessage(unsigned short nErrorCode):
	CStunErrorResponseMessage (SHARED_SECRET_ERROR_RESPONSE,
	nErrorCode)
{
}

CStunSharedSecretErrorResponseMessage::CStunSharedSecretErrorResponseMessage(char *pBuffer):
	CStunErrorResponseMessage (pBuffer)
{
}

CStunSharedSecretErrorResponseMessage::~CStunSharedSecretErrorResponseMessage(void)
{
}