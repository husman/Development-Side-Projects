#include "StdAfx.h"
#include "StunBindingResponseMessage.h"
#include <cassert>

CStunBindingResponseMessage::CStunBindingResponseMessage(CStunAddressAttribute *pMappedAddressAttribute, 
								CStunAddressAttribute *pSourceAddressAttribute,
								CStunAddressAttribute *pChangedAddressAttribute):
								CStunBindingMessage(BINDING_RESPONSE)
{
	assert (pMappedAddressAttribute != NULL);
	assert (pSourceAddressAttribute != NULL);
	assert (pChangedAddressAttribute != NULL);

	AddAttribute (pMappedAddressAttribute);
	AddAttribute (pSourceAddressAttribute);
	AddAttribute (pChangedAddressAttribute);
}

CStunBindingResponseMessage::~CStunBindingResponseMessage(void)
{
}

CStunBindingResponseMessage::CStunBindingResponseMessage(char *pBuffer):CStunBindingMessage(pBuffer)
{
}

void CStunBindingResponseMessage::AddReflectedFromAddressAttribute(unsigned short nPort, const char *pszAddress)
{
	assert (pszAddress != NULL);
	AddAttribute (new CStunAddressAttribute (REFLECTED_FROM,
											 0x0001,
											 nPort,
											 pszAddress));
}

bool CStunBindingResponseMessage::GetMappedAddress(SOCKADDR_IN *pAddr)
{
	CStunAddressAttribute *pAddressAttribute = (CStunAddressAttribute *)GetAttribute (MAPPED_ADDRESS);
	pAddr->sin_addr.S_un.S_addr = (int)pAddressAttribute->GetIPAddress ();
	pAddr->sin_port = htons (pAddressAttribute->GetPort ());
	pAddr->sin_family = AF_INET;
	return true;
}

bool CStunBindingResponseMessage::GetSourceAddress(SOCKADDR_IN *pAddr)
{
	CStunAddressAttribute *pAddressAttribute = (CStunAddressAttribute *)GetAttribute (SOURCE_ADDRESS);
	pAddr->sin_addr.S_un.S_addr = (int)pAddressAttribute->GetIPAddress ();
	pAddr->sin_port = htons (pAddressAttribute->GetPort ());
	pAddr->sin_family = AF_INET;
	return true;
}

bool CStunBindingResponseMessage::GetChangedAddress(SOCKADDR_IN *pAddr)
{
	CStunAddressAttribute *pAddressAttribute = (CStunAddressAttribute *)GetAttribute (CHANGED_ADDRESS);
	pAddr->sin_addr.S_un.S_addr = pAddressAttribute->GetIPAddress ();
	pAddr->sin_port = htons (pAddressAttribute->GetPort ());
	pAddr->sin_family = AF_INET;
	return true;
}

bool CStunBindingResponseMessage::GetReflectedFromAddress(SOCKADDR_IN *pAddr)
{
	CStunAddressAttribute *pAddressAttribute = (CStunAddressAttribute *)GetAttribute (REFLECTED_FROM);
	pAddr->sin_addr.S_un.S_addr = (int)pAddressAttribute->GetIPAddress ();
	pAddr->sin_port = htons (pAddressAttribute->GetPort ());
	pAddr->sin_family = AF_INET;
	return true;
}
