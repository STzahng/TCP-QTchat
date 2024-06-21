#ifndef RESHANDLE_H
#define RESHANDLE_H
#include "protocol.h"

class ResHandle
{
public:
    ResHandle();
    void REGIST_RESPOND(PDU* pdu);
    void LOGIN_RESPOND(PDU* pdu);
    void FIND_USER_RESPOND(PDU* pdu);
    void ONLINE_USER_RESPOND(PDU*pdu);
    void ADD_FRIEND_RESPOND(PDU* pdu);
    void ADD_FRINED_REQUEST(PDU* pdu);
    void ADD_FRIEND_AGREE_RESPOND();
    void ADD_FRIEND_AGREE_REQUEST();
    void FLUSH_FRIEND_RESPOND(PDU* pdu);
    void DEL_FRIEND_RESPOND(PDU* pdu);
};

#endif // RESHANDLE_H
