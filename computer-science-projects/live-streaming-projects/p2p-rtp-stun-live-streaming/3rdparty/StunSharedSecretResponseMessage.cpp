#include "StdAfx.h"
#include "StunSharedSecretResponseMessage.h"

CStunSharedSecretResponseMessage::CStunSharedSecretResponseMessage(const char* pszUsername,
																   const char* pszPassword):
	CStunMessage (SHARED_SECRET_RESPONSE)
{
	AddAttribute (new CStunUsernameAttribute (pszUsername));
	AddAttribute (new CStunPasswordAttribute (pszPassword));
}

CStunSharedSecretResponseMessage::CStunSharedSecretResponseMessage(char *pBuffer):
	CStunMessage (pBuffer)
{
}

CStunSharedSecretResponseMessage::~CStunSharedSecretResponseMessage(void)
{
}

bool CStunSharedSecretResponseMessage::GetUsername (const char **pszUsername)
{
	CStunUsernameAttribute *pAttribute = (CStunUsernameAttribute *)GetAttribute (USERNAME);
	*pszUsername = pAttribute->GetUsername ();
	return true;
}

bool CStunSharedSecretResponseMessage::GetPassword (const char **pszPassword)
{
	CStunPasswordAttribute *pAttribute = (CStunPasswordAttribute *)GetAttribute (PASSWORD);
	*pszPassword = pAttribute->GetPassword ();
	return true;
}