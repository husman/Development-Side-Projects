#pragma once

#include "StunMessage.h"

class CStunTransaction
{
protected:
	bool SendStunMessage (int nResult);
	bool ReceiveStunMessage (CStunMessage **pMessageReceived, int nResult);
	bool Initialize ();

	CStunMessage *m_pMessageToSend;
	CStunMessage *m_pMessageReceived;

	SOCKET m_SendSock;
	SOCKADDR_IN m_SendToAddr;
	int m_nPortToSendMessageFrom;

	//Checks if the message is correcrly formed, looks for message integrity and 
	//other attributes to detect an attack
	bool ValidateMessage();

public:
	CStunTransaction(SOCKADDR_IN sendToAddr, CStunMessage *pMessageToSend);
	CStunTransaction(SOCKADDR_IN sendToAddr, SOCKADDR_IN sendFromAddr, CStunMessage *pMessageToSend);
	virtual ~CStunTransaction(void) = 0;
	bool BindTo (SOCKADDR_IN sendFromAddr);
};
