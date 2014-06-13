#include "StdAfx.h"
#include "StunPasswordAttribute.h"

CStunPasswordAttribute::CStunPasswordAttribute(const char *pszPassword):
	CStunAttribute(PASSWORD)
{
	m_nAttributeLength = strlen (pszPassword);
	m_pszPassword = new char [m_nAttributeLength + 1];
	strncpy_s (m_pszPassword, m_nAttributeLength + 1, pszPassword, m_nAttributeLength);
}

CStunPasswordAttribute::CStunPasswordAttribute(char *pBuffer, int nDummy):
	CStunAttribute(pBuffer)
{
	char *ptrToBuffer = pBuffer + 4;

	m_pszPassword = new char [m_nAttributeLength];
	memcpy_s (m_pszPassword, m_nAttributeLength, pBuffer, m_nAttributeLength);
}

CStunPasswordAttribute::~CStunPasswordAttribute(void)
{
	delete []m_pszPassword;
}

void CStunPasswordAttribute::GetBuffer (char *pBuffer)
{
	unsigned short nAttributeType = htons (m_nAttributeType);
	unsigned short nAttributeLength = htons (GetAttributeLength ());

	int nOffset = 0, nSize = GetTotalLength ();

	memcpy_s (pBuffer, nSize, &nAttributeType, sizeof (nAttributeType));
	nOffset += sizeof (nAttributeType);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &nAttributeLength, sizeof (nAttributeLength));
	nOffset += sizeof (nAttributeLength);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, m_pszPassword, _tcslen (m_pszPassword));
}

const char *CStunPasswordAttribute::GetPassword ()
{
	return m_pszPassword;
}

string CStunPasswordAttribute::ToString()
{
	string strPasswordAttribute = string ("Password attribute: Password = ") + m_pszPassword;
	return strPasswordAttribute;
}