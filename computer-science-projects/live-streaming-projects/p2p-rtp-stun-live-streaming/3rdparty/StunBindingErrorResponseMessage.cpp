#include "StdAfx.h"
#include "StunBindingErrorResponseMessage.h"

CStunBindingErrorResponseMessage::CStunBindingErrorResponseMessage(unsigned short nErrorCode):
	CStunErrorResponseMessage(BINDING_ERROR_RESPONSE,
	nErrorCode)
																
{
}

CStunBindingErrorResponseMessage::CStunBindingErrorResponseMessage (char *pBuffer):
	CStunErrorResponseMessage(pBuffer)
{
}

CStunBindingErrorResponseMessage::~CStunBindingErrorResponseMessage(void)
{
}
