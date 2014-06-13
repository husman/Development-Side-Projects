#include "StdAfx.h"
#include "StunHeader.h"
#include <iomanip>

CStunHeader::CStunHeader(STUN_MESSAGE_TYPE nMessageType):m_nMessageType (nMessageType),
	m_nMessageLength (0)
{
	CreateTransactionId ();
}

CStunHeader::CStunHeader(char *pBuffer)
{
	unsigned short nMessageType = 0;
	unsigned short nMessageLength = 0;

	int nOffest = 0;
	memcpy_s (&nMessageType, sizeof (nMessageType), pBuffer + nOffest, sizeof (nMessageType));
	nOffest += sizeof (nMessageType);
	m_nMessageType = ntohs (nMessageType);

	memcpy_s (&nMessageLength, sizeof (nMessageLength), pBuffer + nOffest, sizeof (nMessageLength));
	nOffest += sizeof (nMessageType);
	m_nMessageLength = ntohs (nMessageLength);

	memcpy_s (m_cTransactionId, sizeof (m_cTransactionId), pBuffer + nOffest, sizeof (m_cTransactionId));
}

CStunHeader::~CStunHeader(void)
{
}

void CStunHeader::CreateTransactionId ()
{
	static int nCounter = 0;
	++nCounter;

	SYSTEMTIME CurrentTime;
	::GetSystemTime (&CurrentTime);

	srand (CurrentTime.wMilliseconds);
	for (int i = 0; i < 16; ++i)
	{
		m_cTransactionId [i] = rand ();
	}

	m_cTransactionId [0] = nCounter;
}

void CStunHeader::GetBuffer (char *pszBuffer)
{
	unsigned short nMessageType = htons (m_nMessageType);
	unsigned short nMessageLength = htons (m_nMessageLength);
	
	int nOffset = 0, nSize = GetHeaderLength ();

	memcpy_s (pszBuffer, nSize, &nMessageType, sizeof (nMessageType));
	nOffset += sizeof (nMessageType);

	memcpy_s (pszBuffer + nOffset, nSize - nOffset, &nMessageLength, sizeof (nMessageLength));
	nOffset += sizeof (nMessageLength);

	memcpy_s (pszBuffer + nOffset, nSize - nOffset, &m_cTransactionId, sizeof (m_cTransactionId));
}

unsigned short CStunHeader::GetHeaderLength ()
{
	unsigned short nLength = sizeof (m_nMessageLength) + sizeof (m_nMessageType) + 
		sizeof (m_cTransactionId);

	return nLength;
}

STUN_MESSAGE_TYPE CStunHeader::GetMessageType ()
{
	return m_nMessageType;
}

unsigned short CStunHeader::GetMessageLength()
{
	string s = ToString ();
	return m_nMessageLength;
}

void CStunHeader::SetMessageLength(unsigned short nMessageLength)
{
	m_nMessageLength = nMessageLength;
}

string CStunHeader::ToString()
{
	stringstream stream;
	stream << "STUN header: Message Type = " << std::showbase << std::hex << m_nMessageType 
		<< ", Message Length = " << std::dec << m_nMessageLength << ", Transaction ID = 0x";

	for (int i = 0; i < 16; ++i)
	{
		stream << std::noshowbase << std::hex << (int)m_cTransactionId [i];
	}

	return stream.str ();
}
