#ifndef ZYRE_LISTENER_H
#define ZYRE_LISTENER_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>

#include "extern/zyre/node.hpp"

class ZyreListener
{
    public:
        ZyreListener(int timeout);
        ~ZyreListener();

        ///////////////////////////////////////////////////////////////////////
        // data queries
        std::vector<std::string> getQueryInterfaceList(std::string message, double timeout=-1);
        std::string getFeatures(std::string msg, double timeout=-1);
        std::string getData(std::string msg, double timeout=-1);
        ///////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////
        // status queries
        std::vector<std::string> getRopodIDs(std::string message, double timeout=-1);
        std::string getStatus(std::string msg, double timeout=-1);
        ///////////////////////////////////////////////////////////////////////

    private:
        ///////////////////////////////////////////////////////////////////////
        // data queries
        static void receiveQueryInterfaceList(zsock_t *pipe, void *args);
        static void receiveFeatures(zsock_t *pipe, void *args);
        static void receiveData(zsock_t *pipe, void *args);
        ///////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////
        // data queries
        static void receiveRopodIDs(zsock_t *pipe, void *args);
        static void receiveStatus(zsock_t *pipe, void *args);
        ///////////////////////////////////////////////////////////////////////

        Json::Value parseMessageJson(std::string msg);
        void shoutMessage(const Json::Value &json_msg);
        zmsg_t* stringToZmsg(std::string msg);

        zyre::node_t *listener_node_;
        std::map<std::string, zactor_t*> actors_;
        std::map<std::string, bool> reply_received_;

        Json::StreamWriterBuilder json_stream_builder_;

        std::map<std::string, std::vector<std::string>> query_interface_names_;
        std::map<std::string, std::vector<std::string>> ropod_ids_;
        std::map<std::string, std::string> received_status_;
        std::map<std::string, std::string> received_msg_;

        int timeout_;
};

struct ListenerParams
{
    ListenerParams(ZyreListener *listener, std::string sender_id)
    {
        this->listener = listener;
        this->sender_id = sender_id;
    }

    ZyreListener *listener;
    std::string sender_id;
};

#endif
