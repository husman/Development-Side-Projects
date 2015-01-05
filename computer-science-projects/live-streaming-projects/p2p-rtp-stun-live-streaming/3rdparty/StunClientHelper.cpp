#include "stdafx.h"
#include "StunClientHelper.h"

CStunClientHelper::CStunClientHelper(SOCKADDR_IN serverAddr): m_pClientTransaction (NULL), m_pBindingRequest (NULL),
	m_bInitialize (false)
{
	m_bInitialize = StunGlobals::Initialize ();
	m_serverAddr = serverAddr;
}

CStunClientHelper::CStunClientHelper (const char *pszServer): m_pClientTransaction (NULL), m_pBindingRequest (NULL),
	m_bInitialize (false)
{
	m_bInitialize = StunGlobals::Initialize ();

	if (m_bInitialize == false)
	{
		return;
	}

	hostent *pHostent = gethostbyname (pszServer);
	if (pHostent == NULL)
	{
		clog << endl << "gethostbyname returned an error: WSAGetLastError = " << WSAGetLastError ()
			<< ", line number = " << __LINE__  << ", in " << __FILE__ << endl;
		m_bInitialize = false;
		return;
	}

	memcpy_s (&m_serverAddr.sin_addr, sizeof (m_serverAddr.sin_addr), 
		pHostent->h_addr_list [0], sizeof (m_serverAddr.sin_addr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons (3478);
}

CStunClientHelper::~CStunClientHelper(void)
{
	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
	}

	StunGlobals::UnInitialize ();
}

bool CStunClientHelper::TestOne(SOCKADDR_IN serverAddr, SOCKADDR_IN sendFromAddr, CStunMessage **pResponseMessage)
{
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}
	
	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (serverAddr, m_pBindingRequest);
	m_pClientTransaction->BindTo (sendFromAddr);

	*pResponseMessage = NULL;
	int nResult = 0;

	//Send a binding request with no attributes
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetBindingResponse();
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetErrorResponse ();
	}

	return true;
}

bool CStunClientHelper::TestTwo(SOCKADDR_IN serverAddr, SOCKADDR_IN sendFromAddr, CStunMessage **pResponseMessage)
{
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}

	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (serverAddr, m_pBindingRequest);
	m_pClientTransaction->BindTo (sendFromAddr);

	*pResponseMessage = NULL;
	int nResult = 0;

	//Send a binding request with change IP and change port flag
	m_pBindingRequest->AddChangeRequestAttribute (CHANGE_PORT | CHANGE_IP);

	//Send the request
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetBindingResponse();
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetErrorResponse ();
	}

	return true;
}

bool CStunClientHelper::TestThree(SOCKADDR_IN serverAddr, SOCKADDR_IN sendFromAddr, CStunMessage **pResponseMessage)
{
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}

	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (serverAddr, m_pBindingRequest);
	m_pClientTransaction->BindTo (sendFromAddr);

	*pResponseMessage = NULL;
	int nResult = 0;

	//Send a binding request with change port attribute
	m_pBindingRequest->AddChangeRequestAttribute (CHANGE_PORT);

	//Send the request
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetBindingResponse();
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetErrorResponse ();
	}

	return true;
}

