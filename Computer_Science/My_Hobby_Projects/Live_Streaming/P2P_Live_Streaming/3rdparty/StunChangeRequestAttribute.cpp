#include "stdafx.h"
#include "StunChangeRequestAttribute.h"

CStunChangeRequestAttribute::CStunChangeRequestAttribute(unsigned int nChangeFlag):
	CStunAttribute (CHANGE_REQUEST), m_nChangeFlag(nChangeFlag)
{
	m_nAttributeLength = sizeof (m_nChangeFlag);
}

CStunChangeRequestAttribute::~CStunChangeRequestAttribute(void)
{
}

CStunChangeRequestAttribute::CStunChangeRequestAttribute(char *pBuffer):
	CStunAttribute (pBuffer)
{
	char *ptrToBuffer = pBuffer + 4;

	memcpy_s (&m_nChangeFlag, sizeof(m_nChangeFlag), ptrToBuffer, sizeof(m_nChangeFlag));
	m_nChangeFlag = ntohl (m_nChangeFlag);
}

void CStunChangeRequestAttribute::GetBuffer(char *pBuffer)
{
	unsigned short nAttributeType = htons (m_nAttributeType);
	unsigned short nAttributeLength = htons (GetAttributeLength ());
	unsigned int nChangeFlag = htonl (m_nChangeFlag);

	int nOffset = 0, nSize = GetTotalLength ();

	memcpy_s (pBuffer, nSize, &nAttributeType, sizeof (nAttributeType));
	nOffset += sizeof (nAttributeType);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &nAttributeLength, sizeof (nAttributeLength));
	nOffset += sizeof (nAttributeLength);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &nChangeFlag, sizeof (nChangeFlag));
}

unsigned int CStunChangeRequestAttribute::GetChangeRequestFlag(void)
{
	return m_nChangeFlag;
}

string CStunChangeRequestAttribute::ToString ()
{
	string strAttribute  = "Change request attribute: ";
	if (m_nChangeFlag == CHANGE_IP)
	{
		strAttribute += "Change IP";
	}
	else
	{
		strAttribute += "Change Port";
	}
	return strAttribute;
}
