//
// async_client.cpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at <a href="http://www.boost.org/LICENSE_1_0.txt">http://www.boost.org/LICENSE_1_0.txt</a>)
//
#include "urldata.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>
#include "asio/ssl.hpp"

using asio::ip::tcp;

sslhand::sslhand(asio::io_context& io_context,
      const std::string& server, const std::string& path, const std::size_t maxsize, const std::string& cert)
    : resolver_(io_context),  ctx_(asio::ssl::context::sslv3),
      socket_(io_context, ctx_), 
      response_(maxsize)// init list
{
    io_ctx = &io_context;
    ctx_.load_verify_file(cert);
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
    tcp::resolver::query query(server, "http"); 

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
    io_ctx->stop();
}

bool sslhand::verify_certificate(bool preverified, asio::ssl::verify_context& ctx)
{
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    std::cout << "Verifying " << subject_name << "\n";

    return preverified;
}

void sslhand::handle_resolve(const asio::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    std::cerr << "handle_resolve\n";
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
                std::cerr << "handle_connect lambda\n";
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
  std::cerr << "in connect\n";
  if(!err){
  std::cerr << "about to handshake\n";
     socket_.async_handshake(asio::ssl::stream_base::client,
      [this](const asio::error_code& err)
      {
          sslhand::handle_handshake();
      });
  }
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
void sslhand::handle_handshake()
{   
    //std::cerr << "successful connection\n";
    // The connection was successful. Send the request.
  std::cerr << "in handshake\n";
           // std::cerr << &request_;
    asio::async_write(socket_, request_,
         [this](const asio::error_code& err, std::size_t bytes)
         {
           if(!err){
           // std::cerr << &request_;
             sslhand::handle_write_request();
            
            }
           else
             std::cerr << "Error: " << err.message() << "\n";
         });
}

void sslhand::handle_write_request()
{
    std::cerr << "handle_write_request\n";
    // Read the response status line.
    asio::async_read_until(socket_, response_, "\r\n",
         [this](const asio::error_code& err, std::size_t bytes)
         {
            if (!err)
            {
                sslhand::handle_read_status_line();
            }
            else
            {
                std::cerr << "Error: " << err.message() << "\n";
            }
         });
}

void sslhand::handle_read_status_line()
{
     std::cerr << "handle_read_status_line\n";
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

    if (status_code == 301 || status_code == 302){
       std::string redir_host;
       std::getline(response_stream, redir_host);
       std::cerr << redir_host;
       //io_serv->restart();
       //sslhand::handle_restart(
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
    while (std::getline(response_stream, header) && header != "\r")
      std::cerr << header << "\n";
    std::cerr << "\n";

    // Write whatever content we already have to output.
    if (response_.size() > 0)
      std::cout << &response_;

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
                std::cerr << "Error: " << err.message() << "\n";
            }

         });
}

void sslhand::connect() {
  try
  {
    io_ctx->run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

}