NAT_TYPE CStunClientHelper::GetNatType(GtkWidget *txt)
{
	char tmp_string[128];

	printf("Performing NAT detection...\n");

	if (m_bInitialize == false)
	{
		printf("There was an error in initialization\n");
		return ERROR_DETECTING_NAT;
	}

	bool bRet = false;
	
	SOCKADDR_IN sendFromAddr;
	char szHostName [MAX_PATH];
	if (gethostname (szHostName, MAX_PATH) == SOCKET_ERROR)
	{
		clog << endl << "gethostname returned an error: WSAGetLastError () = " << WSAGetLastError () 
			<< ", line number = " << __LINE__ << ", in " << __FILE__ << endl;
		return ERROR_DETECTING_NAT;
	}

	hostent *pHostent = gethostbyname (szHostName);
	if (pHostent == 0)
	{
		clog << endl << "gethostbyname returned an error: WSAGetLastError () = " << WSAGetLastError () << endl;
		return ERROR_DETECTING_NAT;
	}

	memcpy_s (&sendFromAddr.sin_addr, sizeof (sendFromAddr.sin_addr), 
		pHostent->h_addr_list [0], sizeof (sendFromAddr.sin_addr));

	sendFromAddr.sin_family = AF_INET;
	sendFromAddr.sin_port = GetRandomPort ();

	CStunMessage *pResponseMessage = NULL;

	printf("Performing test one...\n");
	clog << endl << "Performing test one" << endl;
	bRet = TestOne (m_serverAddr, sendFromAddr, &pResponseMessage);

	if (bRet == false)
	{
		//We didn't get any response so test one has failed
		//and the NAT type is UDP blocking firewall
		return FIREWALL_BLOCKS_UDP;
	}

	if (pResponseMessage->GetMessageType () != BINDING_RESPONSE)
	{
		//We got an error response so just return -1
		printf("Received an error response: %s.\n", pResponseMessage->ToString().c_str());
		clog << endl << "Received an error response:" << pResponseMessage->ToString();
		return ERROR_DETECTING_NAT;
	}

	SOCKADDR_IN mappedAddress = {0}, sourceAddress = {0}, changedAddress = {0}, mappedAddress1 = {0};
	((CStunBindingResponseMessage *)pResponseMessage)->GetMappedAddress (&mappedAddress);
	((CStunBindingResponseMessage *)pResponseMessage)->GetSourceAddress (&sourceAddress);
	((CStunBindingResponseMessage *)pResponseMessage)->GetChangedAddress (&changedAddress);

	//Now match the mapped address with the source address
	if (mappedAddress.sin_port == sourceAddress.sin_port &&
		mappedAddress.sin_family == sourceAddress.sin_family &&
		mappedAddress.sin_addr.S_un.S_addr == sourceAddress.sin_addr.S_un.S_addr)
	{
		//We are not natted, now we try to determine whether we are behind
		//a symmetric UDP firewall or on open Internet

		printf("Performing test two...\n");
		clog << endl << "Performing test two" << endl;
		bRet = TestTwo (m_serverAddr, sendFromAddr, &pResponseMessage);

		if (bRet == true)
		{
			//We got a response
			return OPEN_INTERNET;
		}
		else
		{
			return SYMMETRIC_UDP_FIREWALL;
		}
	}
	else
	{
		//We are behind NAT
		bRet = TestTwo (m_serverAddr, sendFromAddr, &pResponseMessage);

		if (bRet == true)
		{
			//We got a response so we are behind full cone NAT
			return FULL_CONE_NAT;
		}
		else
		{
			//No response
			//Perform test one with the changed address received in previous test one execution
			printf("Performing test one by sending request to the changed address which was sent in response to previous request to test one\n");
			clog << endl << "Performing test one by sending request to the changed address which was sent in response to previous request to test one" << endl;
			bRet = TestOne (changedAddress, sendFromAddr, &pResponseMessage);
		
			if (bRet == false)
			{				
				return ERROR_DETECTING_NAT;
			}

			if (pResponseMessage->GetMessageType () != BINDING_RESPONSE)
			{				
				clog << endl << "Received an error response:" << pResponseMessage->ToString ();
				return ERROR_DETECTING_NAT;
			}

			//Got a response
			((CStunBindingResponseMessage *)pResponseMessage)->GetMappedAddress (&mappedAddress1);

			if (mappedAddress.sin_port != mappedAddress1.sin_port ||
				mappedAddress.sin_family != mappedAddress1.sin_family ||
				mappedAddress.sin_addr.S_un.S_addr != mappedAddress1.sin_addr.S_un.S_addr)
			{
				return SYMMETRIC_NAT;
			}

			//IP's matched so perform test three
			clog << endl << "Performing test three" << endl;
			bRet = TestThree (m_serverAddr, sendFromAddr, &pResponseMessage);

			if (bRet == false)
			{
				//No response
				return RESTRICTED_PORT_CONE_NAT;
			}
			else
			{
				return RESTRICTED_CONE_NAT;
			}
		}
	}
}

bool CStunClientHelper::GetStunMappedAddress (SOCKADDR_IN *pAddr)
{
	clog << endl << "Getting STUN mapped IP" << endl;
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}
	
	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (m_serverAddr, m_pBindingRequest);

	int nResult = 0;

	//Send a binding request with no attributes
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		if (m_pClientTransaction->GetBindingResponse ()->GetMappedAddress (pAddr) == false)
		{
			clog << endl << "A binding response was received, but there was an error in parsing the response." << endl;
			return false;
		}
		return true;
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		unsigned short nErrorCode = 0;
		if (m_pClientTransaction->GetErrorResponse ()->GetErrorCode (nErrorCode) == false)
		{
			clog << endl << "An error response was received, but there was an error in parsing the response." << endl;
			return false;
		}

		clog << endl << "An error response was received: Error code = " << nErrorCode << endl;
		return false;
	}

	return true;
}

unsigned int CStunClientHelper::GetRandomPort ()
{
	SYSTEMTIME time;
	::GetSystemTime (&time);

	srand (time.wMilliseconds);
	return rand () + 10000;
}
