#include "StdAfx.h"
#include "StunBindingMessage.h"

CStunBindingMessage::CStunBindingMessage(STUN_MESSAGE_TYPE MessageType): CStunMessage(MessageType)
{
}

CStunBindingMessage::~CStunBindingMessage(void)
{
}

CStunBindingMessage::CStunBindingMessage(char *pBuffer):CStunMessage (pBuffer)
{
}

void CStunBindingMessage::AddMessageIntegrityAttribute()
{
}
