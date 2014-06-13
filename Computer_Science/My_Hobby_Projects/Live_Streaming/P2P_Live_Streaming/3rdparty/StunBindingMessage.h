#pragma once
#include "stunmessage.h"

class CStunBindingMessage :
	public CStunMessage
{
public:
	CStunBindingMessage(STUN_MESSAGE_TYPE MessageType);
	CStunBindingMessage(char *pBuffer);

	virtual ~CStunBindingMessage(void) = 0;
	
	void AddMessageIntegrityAttribute ();
};
