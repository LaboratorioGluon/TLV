#ifndef __TLVMANAGER_H__
#define __TLVMANAGER_H__


#include <stdint.h>
#include "CircularBuffer.h"

#define MAX_TLV_DATA_SIZE 25

struct tlv_command{
    uint8_t tag;
    uint8_t len;
    uint8_t data[MAX_TLV_DATA_SIZE];
};


enum TLV_STATUS{
    TLV_FAIL = 0,
    TLV_OK = 1,
    TLV_QUEUE_FULL,
    TLV_QUEUE_EMPTY,
    TLV_DATA_BUFFER_FULL
};

class TLV{
public:
    TLV();

    void Init();

    TLV_STATUS addData(uint8_t byte);

    TLV_STATUS getLastTlv(tlv_command *pTlv);

    void Timeout();

private:

    tlv_command mCurrentTlv;
    uint32_t mWrittenData;

    uint8_t mExpecting; // Expecting: 0 = TAG, 1 = LEN, 2 = DATA

    CircularBuffer<tlv_command> mCbTlv;

    void resetCurrentTlv();

};


#endif //__TLVMANAGER_H__