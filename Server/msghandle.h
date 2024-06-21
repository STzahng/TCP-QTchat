#ifndef MSGHANDLE_H
#define MSGHANDLE_H
#include "protocol.h"

#include <QString>

class MsgHandle
{
public:
    MsgHandle();
    PDU* REGIST(PDU* pdu);
    PDU* LOGIN(PDU* pdu,QString& m_strLoginname);
    PDU* FindUser(PDU* pdu);
    PDU* OnlineUser(PDU* pdu);
    PDU* AddFriend(PDU* pdu);
    PDU* AddFriendAgree(PDU* pdu);
    PDU* FlushFriend(PDU* pdu);
    PDU* DelFriend(PDU* pdu);
};

#endif // MSGHANDLE_H
