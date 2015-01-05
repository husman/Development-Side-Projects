#include "stdafx.h"
#include "StunAddressAttribute.h"

CStunAddressAttribute::CStunAddressAttribute(unsigned short nAttributeType, unsigned char byAddressFamily,
							   unsigned short nPort, const char *pszIPAddress): 
								CStunAttribute(nAttributeType),
								m_nPort (nPort), 
								m_nIPAddress (inet_addr (pszIPAddress)), 
								m_byAddressFamily (byAddressFamily)
{
	Initialize ();	
}

CStunAddressAttribute::CStunAddressAttribute (unsigned short nAttributeType, SOCKADDR_IN addr):
	CStunAttribute (nAttributeType), m_nPort (addr.sin_port), m_byAddressFamily (addr.sin_family), 
	m_nIPAddress (addr.sin_addr.S_un.S_addr)
{
	Initialize ();
}

CStunAddressAttribute::~CStunAddressAttribute(void)
{
}

CStunAddressAttribute::CStunAddressAttribute(char *pBuffer):CStunAttribute(pBuffer)
{
	char *ptrToBuffer = pBuffer + 5;

	memcpy_s (&m_byAddressFamily, sizeof(m_byAddressFamily), ptrToBuffer, sizeof(m_byAddressFamily));
	ptrToBuffer += sizeof(m_byAddressFamily);

	memcpy_s (&m_nPort, sizeof(m_nPort), ptrToBuffer, sizeof(m_nPort));
	ptrToBuffer += sizeof(m_nPort);
	m_nPort = ntohs (m_nPort);

	memcpy_s (&m_nIPAddress, sizeof(m_nIPAddress), ptrToBuffer, sizeof(m_nIPAddress));
	ptrToBuffer += sizeof(m_nIPAddress);

	in_addr addr;
	addr.S_un.S_addr = (int)m_nIPAddress;
}

unsigned short CStunAddressAttribute::GetPort(void)
{
	return m_nPort;
}

unsigned char CStunAddressAttribute::GetAddressFamily(void)
{
	return m_byAddressFamily;
}

unsigned int CStunAddressAttribute::GetIPAddress()
{
	return m_nIPAddress;
}

unsigned short CStunAddressAttribute::GetAttributeLength()
{
	return m_nAttributeLength;
}

void CStunAddressAttribute::GetBuffer(char *Buffer)
{
	unsigned short nAttributeType = htons (m_nAttributeType);
	unsigned short nAttributeLength = htons (GetAttributeLength ());
	unsigned char cTemp = htons (0);
	unsigned char byAddressFamily = m_byAddressFamily;
	unsigned short nPort = htons (m_nPort);
	unsigned int nIPAddress = m_nIPAddress;

	int nOffset = 0, nSize = GetTotalLength ();

	memcpy_s (Buffer, nSize, &nAttributeType, sizeof (nAttributeType));
	nOffset += sizeof (nAttributeType);

	memcpy_s (Buffer + nOffset, nSize - nOffset, &nAttributeLength, sizeof (nAttributeLength));
	nOffset += sizeof (nAttributeLength);

	memcpy_s (Buffer + nOffset, nSize - nOffset, &cTemp, sizeof (cTemp));
	nOffset += sizeof (cTemp);

	memcpy_s (Buffer + nOffset, nSize - nOffset, &byAddressFamily, sizeof (byAddressFamily));
	nOffset += sizeof (byAddressFamily);

	memcpy_s (Buffer + nOffset, nSize - nOffset, &nPort, sizeof (nPort));
	nOffset += sizeof (nPort);

	memcpy_s (Buffer + nOffset, nSize - nOffset, &nIPAddress, sizeof (nIPAddress));
}

void CStunAddressAttribute::Initialize(void)
{
	m_nAttributeLength = (sizeof (m_nPort) + sizeof (m_nIPAddress) + sizeof (m_byAddressFamily) + 1);
}

string CStunAddressAttribute::ToString ()
{
	stringstream stream;

	switch (m_nAttributeType)
	{
	case MAPPED_ADDRESS:
		stream << "Mapped address attribute: ";
		break;
	case RESPONSE_ADDRESS:
		stream << "Response address attribute: ";
		break;
	case CHANGED_ADDRESS:
		stream << "Changed address attribute: ";
		break;
	case SOURCE_ADDRESS:
		stream << "Source address attribute: ";
		break;
	case REFLECTED_FROM:
		stream << "Reflected from address attribute: ";
		break;
	default:
		stream << "Unknown attribute: ";
	}

	stream << std::showbase << std::hex << m_nAttributeType << ", Address Family: " << 
		std::showbase << std::hex << (int)m_byAddressFamily << ", Port: " << std::dec << m_nPort << ", IP Address: ";

	in_addr addr;
	addr.S_un.S_addr = (int)m_nIPAddress;
	stream << inet_ntoa (addr);	

	return 	stream.str();
}
