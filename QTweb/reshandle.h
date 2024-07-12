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
    void FLUSH_FRIEND_RESPOND(PDU* pdu);
    void DEL_FRIEND_RESPOND(PDU* pdu);
    void CHAT_REQUEST(PDU* pdu);
    void MKDIR_RESPOND(PDU* pdu);
    void FLUSH_FILE_RESPOND(PDU* pdu);
    void DEL_DIR_RESPOND(PDU* pdu);
    void DEL_FILE_RESPOND(PDU* pdu);
    void RENAME_RESPOND(PDU* pdu);
    void MOVE_FILE_RESPOND(PDU* pdu);
    void UPLOAD_FILE_RESPOND(PDU* pdu);
    void UPLOAD_DATA_RESPOND(PDU* pdu);
    void DOWNLOAD_FILE_RESPOND(PDU* pdu);
    void DOWNLOAD_DATA_RESPOND(PDU* pdu);
    void SHARE_FILE_RESPOND();
    void SHARE_FILE_REQUEST(PDU* pdu);
    void SHARE_FILE_AGREE_RESPOND(PDU* pdu);
};

#endif // RESHANDLE_H
