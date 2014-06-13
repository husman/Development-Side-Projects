#pragma once
#include "stunbindingmessage.h"

class CStunBindingResponseMessage :
	public CStunBindingMessage
{
public:
	CStunBindingResponseMessage(CStunAddressAttribute *pMappedAddressAttribute, 
								CStunAddressAttribute *pSourceAddressAttribute,
								CStunAddressAttribute *pChangedAddressAttribute);
	CStunBindingResponseMessage (char *pBuffer);

	~CStunBindingResponseMessage(void);

	void AddReflectedFromAddressAttribute(unsigned short nPort, const char *pszAddress);

	bool GetMappedAddress(SOCKADDR_IN *pAddr);
	bool GetSourceAddress(SOCKADDR_IN *pAddr);
	bool GetChangedAddress(SOCKADDR_IN *pAddr);
	bool GetReflectedFromAddress(SOCKADDR_IN *pAddr);
};
