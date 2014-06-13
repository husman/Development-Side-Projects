#include "StdAfx.h"
#include "StunAttribute.h"

CStunAttribute::CStunAttribute(STUN_ATTRIBUTE_TYPE AttributeType):m_nAttributeType(AttributeType), 
																  m_nAttributeLength (0)
{
}

CStunAttribute::CStunAttribute (char *pBuffer)
{
	memcpy_s (&m_nAttributeType, sizeof (m_nAttributeType), pBuffer, sizeof (m_nAttributeType));
	memcpy_s (&m_nAttributeLength, sizeof (m_nAttributeLength), pBuffer + sizeof (m_nAttributeType), 
		sizeof (m_nAttributeLength));

	m_nAttributeType = ntohs (m_nAttributeType);
	m_nAttributeLength = ntohs (m_nAttributeLength);
}

CStunAttribute::~CStunAttribute(void)
{
}

STUN_ATTRIBUTE_TYPE CStunAttribute::GetAttributeType()
{
	return m_nAttributeType;
}

unsigned short CStunAttribute::GetTotalLength()
{
	return GetAttributeLength () + sizeof (m_nAttributeType) + 2;
}

unsigned short CStunAttribute::GetAttributeLength ()
{
	return m_nAttributeLength;
}