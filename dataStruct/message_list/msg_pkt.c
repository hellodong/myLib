

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//#include "platform.h"
#include "msg_pkt.h"


#define LOG(fmt,arg...)  printf("%s() #%d "fmt, __FUNCTION__,__LINE__,##arg)


static int msgPkt_init(Msg_pkt_t **msg, uint8_t data[], uint8_t dataLen, uint16_t seq, uint32_t inTime, void *callback, uint8_t retry)
{
    if (dataLen > MSG_MAXSIZE) {
        return EMSGSIZE;
    }

    Msg_pkt_t *msgPtr = NULL;
    msgPtr = (Msg_pkt_t *)malloc(sizeof(Msg_pkt_t));
    if (NULL == msgPtr) {
        return ENOMEM;
    }
    
    memset(msgPtr, 0x00, sizeof(Msg_pkt_t));
    memcpy(msgPtr->data, data, dataLen);
    msgPtr->dataLen = dataLen;
    msgPtr->seq = seq;
    msgPtr->inTime = inTime;
    msgPtr->func_callback = callback;
    msgPtr->retryCnt = retry;
    msgPtr->next = NULL;
    *msg = msgPtr;
    return 0;
}

int msgPktQueue_add(MsgPkt_queue_t *queuePtr, uint8_t data[], uint8_t dataLen, uint16_t seq, uint32_t inTime, void *callback, uint8_t retry)
{
    Msg_pkt_t *msg = NULL;
    int retFunc;

    retFunc = msgPkt_init(&msg, data, dataLen, seq, inTime, callback, retry);
    if (retFunc) {
        return retFunc;
    }

    if (queuePtr->last_outMsgPtr){
        queuePtr->last_outMsgPtr->next = msg;
    }else{
        queuePtr->outMsgPtr = msg;
    }
    queuePtr->last_outMsgPtr = msg;
    queuePtr->outMsgNum++;

    return 0;
}

#if 0
int msgPktQueue_find_bySeq(MsgPkt_queue_t *queuePtr, uint16_t seq, Msg_pkt_t **pre, Msg_pkt_t **now)
{
   Msg_pkt_t *msg = queuePtr->outMsgPtr, *preMsg = NULL;
   while(msg) {
        if(msg->seq == seq) {
            *pre = preMsg;
            *now = msg;
            return 1;
        }
        *preMsg = msg;
        msg = msg->next;
   }
    return 0; 
}
#else
Msg_pkt_t *msgPktQueue_find_bySeq(MsgPkt_queue_t *queuePtr, uint16_t seq)
{
   Msg_pkt_t *msg = queuePtr->outMsgPtr;
   while(msg) {
        if(msg->seq == seq) {
            break;
        }
        msg = msg->next;
   }
    return msg; 
}
#endif

void msgPktQueue_remove_bySeq(MsgPkt_queue_t *queuePtr, uint16_t seq)
{
	Msg_pkt_t *msg = queuePtr->outMsgPtr, *pre = NULL, *next = NULL;
#if 1	
	while(msg) {
		if(msg->seq == seq) {
            next = msg->next;
            if (NULL == pre && NULL == next)
            {
                free(msg);
                queuePtr->outMsgPtr = NULL;
                queuePtr->last_outMsgPtr = NULL;
            }else if (NULL == pre){
                free(msg);
                queuePtr->outMsgPtr = next;
            }else if (NULL == next){
                free(msg);
                queuePtr->last_outMsgPtr = pre;
                pre->next = next;
            }else{
                free(msg);
                pre->next = next;
            }
			break;
		}
        pre = msg;
		msg = msg->next;
	}
#else
    while(msg){
        if (msg->seq == seq){
            if (msg->next){
                next = msg->next;
                memcpy(msg, next, sizeof(Msg_pkt_t));
                free(next);
            }else{
                free(msg);
                queuePtr->last_outMsgPtr = pre;
                pre->next = NULL;
            }
            break;
        }
        pre = msg;
		msg = msg->next;
    }
#endif
	queuePtr->outMsgNum--;
}

void msgPktQueue_remove(MsgPkt_queue_t *queuePtr,Msg_pkt_t **preMsgPtr, Msg_pkt_t **msgPtr)
{
    if(NULL == msgPtr || NULL == *msgPtr) {
        return;
    }
    Msg_pkt_t *next = (*msgPtr)->next; 
    if (next) {
        memcpy(*msgPtr, next, sizeof(Msg_pkt_t));
        free(next);
    }else{
       free(*msgPtr); 
       *msgPtr = NULL;
       queuePtr->last_outMsgPtr = *preMsgPtr;
    }
    queuePtr->outMsgNum--;
}

void msgPktQueue_remove_all(MsgPkt_queue_t *queuePtr)
{
    Msg_pkt_t *msg = NULL, *next = NULL;

    msg = queuePtr->outMsgPtr;
    while(msg)
    {
        next = msg->next;
        free(msg);
        msg = next;
        queuePtr->outMsgNum--;
    }
    queuePtr->outMsgPtr = msg;
    queuePtr->last_outMsgPtr = msg;
}

void msgPktQueue_trversal(MsgPkt_queue_t *queuePtr)
{
    Msg_pkt_t *msg = queuePtr->outMsgPtr;
    
    while(msg)
    {
        LOG("Sequence:%d \n", msg->seq);
        msg = msg->next;
    }
}


int main (void)
{
    MsgPkt_queue_t queue = {NULL,NULL,0};
    uint8_t data[64], idx;
    uint32_t nowTime;
    int retFunc;

    nowTime = time(NULL);
    for(idx = 1;idx < 100;idx++) {
        memset(data, idx, sizeof(data));
        if (retFunc = msgPktQueue_add(&queue, data, sizeof(data), idx, nowTime, NULL, 1)){
            LOG("Error:%d", retFunc);
            break;
        }
    }
    msgPktQueue_trversal(&queue);

    int input;
    Msg_pkt_t *msg, *pre;
    while(scanf("%d", &input) != EOF)
    {
        printf("#input :%d\n" ,input);
        if (-300 == input) {
            msgPktQueue_trversal(&queue);
            continue;
        }else if (-400 == input){
            msgPktQueue_remove_all(&queue);
            continue;
        }else if (-500 == input){
            LOG("Out Message Number:%d\n", queue.outMsgNum);
            continue;
        }else if (input < -599){
            nowTime = time(NULL);
            msgPktQueue_add(&queue,data, sizeof(data), idx++, nowTime, NULL, 1);
        } else if (input < 0){
            msgPktQueue_remove_bySeq(&queue, -input);
            continue;
        }
        msg = msgPktQueue_find_bySeq(&queue, input);
        if (msg){
            LOG("Sequence:%d, UTC:%d\n", msg->seq, msg->inTime);
        }else{
            LOG("No Found\n");
        }
    }
    return 0;
}

