#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>
#include "urldata.h"
#include "handle.h"


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

