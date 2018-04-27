/********************************************************
*               __   __  _____  __                     *
*              / /  / / / / _ \/ /                     *
*             / _ \/ /_/ / , _/ /__                    *
*            /_//_/\____/_/|_/____/                    *
*                                                      *
* This file contains the implementation of sslclient.  *
*                                                      *
* It is based on example code from async_client.cpp    *
* and ssl/client.cpp, please see                       *
* https://www.boost.org/doc/libs/1_50_0/doc/           *
* html/boost_asio/example/ssl/client.cpp               *
*                                                      *
********************************************************/

#include "urldata.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>
#include "asio/ssl.hpp"

using asio::ip::tcp;

// sslhand constructor
sslhand::sslhand(asio::io_service& io_service,
      const std::string& server, const std::string& path, 
      const std::size_t maxsize, const std::string& cert,
      std::function<int(const unsigned char *, std::size_t)>& fw,
      std::function<int(const unsigned char *, std::size_t)>& fr)
    : resolver_(io_service), ctx_(asio::ssl::context::sslv23),
      socket_(io_service, ctx_), response_(maxsize),
      writeback(fw), readback(fr)  // init list
{
    // setting context
    ctx_.set_default_verify_paths();
    io_serv = &io_service;
    //ctx_.use_certificate_file(cert, asio::ssl::context::file_format::pem);

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    // we must query it as "https" for secure connection.
    tcp::resolver::query query(server, "https");

    resolver_.async_resolve(query,
        [this](const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
        {
            if(!err)
            {
                sslhand::handle_resolve(err, endpoint_iterator);
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
        });

}
void sslhand::handle_stop()
{
    io_serv->stop();
}

bool sslhand::verify_certificate(bool preverified, asio::ssl::verify_context& ctx)
{
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    //std::cerr << "Verifying " << subject_name << "\n";

    return preverified;
}

void sslhand::handle_resolve(const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    // std::cerr << "handle_resolve\n";
    // begin verification
    socket_.set_verify_mode(asio::ssl::verify_peer);
    socket_.set_verify_callback(
        [this](bool preverified, asio::ssl::verify_context& ctx)
        {
            return sslhand::verify_certificate(preverified, ctx);
        }
        );

      // Attempt a connection to the first endpoint in the list. Each endpoint
      // will be tried until we successfully establish a connection.
    asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
         [this](const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
         {
            if(!err)
            {
                // std::cerr << "handle_connect lambda\n";
                sslhand::handle_connect(err, endpoint_iterator);
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });

}

void sslhand::handle_connect(const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    // std::cerr << "in connect\n";
  if(!err){
      // std::cerr << "about to handshake\n";
     socket_.async_handshake(asio::ssl::stream_base::client,
      [this](const asio::error_code& err)
      {
          sslhand::handle_handshake(err);
      });
  }
  // this is actually optional--do we want to keep trying other endpoints
  // if the first fails? Often first one works, don't waste time trying more 
/*  else if (endpoint_iterator != tcp::resolver::iterator())
  {
    // std::cerr << "try enxt endpoint\n";
   // The connection failed. Try the next endpoint in the list.
    asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
         [this](std::error_code err, tcp::resolver::iterator endpoint_iterator)
         {
            sslhand::handle_connect(err, ++endpoint_iterator);
         });
  }*/
  else{
    std::cerr << "Error: " << err.message() << "\n";
    }
}
void sslhand::handle_handshake(const asio::error_code& err)
{
    if(!err){
    // The connection was successful. Send the request.
    // std::cerr << "in handshake\n";
    const char* header=asio::buffer_cast<const char*>(request_.data());
    // std::cerr << header << "\n";
    asio::async_write(socket_, request_,
         [this](const asio::error_code& err, std::size_t bytes)
         {
               sslhand::handle_write_request(err);
         });
    }
    else
         std::cerr << "Handshake failed: " << err.message() << "\n";
}

void sslhand::handle_write_request(const asio::error_code& err)
{
    if (!err)
    {
    // std::cerr << "handle_write_request\n";
    // Read the response status line.
    asio::async_read_until(socket_, response_, "\r\n",
         [this](const asio::error_code& err, std::size_t bytes)
         {
                sslhand::handle_read_status_line();
         });
    }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
}

void sslhand::handle_read_status_line()
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
    else if (status_code != 200)
    {
      std::cerr << "Response returned with status code ";
      std::cerr << status_code << "\n";
      return;
    }

    // Read the response headers, which are terminated by a blank line.
    asio::async_read_until(socket_, response_, "\r\n\r\n",
         [this](const asio::error_code& err, std::size_t bytes)
         {
            if (!err)
            {
                sslhand::handle_read_headers();
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });
}

void sslhand::handle_read_headers()
{
    // Process the response headers.
    std::istream response_stream(&response_);
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");
    //std::cerr << header << "\n";
    //std::cerr << "\n";

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
                sslhand::handle_read_content();
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });
}

void sslhand::handle_read_content()
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
                sslhand::handle_read_content();
            }
            else
            {
                // suppressing errors here might not be best ~~~~
                // std::cerr << "Error: " << err.message() << "\n";
            }

         });
}
// "main" function called by Easy handle and Multi handle to perform a transfer
void sslhand::connect() {
  try
  {
    io_serv->run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

}
