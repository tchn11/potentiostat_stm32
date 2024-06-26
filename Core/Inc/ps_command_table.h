#ifndef PS_COMMAND_TABLE_H 
#define PS_COMMAND_TABLE_H 

#include "ps_return_status.h"
#include "ps_keyvalue_command.h"
#include "ps_constants.h"
#include "ArduinoJson.h"
#include "Array.h"
#include "string"
#include "stdint.h"
using namespace std;

namespace ps
{

    template<typename T, size_t MAX_SIZE>
    class CommandTable
    {
        
        public:

            CommandTable(T *client=nullptr);

            void clear();
            void clearTable();

            size_t size();
            size_t maxSize();

            void setClient(T *client);
            void registerMethod(string key, string value, ReturnStatus (T::*method)(JsonObject&,JsonObject&));

            ReturnStatus apply(string key, JsonObject &jsonMsg, JsonObject &jsonDat);

        protected:

            T *client_;
            Array<KeyValueCommand<T>,MAX_SIZE> table_;

    };


    template<typename T, size_t MAX_SIZE>
    CommandTable<T,MAX_SIZE>::CommandTable(T *client) : client_(client) {}


    template<typename T, size_t MAX_SIZE>
    void CommandTable<T,MAX_SIZE>::clearTable()
    {
        table_.clear();
    }


    template<typename T, size_t MAX_SIZE>
    void CommandTable<T,MAX_SIZE>::clear()
    {
        clearTable();
        client_ = nullptr;
    }


    template<typename T, size_t MAX_SIZE>
    size_t CommandTable<T,MAX_SIZE>::size()
    {
        return table_.size();
    }


    template<typename T, size_t MAX_SIZE>
    size_t CommandTable<T,MAX_SIZE>::maxSize()
    {
        return MAX_SIZE;
    }


    template<typename T, size_t MAX_SIZE>
    void CommandTable<T,MAX_SIZE>::setClient(T* client)
    {
        client_ = client;
    }


    template<typename T, size_t MAX_SIZE>
    void CommandTable<T,MAX_SIZE>::registerMethod(string key, string value, ReturnStatus (T::*method)(JsonObject&,JsonObject&))
    {
        KeyValueCommand<T> kvCommand(key,value,method);
        table_.push_back(kvCommand);
    }

    template<typename T, size_t MAX_SIZE>
    ReturnStatus CommandTable<T,MAX_SIZE>::apply(string key, JsonObject &jsonMsg, JsonObject &jsonDat)
    {
        ReturnStatus status;
        if (client_!= nullptr)
        {
            if ( jsonMsg.containsKey(key.c_str()) )
            {
                string cmd = string((const char *)(jsonMsg[key]));
                //cmd.trim();
                bool found = false;
                for (size_t i=0; i<table_.size(); i++)
                {
                    if (cmd.compare(table_[i].value()))
                    {
                        found = true;
                        jsonDat.set(CommandKey,jsonMsg[key]);
                        status = table_[i].applyMethod(client_,jsonMsg,jsonDat);
                        break;
                    }
                }
                if (!found)
                {
                    status.success = false;
                    status.message = string("command, ") + cmd + string(", not found");;
                }
            }
            else
            {
                status.success = false;
                status.message = "CommandTable: command not found";
            }
        }
        else
        {
            status.success = false;
            status.message = "CommandTable: client is nullptr";
        }
        return status;
    }


} // namespace ps

#endif
