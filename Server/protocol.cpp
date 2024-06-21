#include "protocol.h"
#include <stdlib.h>
#include <QString>
#include <QDebug>
PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof (PDU) + uiMsgLen;
    PDU* pdu = (PDU*)malloc(uiPDULen);
    if(pdu == NULL){
        qDebug()<<"pdu error";
        exit(1);
    }
    memset(pdu,0,uiPDULen);
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgLen = uiMsgLen;
    return pdu;
}
