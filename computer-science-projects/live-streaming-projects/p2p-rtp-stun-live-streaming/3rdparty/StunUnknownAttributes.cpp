#include "StdAfx.h"
#include "StunUnknownAttributes.h"

CStunUnknownAttributes::CStunUnknownAttributes(void):CStunAttribute(UNKNOWN_ATTRIBUTES), m_nAttributesCount(0)
{
	m_nAttributeLength = 0;

	for (int i = 0; i < MAX_UNKNOWN_ATTRIBUTES; ++i)
	{
		m_UnknownAttributes [i] = 0;
	}
}

CStunUnknownAttributes::~CStunUnknownAttributes(void)
{
}

CStunUnknownAttributes::CStunUnknownAttributes(char *pBuffer):CStunAttribute (pBuffer)
{
	char *ptrToBuffer = pBuffer + 4;
	int nAttributesCount = m_nAttributeLength / 2;
	unsigned short nAttributeType = 0;

	for (int i = 0; i < nAttributesCount; ++i)
	{
		memcpy_s (&nAttributeType, sizeof (nAttributeType), ptrToBuffer, sizeof (nAttributeType));
		m_UnknownAttributes [i] = ntohs (nAttributeType);
	}	
}

bool CStunUnknownAttributes::AddAttribute(STUN_ATTRIBUTE_TYPE nAttributeType)
{
	if (m_nAttributesCount == MAX_UNKNOWN_ATTRIBUTES - 1)
	{
		return false;
	}

	m_UnknownAttributes [m_nAttributesCount++] = nAttributeType;

	unsigned short nCount = m_nAttributesCount;
	if ((nCount % 2) == 1)
	{
		nCount += 1;
	}

	m_nAttributeLength = nCount * sizeof(m_UnknownAttributes [0]);

	return true;
}

void CStunUnknownAttributes::GetBuffer (char *pBuffer)
{
	unsigned short nAttributeType = htons (m_nAttributeType);
	unsigned short nAttributeLength = htons (GetAttributeLength ());

	int nOffset = 0, nSize = GetTotalLength ();

	memcpy_s (pBuffer, nSize, &nAttributeType, sizeof (nAttributeType));
	nOffset += sizeof (nAttributeType);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &nAttributeLength, sizeof (nAttributeLength));
	nOffset += sizeof (nAttributeLength);

	for (int i = 0; i < m_nAttributesCount; ++i)
	{
		nAttributeType = htons (m_UnknownAttributes [i]);

		memcpy_s (pBuffer + nOffset, nSize - nOffset, &nAttributeType, sizeof (nAttributeType));
		nOffset += sizeof (nAttributeType);		
	}

	if ((m_nAttributesCount % 2) == 1)
	{
		memcpy_s (pBuffer + nOffset, nSize - nOffset, &nAttributeType, sizeof (nAttributeType));
	}
}

int CStunUnknownAttributes::GetUnknownAttributesCount(void)
{
	return m_nAttributesCount;
}

unsigned short* CStunUnknownAttributes::GetUnknownAttributes(void)
{
	return m_UnknownAttributes;
}

string CStunUnknownAttributes::ToString ()
{
	unsigned short nAttributeType;
	string strUnknownAttributes;
	stringstream stream;
	stream << "Unknown attributes: ";

	for (int i = 0; i < m_nAttributesCount; ++i)
	{		
		nAttributeType = ntohs(m_UnknownAttributes [i]);				
		stream << std::showbase << std::hex << nAttributeType << ", ";
	}

	strUnknownAttributes = stream.str ();
	strUnknownAttributes = strUnknownAttributes.substr (0, strUnknownAttributes.length () - 2);

	return strUnknownAttributes;
}