#ifndef PS_MESSAGE_RECEIVER_H 
#define PS_MESSAGE_RECEIVER_H

#include "ps_constants.h"
#include "ps_circular_buffer.h"
#include "string"
#include "stdint.h"
using namespace std;

namespace ps
{

    class MessageReceiver
    {

        public:

            MessageReceiver();
            void reset();
            void readData();
            string next();
            bool available() const;
            uint32_t getMessageCnt() const;
            uint32_t getTotalMessageCnt() const;

        protected:

            CircularBuffer<char,SerialBufferSize> serialBuffer_;
            bool overflow_ = false;
            uint32_t messageCnt_ = 0;
            uint32_t totalMessageCnt_ = 0;


    };

} // namespace ps

#endif
