

#ifndef _MSG_PKT_H_
#define _MSG_PKT_H_


#define MSG_MAXSIZE     (128)

typedef struct _msg_pkt_t
{
    uint8_t data[MSG_MAXSIZE];
    uint16_t dataLen;
    uint16_t seq;
    uint32_t inTime;
    uint32_t outTime;
    void *func_callback;
    uint8_t retryCnt;
    struct _msg_pkt_t *next;
}Msg_pkt_t;

typedef struct _msgPkt_queue_t
{
    Msg_pkt_t *outMsgPtr;
    Msg_pkt_t *last_outMsgPtr;
    uint16_t  outMsgNum;
}MsgPkt_queue_t;



#endif
