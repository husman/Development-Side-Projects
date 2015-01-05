#pragma once
#include "StunMessage.h"
#include "StunClientTransaction.h"
#include "StunBindingRequestMessage.h"
#include "StunClientTransaction.h"
#include "StunGlobals.h"
#include "globals.h"

enum NAT_TYPE
{
	ERROR_DETECTING_NAT = -1,
	OPEN_INTERNET,
	FIREWALL_BLOCKS_UDP,
	SYMMETRIC_UDP_FIREWALL,
	FULL_CONE_NAT,
	SYMMETRIC_NAT,
	RESTRICTED_CONE_NAT,
	RESTRICTED_PORT_CONE_NAT,
};

class CStunClientHelper
{
public:
	CStunClientHelper(SOCKADDR_IN serverAddr);
	CStunClientHelper(const char *pszServer);

	~CStunClientHelper(void);

	bool TestOne (SOCKADDR_IN serverAddr, SOCKADDR_IN sendFromAddr, CStunMessage **pMessage);
	bool TestTwo (SOCKADDR_IN serverAddr, SOCKADDR_IN sendFromAddr, CStunMessage **pMessage);
	bool TestThree (SOCKADDR_IN serverAddr, SOCKADDR_IN sendFromAddr, CStunMessage **pMessage);

	NAT_TYPE GetNatType (GtkWidget *txt);

	bool GetStunMappedAddress (SOCKADDR_IN *pAddr);
	
private:
	SOCKADDR_IN m_serverAddr;
	CStunBindingRequestMessage *m_pBindingRequest;
	CStunClientTransaction *m_pClientTransaction;
	bool m_bInitialize;

	unsigned int GetRandomPort ();
};
