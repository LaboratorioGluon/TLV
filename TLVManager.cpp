
#include "TLVManager.h"

#include <string.h>

TLV::TLV():
    mCbTlv(6){

    mExpecting = 0;
    resetCurrentTlv();
    mWrittenData = 0;

}

void TLV::Init(){

    mCbTlv.Init();

}

TLV_STATUS TLV::getLastTlv(tlv_command *pTlv){
    if( !mCbTlv.isEmpty() ){

        mCbTlv.Read(pTlv);

        return TLV_OK;

    }else{

         return TLV_QUEUE_EMPTY;       

    }
    
}

TLV_STATUS TLV::addData(uint8_t byte){


    if( mExpecting == 0){ // TAG

        mCurrentTlv.tag = byte;
        mExpecting = 1;

        return TLV_OK;

    }else if(mExpecting == 1){ // LEN

        mCurrentTlv.len = byte;
        mExpecting = 2;
        return TLV_OK;

    }else if(mExpecting == 2){ // DATA

        if( mWrittenData == MAX_TLV_DATA_SIZE ){

          return TLV_DATA_BUFFER_FULL;

        }else{

            mCurrentTlv.data[mWrittenData] = byte;
            mWrittenData++;

            if(mWrittenData == mCurrentTlv.len){
                mCbTlv.Write(mCurrentTlv);
                resetCurrentTlv();
                mWrittenData = 0;
                mExpecting = 0;
            }
        
        }

    }

    return TLV_OK;

}


void TLV::Timeout(){
    mExpecting = 0;
    resetCurrentTlv();
    mWrittenData = 0;
}

void TLV::resetCurrentTlv(){
    memset(&mCurrentTlv,0, sizeof(mCurrentTlv));
}