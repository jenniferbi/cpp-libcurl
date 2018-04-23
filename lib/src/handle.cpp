#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>
#include "urldata.h"
#include "handle.h"

/*
using asio::ip::tcp;
void Easy::perform(){
    asio::io_service io_service;
    httphand h(io_service, "images.metmuseum.org", "/CRDImages/as/original/DP141263.jpg");
    httphand h(io_service, "images.metmuseum.org", "/CRDImages/as/original/DP141051.jpg");
    httphand h(io_service, "images.metmuseum.org", "/CRDImages/as/original/DP123282.jpg");
    httphand h(io_service, "images.metmuseum.org", "/CRDImages/as/original/DP140984.jpg");
    httphand h(io_service, "images.metmuseum.org", "/CRDImages/ep/original/DT1946.jpg");
    h.connect();
//    Protocol * p = new HttpHandle(io_service, "www.boost.org", "/LICENSE_1_0.txt");
//    p->connect();
}
*/


// function where we loop through the easy handles
/*
void do_easy(asio::io_context& io_context, asio::yield_context yield){

    //auto self(shared_from_this());
    asio::io_service io_service;
    for (auto ez : easyhandles){
        shared_ptr<httphand> new_httphand(new httphand(io_service,
            "images.metmuseum.org", "/CRDImages/as/original/DP141263.jpg"));
        asio::io_context::strand strand_;
        asio::spawn(strand_,
             [this](asio::yield_context yield)
            {
                new_httphand->connect();
            });
    }

}*/

/*
void Multi::perform(){

   asio::io_context io_context;
   asio::spawn(io_context, do_easy);

}
*/

/*
void Multi::perform(){
    std::vector<std::shared_ptr<asio::thread> > threads;
    std::vector<std::shared_ptr<asio::io_service> > servs;
//    for (std::size_t i= 0; i < thread_pool_size; ++i){
    for (std::size_t i= 0; i < thread_pool_size; ++i){
        std::shared_ptr<asio::io_service> serv(new asio::io_service());
        servs.push_back(serv);
        std::shared_ptr<httphand> h(new httphand(*serv, "images.metmuseum.org", "/CRDImages/as/original/DP141263.jpg"));
        easy_transfers.push_back(h);
    }
    for (auto h : easy_transfers){
        std::shared_ptr<asio::thread> thread(new asio::thread([&h](){ h->connect(); }));
        threads.push_back(thread);
        std::cerr << "new thread who dis\n";
    }
    for (std::size_t i = 0; i < threads.size(); ++i){
        threads[i]->join();
        std::cerr << "new join\n";
    }
}
*/
