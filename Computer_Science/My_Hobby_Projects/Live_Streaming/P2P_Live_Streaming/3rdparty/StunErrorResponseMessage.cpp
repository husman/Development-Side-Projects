#include "StdAfx.h"
#include "StunErrorResponseMessage.h"

CStunErrorResponseMessage::CStunErrorResponseMessage(STUN_MESSAGE_TYPE nMessageType, unsigned short nErrorCode):
	CStunMessage (nMessageType)
{
	AddAttribute (new CStunErrorCodeAttribute (nErrorCode));
}

CStunErrorResponseMessage::~CStunErrorResponseMessage(void)
{
}

CStunErrorResponseMessage::CStunErrorResponseMessage(char *pBuffer):CStunMessage(pBuffer)
{
}

bool CStunErrorResponseMessage::AddUnknownAttributes(unsigned short nAttribute)
{
	bool bRet = false;
	CStunUnknownAttributes *pStunUnknownAttributes = NULL;

	if (GetAttribute (UNKNOWN_ATTRIBUTES) == NULL)
	{
		pStunUnknownAttributes = new CStunUnknownAttributes();

		bRet = pStunUnknownAttributes->AddAttribute (nAttribute);

		if (bRet)
		{
			AddAttribute (pStunUnknownAttributes);
		}
		else
		{
			delete pStunUnknownAttributes;
		}
	}
	else
	{
		pStunUnknownAttributes = (CStunUnknownAttributes *)GetAttribute (UNKNOWN_ATTRIBUTES);
		bRet = pStunUnknownAttributes->AddAttribute (nAttribute);
	}

	return bRet;	
}

bool CStunErrorResponseMessage::GetErrorCode(unsigned short nErrorCode)
{
	if (HasAttribute (ERROR_CODE) == true)
	{
		nErrorCode = ((CStunErrorCodeAttribute *)GetAttribute (ERROR_CODE))->GetErrorCode ();
		return true;
	}

	return false;
}
