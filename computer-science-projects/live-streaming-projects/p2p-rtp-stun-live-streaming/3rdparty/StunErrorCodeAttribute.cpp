#include "stdafx.h"
#include "StunErrorCodeAttribute.h"

CStunErrorCodeAttribute::CStunErrorCodeAttribute(unsigned short nErrorCode, const char *pszReasonPhrase):
	CStunAttribute (ERROR_CODE), m_nErrorCode (nErrorCode)
{
	m_byNumber = nErrorCode % 100;
	m_byClass = nErrorCode / 100;

	if (pszReasonPhrase == NULL)
	{
		switch (m_nErrorCode)
		{
			case 400:
				pszReasonPhrase = "The request was malformed.  The client should not retry the request without modification from the previous attempt.";
				break;

			case 401: 
				pszReasonPhrase = "The Binding Request did not contain a MESSAGE-INTEGRITY attribute.";
				break;

			case 420:
				pszReasonPhrase = "The server did not understand a mandatory attribute in the request.";
				break;

			case 430:
				pszReasonPhrase = "The Binding Request did contain a MESSAGE-INTEGRITY attribute, but it used a shared secret that has expired. The client should obtain a new shared secret and try again.";
				break;

			case 431:
				pszReasonPhrase = "The Binding Request contained a MESSAGE-INTEGRITY attribute, but the HMAC failed verification. This could be a sign of a potential attack, or client implementation error.";
				break;

			case 432:
				pszReasonPhrase = "The Binding Request contained a MESSAGE-INTEGRITY attribute, but not a USERNAME attribute. Both must be present for integrity checks.";
				break;

			case 433:
				pszReasonPhrase = "The Shared Secret request has to be sent over TLS, but was not received over TLS.";
				break;

			case 500:
				pszReasonPhrase = "The server has suffered a temporary error. The client should try again.";
				break;

			case 600:
				pszReasonPhrase = "The server is refusing to fulfill the request. The client should not retry.";
				break;

			default:
				pszReasonPhrase = NULL;
				break;
		}
	}	
	
	int nSize = strlen (pszReasonPhrase);
	m_pszReasonPhrase = new char [nSize + 1];
	strncpy_s (m_pszReasonPhrase, nSize + 1, pszReasonPhrase, nSize);

	m_nAttributeLength = 4 + nSize;
}

CStunErrorCodeAttribute::CStunErrorCodeAttribute(char *pBuffer):
	CStunAttribute(pBuffer)
{
	char *ptrToBuffer = pBuffer + 6;

	memcpy_s (&m_byClass, sizeof(m_byClass), ptrToBuffer, sizeof(m_byClass));
	ptrToBuffer += sizeof(m_byClass);

	memcpy_s (&m_byNumber, sizeof(m_byNumber), ptrToBuffer, sizeof(m_byNumber));
	ptrToBuffer += sizeof(m_byNumber);

	m_pszReasonPhrase = new char [m_nAttributeLength];
	memcpy_s (m_pszReasonPhrase, m_nAttributeLength, pBuffer, m_nAttributeLength);
}

CStunErrorCodeAttribute::~CStunErrorCodeAttribute(void)
{
	if (m_pszReasonPhrase)
	{
		delete []m_pszReasonPhrase;
	}
}

unsigned char CStunErrorCodeAttribute::GetClass(void)
{
	return m_byClass;
}

unsigned char CStunErrorCodeAttribute::GetNumber(void)
{
	return m_byNumber;
}

void CStunErrorCodeAttribute::GetBuffer(char *pBuffer)
{
	unsigned short nAttributeType = htons (m_nAttributeType);
	unsigned short nAttributeLength = htons (GetAttributeLength ());
	unsigned short nNull = 0;

	int nOffset = 0, nSize = GetTotalLength ();

	memcpy_s (pBuffer, nSize, &nAttributeType, sizeof (nAttributeType));
	nOffset += sizeof (nAttributeType);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &nAttributeLength, sizeof (nAttributeLength));
	nOffset += sizeof (nAttributeLength);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &nNull, sizeof (nNull));
	nOffset += sizeof (nNull);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &m_byClass, sizeof (m_byClass));
	nOffset += sizeof (m_byClass);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, &m_byNumber, sizeof (m_byNumber));
	nOffset += sizeof (m_byNumber);

	memcpy_s (pBuffer + nOffset, nSize - nOffset, m_pszReasonPhrase, strlen (m_pszReasonPhrase));
}

const char* CStunErrorCodeAttribute::GetReasonPhrase()
{
	return m_pszReasonPhrase;
}

unsigned short CStunErrorCodeAttribute::GetErrorCode(void)
{
	return m_nErrorCode;
}

string CStunErrorCodeAttribute::ToString ()
{
	stringstream stream;
	stream << "Error code attribute: Error Code = " << m_nErrorCode << ", Reason Phrase = " << m_pszReasonPhrase;
	
	return stream.str ();
}
