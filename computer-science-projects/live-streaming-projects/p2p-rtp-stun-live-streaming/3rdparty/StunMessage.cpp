#include "StdAfx.h"
#include "StunMessage.h"

CStunMessage::CStunMessage(STUN_MESSAGE_TYPE MessageType):m_StunHeader (MessageType), m_bValidMessage (true)
{
	for (int i = 0; i < MAX_ATTRIBUTES; ++i)
	{
		m_pArrAttributes [i] = NULL;
	}
}

CStunMessage::~CStunMessage(void)
{
	for (int i = 0; i < MAX_ATTRIBUTES; ++i)
	{
		delete m_pArrAttributes [i];
	}
}

CStunMessage::CStunMessage(char *pBuffer):m_StunHeader(pBuffer), m_bValidMessage (true)
{
	for (int i = 0; i < MAX_ATTRIBUTES; ++i)
	{
		m_pArrAttributes [i] = NULL;
	}

	char *ptrToBuffer = pBuffer + m_StunHeader.GetHeaderLength ();

	int nSize = m_StunHeader.GetMessageLength (), nBytesRead = 0; 
	
	STUN_ATTRIBUTE_TYPE nAttributeType = 0;
	unsigned short nAttributeLength = 0;
	
	CStunAttribute *pStunAttribute = NULL;

	while (nBytesRead < nSize)
	{
		memcpy_s (&nAttributeType, sizeof (nAttributeType), ptrToBuffer, sizeof (nAttributeType));
		memcpy_s (&nAttributeLength, sizeof (nAttributeLength), ptrToBuffer + sizeof (nAttributeType), 
			sizeof (nAttributeLength));

		nAttributeLength = ntohs (nAttributeLength);
		nAttributeType = ntohs (nAttributeType);

		switch (nAttributeType)
		{
			case MAPPED_ADDRESS:
				pStunAttribute = new CStunAddressAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case RESPONSE_ADDRESS:
				pStunAttribute = new CStunAddressAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case CHANGE_REQUEST:
				pStunAttribute = new CStunChangeRequestAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case SOURCE_ADDRESS:
				pStunAttribute = new CStunAddressAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case CHANGED_ADDRESS:
				pStunAttribute = new CStunAddressAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case USERNAME:
				pStunAttribute = new CStunUsernameAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case PASSWORD:
				pStunAttribute = new CStunPasswordAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case MESSAGE_INTEGRITY:
				//pStunAttribute = new CStunMessageInAttribute (ptrToBuffer);
				//AddAttribute (pStunAttribute);
				break;

			case ERROR_CODE:
				pStunAttribute = new CStunErrorCodeAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case UNKNOWN_ATTRIBUTES:
				pStunAttribute = new CStunUnknownAttributes (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			case REFLECTED_FROM:
				pStunAttribute = new CStunAddressAttribute (ptrToBuffer);
				AddAttribute (pStunAttribute);
				break;

			default:
				if (nAttributeType > 0x7fff)
				{
					clog << "Unknown Attribute: " << std::showbase << std::hex << nAttributeType << endl;
				}
				else if (nAttributeType <= 0x7fff)
				{
					clog << "Invalid Attribute: " << std::showbase << std::hex << nAttributeType << 
						", discarding the message." << endl;
					m_bValidMessage = false;
				}
				break;
		}

		ptrToBuffer = ptrToBuffer + (nAttributeLength + 4);
		nBytesRead = nBytesRead + nAttributeLength + 4;
	}
}

char *CStunMessage::GetBuffer()
{
	char *Buffer = new char [GetTotalLength ()];
	char *ptrBuffer = Buffer;

	m_StunHeader.SetMessageLength (GetTotalLength () - m_StunHeader.GetHeaderLength ());
	m_StunHeader.GetBuffer (ptrBuffer);
	ptrBuffer += m_StunHeader.GetHeaderLength ();

	for (int i = 0; i < MAX_ATTRIBUTES; ++i)
	{
		if (m_pArrAttributes [i])
		{
			m_pArrAttributes[i]->GetBuffer (ptrBuffer);
			ptrBuffer += m_pArrAttributes[i]->GetTotalLength ();
		}
	}

	string s = m_StunHeader.ToString ();
	return Buffer;
}

unsigned short CStunMessage::GetTotalLength ()
{
	unsigned short nTotalLength = 0;

	nTotalLength += m_StunHeader.GetHeaderLength ();

	for (int i = 0; i < MAX_ATTRIBUTES; ++i)
	{
		if (m_pArrAttributes [i])
		{
			nTotalLength += m_pArrAttributes [i]->GetTotalLength();
		}
	}

	return nTotalLength;
}

STUN_MESSAGE_TYPE CStunMessage::GetMessageType ()
{
	return m_StunHeader.GetMessageType ();
}

void CStunMessage::RemoveAttribute(STUN_ATTRIBUTE_TYPE nAttributeType)
{
	if (m_pArrAttributes [nAttributeType] != NULL)
	{
		delete m_pArrAttributes [nAttributeType];
	}
}

void CStunMessage::AddAttribute(CStunAttribute *pStunAttribute)
{
	STUN_ATTRIBUTE_TYPE nAttributeType = pStunAttribute->GetAttributeType ();

	if (m_pArrAttributes [nAttributeType] != NULL)
	{
		delete m_pArrAttributes [nAttributeType];
	}

	m_pArrAttributes [nAttributeType] = pStunAttribute;
}

CStunAttribute *CStunMessage::GetAttribute(STUN_ATTRIBUTE_TYPE nAttributeType)
{
	return m_pArrAttributes [nAttributeType];
}

bool CStunMessage::HasAttribute (STUN_ATTRIBUTE_TYPE nAttributeType)
{
	if (m_pArrAttributes [nAttributeType] != NULL)
	{
		return true;
	}

	return false;
}

bool CStunMessage::IsMessageValid ()
{
	return m_bValidMessage;
}

string CStunMessage::ToString ()
{
	string strMessage;
	
	strMessage += m_StunHeader.ToString () + "\n";

	for (int i = 0; i < MAX_ATTRIBUTES; ++i)
	{
		if (m_pArrAttributes [i])
		{
			strMessage += m_pArrAttributes [i]->ToString () + "\n";
		}
	}

	return strMessage;
}