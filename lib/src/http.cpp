/********************************************************
*               __   __  _____  __                     *
*              / /  / / / / _ \/ /                     *
*             / _ \/ /_/ / , _/ /__                    *
*            /_//_/\____/_/|_/____/                    *
*                                                      *
* This file contains the implementation of http client.*
*                                                      *
* It is based on example code from async_client.cpp    *
* https://www.boost.org/doc/libs/1_40_0/doc/           *
* html/boost_asio/example/http/client/async_client.cpp *
*                                                      *
********************************************************/



#include "urldata.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>
//#include <error_code.hpp>
using asio::ip::tcp;

//httphand constructor
httphand::httphand(asio::io_service& io_service,
      const std::string& server, const std::string& path,
      const std::size_t maxsize,
      std::function<int(const unsigned char *, std::size_t)>& fw,
      std::function<int(const unsigned char *, std::size_t)>& fr)
    : resolver_(io_service), socket_(io_service),
      signals_(io_service, SIGINT, SIGTERM),
      response_(maxsize), writeback(fw), readback(fr) // init list
{
    io_serv = &io_service;

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query(server, "http");
    resolver_.async_resolve(query,
        [this](const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
        {
            if(!err)
            {
                httphand::handle_resolve(err, endpoint_iterator);
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
        });

}

// typically not used. restart is preferable in non-error situations
// because we can reuse the handle
// but if we call stop, it is most likely an error situation
void httphand::handle_stop()
{
    io_serv->stop();
}

void httphand::handle_resolve(const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    //std::cerr << "handle_resolve\n";
      // Attempt a connection to the first endpoint in the list. Each endpoint
      // will be tried until we successfully establish a connection.
    asio::async_connect(socket_, endpoint_iterator,
         [this](const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
         {
            if(!err)
            {
                // std::cerr << "handle_connect lambda\n";
                httphand::handle_connect(err, ++endpoint_iterator);
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });

}

void httphand::handle_connect(const asio::error_code& err,
    tcp::resolver::iterator endpoint_iterator)
{
    // std::cerr << "in connect\n";
  if (!err)
  {
  //  std::cerr << "successful connection\n";
    // The connection was successful. Send the request.
    asio::async_write(socket_, request_,
         [this](const asio::error_code& err, std::size_t bytes)
         {
             httphand::handle_write_request();
         });
  }
  else if (endpoint_iterator != tcp::resolver::iterator())
  {
    // std::cerr << "try enxt endpoint\n";
   // The connection failed. Try the next endpoint in the list.
    socket_.close();
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
         [this, &endpoint_iterator](std::error_code err)
         {
            httphand::handle_connect(err, ++endpoint_iterator);
         });

  }
  else{
    std::cerr << "Error: " << err.message() << "\n";
    }
}

void httphand::handle_write_request()
{
    // std::cerr << "handle_write_request\n";
    // Read the response status line.
    asio::async_read_until(socket_, response_, "\r\n",
         [this](const asio::error_code& err, std::size_t bytes)
         {
            if (!err)
            {
                httphand::handle_read_status_line();
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });
}

void httphand::handle_read_status_line()
{
    // std::cerr << "handle_read_status_line\n";
    // Check that response is OK.
    std::istream response_stream(&response_);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cerr << "Invalid response\n";
      return;
    }
    if (status_code != 200)
    {
      // std::cerr << "Response returned with status code ";
      // std::cerr << status_code << "\n";
      return;
    }

    // Read the response headers, which are terminated by a blank line.
    asio::async_read_until(socket_, response_, "\r\n\r\n",
         [this](const asio::error_code& err, std::size_t bytes)
         {
            if (!err)
            {
                httphand::handle_read_headers();
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });
}

void httphand::handle_read_headers()
{
    // Process the response headers.
    std::istream response_stream(&response_);
    std::string header;
    while (std::getline(response_stream, header) && header != "\r") {
        // std::cerr << header << "\n";
    }
    // std::cerr << "\n";

    // Write whatever content we already have to output.
    if (response_.size() > 0){
      if (writeback){
        const unsigned char * p = asio::buffer_cast<const unsigned char *>(response_.data());
        writeback(p, response_.size());
      }
      else
        std::cout << &response_;
    }

    // Start reading remaining data until EOF.
    asio::async_read(socket_, response_,
        asio::transfer_at_least(1),
         [this](const asio::error_code& err, std::size_t bytes)
         {
            if (!err)
            {
                httphand::handle_read_content();
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });
}

void httphand::handle_read_content()
{
    // Write all of the data that has been read so far.
      if (writeback){
        const unsigned char * p = asio::buffer_cast<const unsigned char *>(response_.data());
        writeback(p, response_.size());
      }
      else
        std::cout << &response_;

    // Continue reading remaining data until EOF.
    asio::async_read(socket_, response_,
        asio::transfer_at_least(1),
         [this](const asio::error_code& err, std::size_t bytes)
         {
            if (!err)
            {
                httphand::handle_read_content();
            }
            else if (err.value() == 2)
            {
                return;
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }

         });
}

// "main" function called by Easy and Multi handles to perform the transfer.
void httphand::connect() {
  try
  {
    io_serv->run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

}
