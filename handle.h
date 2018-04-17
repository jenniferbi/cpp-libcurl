#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "urldata.h"

#ifndef HANDLE_H
#define HANDLE_H
using namespace std;
/*class Curl
{
    Curl(); // global init
    ~Curl(); // global cleanup
};*/
class Curl {
    public:
        virtual void initialize();
        virtual void perform();
        virtual void setOpt(); // variadic options
        virtual void cleanup();
};


class Easy : public Curl {
    public:
        //struct connectdata *easy_conn;
        
        void initialize() override;
        void perform() override;
        void setOpt() override; // variadic options
        void cleanup() override;

};

//X(std::initializer_list<U>): v(il) { }

class Multi : public Curl {
    public:
        void initialize() override;
        void perform() override;
        void setOpt() override; // variadic options
        void cleanup() override;
        Easy * easy_handle; 

};

void Easy::perform(){
    boost::asio::io_service io_service;
    HttpHandle h(io_service, "www.boost.org", "/LICENSE_1_0.txt");
    h.connect();
    //Protocol * p = new HttpHandler(io_service, "www.boost.org", "/LICENSE_1_0.txt");
    //p->connect();
}

#endif /* HANDLE_H */
