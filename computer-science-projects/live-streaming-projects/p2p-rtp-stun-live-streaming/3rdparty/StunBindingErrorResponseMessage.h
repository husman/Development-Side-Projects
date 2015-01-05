#pragma once
#include "stunerrorresponsemessage.h"

class CStunBindingErrorResponseMessage :
	public CStunErrorResponseMessage
{
public:
	CStunBindingErrorResponseMessage(unsigned short nErrorCode);
	CStunBindingErrorResponseMessage(char *pBuffer);
	~CStunBindingErrorResponseMessage(void);
};
