/********************************************************
*               __   __  _____  __                     *
*              / /  / / / / _ \/ /                     *
*             / _ \/ /_/ / , _/ /__                    *
*            /_//_/\____/_/|_/____/                    *
*                                                      *
* This file contains the essential handle data         *
* structure, supporting data structures, and functions *
* that define the user interface                       *
*                                                      *
********************************************************/

#include <algorithm>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>
#include <vector>
#include <string>
#include <asio.hpp>
#include "urldata.h"

#ifndef HANDLE_H
#define HANDLE_H
using namespace std;

std::size_t maxfile = std::numeric_limits< std::size_t >::max();

// abstract class Curl
class Curl {
    public:
        virtual void perform() {};
        void _setopt(int a, string b);
        void _setopt(int a, long b);
        void _setopt(int a, std::function<int(const unsigned char *, std::size_t)> f);
        template<typename T, typename... Args>
        void setOpt(int a, T b, Args... args); // variadic options
        template<typename T>
        void setOpt(int a, T b); // only two args, used to parse variadic fn
        shared_ptr<UserDefined> defs;
        void parse_url(string url);
};

// Easy handle, shareable
class Easy : public Curl, std::enable_shared_from_this<Easy> {
    public:
        Easy(); // initialize UserDefined and other shared objects
        void perform() override;
    private:
         asio::steady_timer * timer;
};

// Multi handle, inheriting from Curl
class Multi : public Curl {
    public:
        Multi();
        ~Multi();
        void perform() override;
        size_t thread_pool_size = 10;
    private:
        vector<shared_ptr<httphand> > easy_transfers;
};

void Curl::parse_url(string url) {
    size_t pos = url.find("://");
    if (pos != string::npos) {
        string scheme (url.substr(0, pos));
        //cerr << scheme << "\n";
        if (scheme.compare("http") == 0) {
        }
        else if (scheme.compare("https") == 0) {
            defs->scheme = CURLPP_OPT_HTTPS;
        }
        else {
            defs->scheme = CURLPP_OPT_NOT_SUPPORTED;
            std::cerr << "Error: This scheme is not yet supported. Defaulting to HTTP" << "\n";
        }
        url = url.substr(pos+3);
    }
    pos = url.find('/');
    if (url[pos] == '/') {
        defs->host = url.substr(0, pos);
        defs->path = url.substr(pos);
    }
    else {
        std::cerr << "Error: Poorly formatted url" << "\n";
    }
}

void Curl::_setopt(int a, long b) {
    switch (a) {
    case CURLPP_OPT_PORT:
        defs->use_port = b;
        break;
    case CURLPP_OPT_TIMEOUT:
        defs->timeout = b;
        break;
    case CURLPP_OPT_MAXFILE:
        defs->maxfile = b;
        break;
    case CURLPP_OPT_SCHEME:
        defs->scheme = b;
        break;
    case CURLPP_OPT_THREAD_POOL:
        dynamic_cast<Multi*>(this)->thread_pool_size = b;
        break;
    default: std::cerr << "Error: CURLPP_OPT not yet supported" << "\n";
    }
}

void Curl::_setopt(int a, std::function<int(const unsigned char *, std::size_t)> f){
    switch (a) {
    case CURLPP_OPT_WRITEFN:
        defs->writeback = f;
        break;
    case CURLPP_OPT_READFN:
        defs->readback = f;
        break;
    default: std::cerr << "Error: CURLPP_OPT not yet supported" << "\n";
    }
}

void Curl::_setopt(int a, string b) {
    switch (a) {
    case CURLPP_OPT_URL:
        parse_url(static_cast<string>(b));
        break;
    case CURLPP_OPT_HOST:
        defs->host = static_cast<string>(b);
        break;
    case CURLPP_OPT_PATH:
        defs->path = static_cast<string>(b);
        break;
    case CURLPP_OPT_SSLCERT:
        defs->clientcert = static_cast<string>(b);
        break;
    default: std::cerr << "Error: CURLPP_OPT not yet supported" << "\n";
    }
}
// two main setopt functions. peel of the arguments in pairs, process separately!
template<typename T>
void Curl::setOpt(int a, T b) {
    Curl::_setopt(a, b);
}

template<typename T, typename... Args>
void Curl::setOpt(int a, T b, Args... args) {
    Curl::_setopt(a, b);
    Curl::setOpt(args...);
}

// Easy constructor and perform implementation
using asio::ip::tcp;
Easy::Easy(){
    defs = std::make_shared<UserDefined>();
}

void Easy::perform(){
    asio::io_service io_service;

    if (defs->host.empty()){
        cerr << "Empty url\n";
        return;
    }

	if(defs->path.empty())
	{
        defs->path = static_cast<string>("/");
	}
    // set timeout
    if (defs->timeout > 0){
        long to = defs->timeout;
        // std::cerr << "timeout is set to " << to << "\n";
        timer = new asio::steady_timer(io_service, asio::chrono::seconds(to));
        timer->async_wait([to, &io_service](const asio::error_code& e)
                    {   print_timeout(to);
                        io_service.stop();
                    });
        // cerr << "returned frmo async_wait\n";
    }
    if (defs->maxfile > 0)
        maxfile = defs->maxfile;
    if (defs->scheme == CURLPP_OPT_HTTPS && !defs->clientcert.empty()){
        //std::cerr << defs->clientcert << "\n";
        sslhand h(io_service, defs->host, defs->path, maxfile, defs->clientcert, defs->writeback, defs->readback);
        h.connect();
    }
    else if (defs->scheme == CURLPP_OPT_HTTP) {
        httphand h(io_service, defs->host, defs->path,
                maxfile, defs->writeback, defs->readback);

        h.connect();
    }
    io_service.restart();
    if (timer)
        delete timer;
}

// Multi constructor and perform implementation
Multi::Multi(){
    defs = std::make_shared<UserDefined>();
}
Multi::~Multi(){
    easy_transfers.clear();

}
void Multi::perform(){
    std::vector<asio::thread*> threads;
    std::vector<asio::io_service*> servs;
    for (std::size_t i= 0; i < thread_pool_size; ++i){
        asio::io_service *serv = new asio::io_service();
        servs.push_back(serv);
        std::shared_ptr<httphand> h(new httphand(*serv, defs->host, defs->path, maxfile, defs->writeback, defs->readback));
        easy_transfers.push_back(h);
    }
    for (auto h : easy_transfers){
        asio::thread * thread = new asio::thread([&h](){ h->connect(); });
        threads.push_back(thread);
        // std::cerr << "new thread who dis\n";
    }
    for (auto th : threads){
        th->join();
        // std::cerr << "new join\n";
    }
}


#endif /* HANDLE_H */
