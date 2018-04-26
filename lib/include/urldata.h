/********************************************************
*               __   __  _____  __                     *
*              / /  / / / / _ \/ /                     *
*             / _ \/ /_/ / , _/ /__                    *
*            /_//_/\____/_/|_/____/                    *
*                                                      *
* This file contains the class definitions of various  *
* protocols, all inheriting from virtual class protocol*
*                                                      *
********************************************************/

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>
#include "asio/ssl.hpp"

#ifndef URLDATA_H
#define URLDATA_H

// abstract class defining protocol interface, used by handles
using asio::ip::tcp;
class protocol {
    public:
    // member fns, all pure virtual
    virtual void handle_connect(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator) = 0;
    virtual void handle_resolve(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator) = 0;
    virtual void handle_write_request(){};//const asio::error_code& err);
    virtual void handle_write_request(const asio::error_code& err){};
    virtual void handle_read_status_line() = 0;//const asio::error_code& err);
    virtual void handle_read_headers() = 0;//const asio::error_code& err);
    virtual void handle_read_content() = 0;//const asio::error_code& err);
    virtual void handle_stop() = 0;//const asio::error_code& err);

        virtual void connect()=0;
};

// options, struct to support options
#define CURLPP_OPT_URL 99
#define CURLPP_OPT_HOST 100
#define CURLPP_OPT_PATH   101
#define CURLPP_OPT_PORT   102
#define CURLPP_OPT_VERSION 103
#define CURLPP_OPT_MAXFILE 104
#define CURLPP_OPT_TIMEOUT 105
#define CURLPP_OPT_SSLCERT 106
#define CURLPP_OPT_WRITEFN 107
#define CURLPP_OPT_READFN 108

#define CURLPP_OPT_SCHEME 200
#define CURLPP_OPT_HTTP 201
#define CURLPP_OPT_HTTPS 202
#define CURLPP_OPT_NOT_SUPPORTED 250

struct UserDefined : std::enable_shared_from_this<UserDefined>{
    std::string host;
    std::string path;
    long timeout = -1;
    long use_port; /* TODO */
    long httpversion; /* TODO */
    std::size_t maxfile = -1; /* bytes*/
    std::string clientcert;
    int scheme = CURLPP_OPT_HTTP;
    std::function<int(const unsigned char *, std::size_t)> writeback;
    std::function<int(const unsigned char *, std::size_t)> readback;
};

// helper functions
// must be inlined if used in callback in any handle, since those are in header file
inline void print_timeout(long ms)
{
    std::cerr << "Operation timed out after " << ms << "ms\n";
}

// each protocol inherits from protocol class
class httphand : public protocol/*, public std::enable_shared_from_this<httphand>*/ {
  private:
    asio::signal_set signals_;
    asio::io_service* io_serv;
    tcp::socket socket_;
    asio::streambuf request_;
    asio::streambuf response_;
    tcp::resolver resolver_;
    std::function<int(const unsigned char *, std::size_t)>& writeback;
    std::function<int(const unsigned char *, std::size_t)>& readback;
    // member fns
    void handle_resolve(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_write_request();//const asio::error_code& err);
    void handle_read_status_line();//const asio::error_code& err);
    void handle_read_headers();//const asio::error_code& err);
    void handle_read_content();//const asio::error_code& err);
    void handle_stop();//const asio::error_code& err);

  public:
    // constructor
    httphand(asio::io_service& io_service,
    const std::string& server, const std::string& path,
    const std::size_t maxsize,
    std::function<int(const unsigned char *, std::size_t)>& fw,
    std::function<int(const unsigned char *, std::size_t)>& fr);

    // connect functions
    void connect();
};
// sslhand class definition
class sslhand : public protocol/*, public std::enable_shared_from_this<httphand>*/ {
  private:
    asio::io_service* io_serv;
    asio::ssl::context ctx_;
    asio::ssl::stream<asio::ip::tcp::socket> socket_;
    asio::streambuf request_;
    tcp::resolver resolver_;
    asio::streambuf response_;
    // member fns
    void handle_resolve(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_handshake(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_write_request(const asio::error_code& err);
    void handle_read_status_line();//const asio::error_code& err);
    void handle_read_headers();//const asio::error_code& err);
    void handle_read_content();//const asio::error_code& err);
    void handle_stop();//const asio::error_code& err);
    void handle_handshake(const asio::error_code& err);
    bool verify_certificate(bool preverified, asio::ssl::verify_context& ctx);
  public:
    // constructor
    sslhand(asio::io_service& io_service,
    const std::string& server, const std::string& path,
    const std::size_t maxsize, const std::string& cert);
    // connect functions
    void connect();
};

#endif /* URLDATA_H */
