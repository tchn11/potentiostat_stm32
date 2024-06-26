#include "ps_voltammetry.h"
#include "ps_time_utils.h"
#include "string"
using namespace std;

namespace ps
{

    const string Voltammetry::TestKey("test");

    Voltammetry::Voltammetry()
    {
        chronoampTest.setNumStep(2);
        chronoampTest.setName("chronoamp");

        availableTests_.push_back(&cyclicTest);
        availableTests_.push_back(&sinusoidTest);
        availableTests_.push_back(&constantTest);
        availableTests_.push_back(&squareWaveTest);
        availableTests_.push_back(&linearSweepTest);
        availableTests_.push_back(&chronoampTest);
        availableTests_.push_back(&multiStepTest);
    }
            

    BaseTest *Voltammetry::getTest(string name)
    {
        BaseTest *testPtr = nullptr;
        for (size_t i=0; i<availableTests_.size(); i++)
        {
            string currName = availableTests_[i] -> getName();
            //currName.trim();
            if (name.compare(currName))
            {
                testPtr = availableTests_[i];
                break;
            }
        }
        return testPtr;
    }

    
    ReturnStatus Voltammetry::getTest(JsonObject &jsonMsg, JsonObject &jsonDat, BaseTest* &testPtr)
    {
        ReturnStatus status;
        if (jsonMsg.containsKey(TestKey))
        {
            string testName = string((const char *)(jsonMsg[TestKey]));
            jsonDat.set(TestKey,jsonMsg[TestKey]);
            testPtr = getTest(testName);
            if (testPtr == nullptr)
            {
                status.success = false;
                status.message = string("test not found");
            }
        }
        else
        {
            status.success = false;
            status.message = string("json does not contain key: ") + TestKey;
        }
        return status;
    }


    ReturnStatus Voltammetry::getParam(JsonObject &jsonMsg, JsonObject &jsonDat)
    {
        ReturnStatus status;

        if (jsonMsg.containsKey(TestKey))
        {
            BaseTest *testPtr = nullptr;
            status = getTest(jsonMsg,jsonDat,testPtr);
            if (status.success && (testPtr != nullptr))
            {
                testPtr -> getParam(jsonDat);
            }
        }
        else
        {
            status.success = false;
            status.message = string("json does not contain key: ") + TestKey;
        }
        return status;
    }
    
    
    ReturnStatus Voltammetry::setParam(JsonObject &jsonMsg, JsonObject &jsonDat)
    {
        ReturnStatus status;

        if (jsonMsg.containsKey(TestKey))
        {
            BaseTest *testPtr = nullptr;
            status = getTest(jsonMsg,jsonDat,testPtr);
            if (status.success && (testPtr != nullptr))
            {
                status = testPtr -> setParam(jsonMsg,jsonDat);
            }
            else
            {
                status.success = false;
                status.message = string("test not found");
            }
        }
        else
        {
            status.success = false;
            status.message = TestKey + string(" key not found");
        }
        return status;
    } 
    
    
    ReturnStatus Voltammetry::getTestDoneTime(JsonObject &jsonMsg, JsonObject &jsonDat)
    {
        ReturnStatus status;

        if (!jsonMsg.containsKey(TestKey))
        {
            status.success = false;
            status.message = TestKey + string(" key not found");
            return status;
        }

        BaseTest *testPtr = nullptr;
        status = getTest(jsonMsg,jsonDat,testPtr);
        if ((!status.success) || (testPtr == nullptr))
        {
            status.success = false;
            status.message = string("test not found");
            return status;
        }

        string testName = testPtr -> getName();
        uint64_t doneTimeUs = testPtr -> getDoneTime();
        uint32_t doneTimeMs = convertUsToMs(doneTimeUs);

        jsonDat.set(TestKey, testName);
        jsonDat.set(TestDoneTimeKey, doneTimeMs);

        return status;
    }

    ReturnStatus Voltammetry::getTestNames(JsonObject &jsonMsg, JsonObject &jsonDat)
    {
        ReturnStatus status;
        JsonArray &jsonNameArray = jsonDat.createNestedArray(TestNameArrayKey);
        for (size_t i=0; i<availableTests_.size(); i++)
        {
            jsonNameArray.add(availableTests_[i] -> getName());
        }
        return status;
    }


    ReturnStatus Voltammetry::getMuxTestNames(JsonObject &jsonMsg, JsonObject &jsonDat)
    {
        ReturnStatus status;
        JsonArray &jsonNameArray = jsonDat.createNestedArray(TestNameArrayKey);
        for (size_t i=0; i<availableTests_.size(); i++)
        {
            if (availableTests_[i] -> isMuxCompatible())
            {
                jsonNameArray.add(availableTests_[i] -> getName());
            }
        }
        return status;
    }


    void Voltammetry::setSamplePeriod(uint64_t samplePeriod)
    {
        for (size_t i=0; i<availableTests_.size(); i++)
        {
            availableTests_[i] -> setSamplePeriod(samplePeriod);
        }
    }


} // namespace ps
