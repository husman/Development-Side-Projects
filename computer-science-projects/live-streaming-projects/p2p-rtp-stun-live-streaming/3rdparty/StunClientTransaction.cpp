#include "StdAfx.h"
#include "StunClientTransaction.h"

CStunClientTransaction::CStunClientTransaction (SOCKADDR_IN serverAddr, CStunBindingRequestMessage *pRequestMessage):
	CStunTransaction (serverAddr, pRequestMessage)
{
}

CStunClientTransaction::~CStunClientTransaction(void)
{
}

bool CStunClientTransaction::ReceiveResponse(int nResult)
{
	int nRetransmissionIntervals [] = {100, 300, 700, 1500, 3100, 4700, 6300, 7900, 9500};
	int nRetransmitCount = 1;
	
	int nMaxTries = 0;

#ifdef STRICT_IMPLEMENTATION
	nMaxTries = sizeof (nRetransmissionIntervals)/sizeof (*nRetransmissionIntervals);
#else
	nMaxTries = 4;
#endif

	nResult = 0;
	fd_set fdRead;
	timeval timeInterval = {0, 0};

	while (nRetransmitCount < nMaxTries)
	{
		FD_ZERO (&fdRead);
		FD_SET (m_SendSock, &fdRead);

		timeInterval.tv_usec = nRetransmissionIntervals[nRetransmitCount] * 1000;
		if ((nResult = select (0, &fdRead, NULL, NULL, &timeInterval)) 
			== SOCKET_ERROR)
		{
			nResult = WSAGetLastError ();
			clog << endl << "An error occured in select operation: " << "WSAGetLastError () = " << 
				nResult << ", line number = " << __LINE__  << ", in " << __FILE__ << endl;
			return false;		
		}

		if (nResult > 0 && FD_ISSET (m_SendSock, &fdRead))
		{
			if (ReceiveStunMessage (&m_pMessageReceived, nResult) == false)
			{
				return false;
			}

			if (ValidateMessage () == false)
			{
				return false;
			}

#ifdef STRICT_IMPLEMENTATION
			//We now wait for ten seconds and make sure that all responses received during this
			//are valid and the number of responses are not more than the number of requests
			//sent out
			if (WaitAndValidate (nResult) == false)
			{
				delete m_pMessageReceived;
				m_pMessageReceived = NULL;
				return false;
			}
#endif
			//Check whether the response is an error response, if it's then act accordingly
			if (m_pMessageReceived->GetMessageType () == BINDING_ERROR_RESPONSE ||
				m_pMessageReceived->GetMessageType () == SHARED_SECRET_ERROR_RESPONSE)
			{
				CStunErrorResponseMessage *pErrorResponseMessage = 
					(CStunErrorResponseMessage *)m_pMessageReceived;
				
				int nErrorCode = 0;
				if (pErrorResponseMessage->GetErrorCode (nErrorCode) == false)
				{
					return true;
				}

				switch (nErrorCode)
				{
				case 430:
					//TODO: Obtain a new shared secret key and retry with a new transaction
					break;

				case 432:
					//TODO: Add the username attribute
					break;

				case 401:
					//TODO: Add the message integrity attribute
					break;

				case 420:
					//The server couldn't understand a mandatory attribute
					//We don't retry the request and return
					break;
				}
			}

			return true;
		}
		else
		{
			++nRetransmitCount;
			clog << endl << "Time out occured waiting for response, doing a re-transmission number: " << std::dec << 
				nRetransmitCount - 1 << endl;
		
			if (SendStunMessage (nResult) == false)
			{
				clog << endl << "Failed to re-transmit the message. Aborting the transaction." << endl;
				return false;
			}
		}

		timeInterval.tv_usec = nRetransmissionIntervals [nRetransmitCount];
	}

	clog << endl << "Failed to receive a response after maximum re-transmissions. Transaction aborted." << endl;
	return false;
}

bool CStunClientTransaction::SendRequest (int nResult)
{
	nResult = 0;

	if (SendStunMessage (nResult) == false)
	{
		return false;
	}

	if (ReceiveResponse (nResult) == false)
	{
		return false;
	}

	return true;
}

CStunBindingResponseMessage *CStunClientTransaction::GetBindingResponse()
{
	if (m_pMessageReceived && m_pMessageReceived->GetMessageType () == BINDING_RESPONSE)
	{
		return (CStunBindingResponseMessage *)m_pMessageReceived;
	}

	return NULL;
}

STUN_MESSAGE_TYPE CStunClientTransaction::GetResponseType()
{
	if (m_pMessageReceived)
	{
		return m_pMessageReceived->GetMessageType ();
	}

	return -1;
}

CStunErrorResponseMessage *CStunClientTransaction::GetErrorResponse()
{
	if (m_pMessageReceived && m_pMessageReceived->GetMessageType () != BINDING_RESPONSE)
	{
		return (CStunErrorResponseMessage *)m_pMessageReceived;
	}

	return NULL;
}

//This function waits for ten seconds and makes sure that all the responses received in 
//this interval are consistent to the first response received
bool CStunClientTransaction::WaitAndValidate(int nResult)
{
	SYSTEMTIME CurrentTime, StartingTime;
	::GetSystemTime (&StartingTime);
	
	int nElapsedSeconds = 0;
	bool bRet = true, bFlag = false;

	fd_set fdRead;
	timeval timeInterval = {0, 0};

	CStunMessage *pPendingResponse = NULL;
	do
	{
		::GetSystemTime (&CurrentTime);
		nElapsedSeconds = CurrentTime.wSecond - StartingTime.wSecond;

		if (nElapsedSeconds < 0)
		{
			nElapsedSeconds += 60;
		}

		if (nElapsedSeconds == 10)
		{
			//Ten seconds wait is completed so break out of the loop
			bFlag = true;
		}

		FD_ZERO (&fdRead);
		FD_SET (m_SendSock, &fdRead);

		timeInterval.tv_sec = 10 - nElapsedSeconds;

		if ((nResult = select (0, &fdRead, NULL, NULL, &timeInterval))
			== SOCKET_ERROR)
		{
			clog << endl << "An error occured in select operation: " << "WSAGetLastError () = " << 
				WSAGetLastError () << ", line number = " << __LINE__  << ", in " << __FILE__ << endl;
			bFlag = true;		
			bRet = false;
		}

		if (nResult > 0 && FD_ISSET (m_SendSock, &fdRead))
		{			
			clog << endl << "Received a pending response" << endl;
			if (ReceiveStunMessage (&pPendingResponse, nResult) == false)
			{
				clog << endl << "Error in receiving pending responses" << endl;
				bRet = false;
				bFlag = true;
			}

			//Ugly way to compare the messages
			if (strcmp (pPendingResponse->GetBuffer (), m_pMessageReceived->GetBuffer ()) != 0)
			{
				clog << endl << "Pending response doesn't match the first response. Discarding all the received responses." << 
					endl;
				bRet = false;
				bFlag = true;
			}

			clog << endl << "Pending response matches the first received response" << endl;
			delete pPendingResponse;
		}
	}
	while (bFlag == false);

	return bRet;
}
