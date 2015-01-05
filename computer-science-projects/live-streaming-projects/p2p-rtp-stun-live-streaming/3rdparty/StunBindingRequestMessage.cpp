#include "StdAfx.h"
#include "StunBindingRequestMessage.h"
#include <cassert>

CStunBindingRequestMessage::CStunBindingRequestMessage(void):CStunBindingMessage(BINDING_REQUEST)
{
}

CStunBindingRequestMessage::CStunBindingRequestMessage(char *pBuffer):CStunBindingMessage(pBuffer)
{
}

CStunBindingRequestMessage::~CStunBindingRequestMessage(void)
{
}

void CStunBindingRequestMessage::AddResponseAddressAttribute(unsigned short nPort, const char *pszAddress)
{
	assert (pszAddress != NULL);
	CStunAddressAttribute *pStunReponseAddressAttribute = new CStunAddressAttribute (RESPONSE_ADDRESS,
																					 0x01,
																					 nPort,
																					 pszAddress);
	AddAttribute (pStunReponseAddressAttribute);
}

void CStunBindingRequestMessage::AddChangeRequestAttribute(unsigned int nChangeFlag)
{
	AddAttribute (new CStunChangeRequestAttribute (nChangeFlag));
}

void CStunBindingRequestMessage::AddUsernameAttribute (const char* pszUsername)
{
	assert (pszUsername != NULL);
	AddAttribute (new CStunUsernameAttribute (pszUsername));
}

bool CStunBindingRequestMessage::GetResponseAddress(SOCKADDR_IN *addr)
{
	if (HasAttribute (RESPONSE_ADDRESS) == true)
	{
		CStunAddressAttribute *pAddressAttribute = (CStunAddressAttribute *)GetAttribute (RESPONSE_ADDRESS);
		addr->sin_addr.S_un.S_addr = (int)pAddressAttribute->GetIPAddress ();
		addr->sin_port = htons (pAddressAttribute->GetPort ());
		addr->sin_family = AF_INET;
		return true;
	}

	return false;
}

bool CStunBindingRequestMessage::GetUsername (const char **pszUsername)
{
	if (HasAttribute (USERNAME) == true)
	{
		CStunUsernameAttribute *pUsernameAttribute = (CStunUsernameAttribute *)GetAttribute (USERNAME);
		*pszUsername = pUsernameAttribute->GetUsername ();
		return true;
	}

	return false;
}

bool CStunBindingRequestMessage::GetChangeRequest(unsigned int nChangeRequest)
{
	if (HasAttribute (CHANGE_REQUEST) == true)
	{
		CStunChangeRequestAttribute *pAttribute = (CStunChangeRequestAttribute*)GetAttribute (CHANGE_REQUEST);
		nChangeRequest = pAttribute->GetChangeRequestFlag ();
		return true;
	}

	return false;
}