#include "StdAfx.h"
#include "StunGlobals.h"

bool StunGlobals::UnInitialize ()
{
#ifdef LOG_TO_FILE
	if (m_LogFile.is_open () == true)
	{
		m_LogFile.close ();
	}
#endif
	if (WSACleanup () == SOCKET_ERROR)
	{
		clog << endl << "WSACleanup returned an error: WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}

	return true;
}

//Returns you a pointer to the CStunMessage which is created by
//peeking into the pBuffer and thus determining the appropriate type
CStunMessage* StunGlobals::CreateMessage(char *pBuffer)
{
	//Not a very elegant way to do things but here we peek into the header
	//and determine the STUN message type
	unsigned short nMessageType = 0;
	memcpy_s (&nMessageType, sizeof (nMessageType), pBuffer, sizeof (nMessageType));
	nMessageType = ntohs (nMessageType);

	//Now we create the corresponding message (this is not the most elegant way to do
	//things, but I can't think of a better way :|)
	CStunMessage *pMessage = NULL;

	switch (nMessageType)
	{
		case BINDING_REQUEST:
			pMessage = new CStunBindingRequestMessage (pBuffer);
			break;

		case BINDING_RESPONSE:
			pMessage = new CStunBindingResponseMessage (pBuffer);
			break;

		case BINDING_ERROR_RESPONSE:
			pMessage = new CStunBindingErrorResponseMessage (pBuffer);
			break;

		case SHARED_SECRET_ERROR_RESPONSE:
			pMessage = new CStunSharedSecretErrorResponseMessage (pBuffer);
			break;

		case SHARED_SECRET_REQUEST:
			pMessage = new CStunSharedSecretRequestMessage (pBuffer);
			break;

		case SHARED_SECRET_RESPONSE:
			pMessage = new CStunSharedSecretResponseMessage (pBuffer);
			break;

		default:
			clog << endl << "CStunMessage::CreateMessage (): Unknown message type." << endl;
			break;
	}

	return pMessage;
}

bool StunGlobals::Initialize()
{
#ifdef LOG_TO_FILE
	m_LogFile.open ("stunner.log", ios_base::out | ios_base::trunc);
	if (m_LogFile.is_open () == false)
	{
		return false;
	}
	clog.rdbuf (m_LogFile.rdbuf ());
#endif
	
	int nRet = 0;
	WSADATA wsaData;

	nRet = WSAStartup (MAKEWORD (2, 2), &wsaData);
	if (nRet != 0)
	{
		nRet = WSAGetLastError ();
		clog << endl << "WSAStartup returned an error: " << "WSAGetLastError () = " << WSAGetLastError () << endl;
		return false;
	}

	return true;
}