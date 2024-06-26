#ifndef PS_ANALOG_SUBSYSTEM_H
#define PS_ANALOG_SUBSYSTEM_H

#include "stm32f4xx.h"

#include "ps_hardware_defs.h"
#include "ps_pin_map.h"
#include "ps_gains.h"
#include "ps_volt_range.h"
#include "ps_curr_range.h"
#include "ps_return_status.h"

#include "string"
#include "stdint.h"
using namespace std;

#include "exteranl_analog.h"

namespace ps
{

    class AnalogSubsystem
    {
        public:

#if defined DEVBOARD_TEENSY
            static const uint16_t DefaultAnalogWriteResolution = 12;
            static const uint16_t DefaultAnalogReadResolution = 16;
            static const uint16_t DefaultAnalogReadAveraging = 16;
            static const uint8_t  DefaultAnalogReference = INTERNAL;
            static const uint16_t MaxValueAin = uint16_t((uint32_t(1) << DefaultAnalogReadResolution) -1);
            static const uint16_t MaxValueDac = uint16_t((uint32_t(1) << DefaultAnalogWriteResolution)-1);
            static const uint16_t MidValueDac = MaxValueDac/2;
#elif defined DEVBOARD_ITSY_BITSY
            static const uint16_t DefaultAnalogWriteResolution;
            static const uint16_t DefaultAnalogReadResolution;
            //static const eAnalogReference DefaultAnalogReference = AR_DEFAULT;
            // ----------------------------------------------------------------------------------------------
            // Temporary  - to check system prior to changing resistor values
            // ----------------------------------------------------------------------------------------------
            static const uint16_t MaxValueAin;
            static const uint16_t MaxValueDac;
            //static const uint16_t MaxValueAin = 1489;
            //static const uint16_t MaxValueDac = 1489;
            // ----------------------------------------------------------------------------------------------
            static const uint16_t MidValueDac;
#endif

            AnalogSubsystem();
            void initialize(); 

            void setVolt(float value); 
            float getVolt() const;     
            
            float getCurr() const;           
            float getRefElectVolt() const;   

            void setVoltRange(VoltRangeDac range);  
            VoltRangeDac getVoltRange() const;      
            bool autoVoltRange(float minVolt, float maxVolt);

            void setCurrRange(CurrRange range);  
            CurrRange getCurrRange() const;      

            ReturnStatus setVoltRangeByName(string voltRangeName);
            string getVoltRangeName() const;

            ReturnStatus setCurrRangeByName(string currRangeName);
            string getCurrRangeName() const;

#if defined HARDWARE_VERSION_0P2
            void setRefElectVoltRange(VoltRangeAdc range);
            VoltRangeAdc getRefElectVoltRange() const;

            ReturnStatus setRefElectVoltRangeByName(string voltRangeName);
            string getRefElectVoltRangeName() const;
            bool autoRefElectVoltRange(float minVolt, float maxVolt);
#endif

        protected:

            uint16_t valueDac_;
            VoltRangeDac voltRange_;
            CurrRange currRange_;

            spi_analog analog;

            void setVoltGain(VoltGain value);    
            VoltGain getVoltGain() const;        

            void setCurrGainPath(CurrGainPath value); 
            CurrGainPath getCurrGainPath() const;

            string getVoltGainString() const;
            string getCurrGainPathString() const;

            void setValueDac(uint16_t value); 
            uint16_t getValueDac() const;     

            uint16_t getTransAmpAin() const;  
            uint16_t getRefElectAin() const;  

            VoltRangeAdc refElectVoltRange_;
#if defined HARDWARE_VERSION_0P2
            void setRefElectVoltGain(VoltGain value);
            VoltGain getRefElectVoltGain() const;
#endif

    };

} // namespace ps


#endif
