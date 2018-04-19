#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>

#ifndef URLDATA_H
#define URLDATA_H
// abstract class 
class protocol {
    public:
        virtual void setup_connection(struct connectdata *);
        virtual int connect();
        virtual void disconnect(struct connectdata *);
};

// The connectdata struct contains all fields and variables that should be
// unique for an entire connection
struct connectdata {
  //  bool inuse;
   // long connection_id;
    // dns_entry is the particular host we use. points to an entry in
    // DNS cache. Only modified in done(). The entry will be NULL if the 
    // connection is re-used, as then there is no name resolve done.
   // struct dns_entry * dns_entry;
    std::string server;
    std::string path;

};

using asio::ip::tcp;
class httphand /*: public Protocol*/ {
  private:
    asio::io_service* io_serv;
    tcp::resolver resolver_;
    tcp::socket socket_;
    asio::streambuf request_;
    asio::streambuf response_;
    unsigned long long max_timeout; 
    // member fns
    void handle_resolve(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_write_request();//const asio::error_code& err);
    void handle_read_status_line();//const asio::error_code& err);
    void handle_read_headers();//const asio::error_code& err);
    void handle_read_content();//const asio::error_code& err);

  public:
    // constructor
    httphand(asio::io_service& io_service,
    const std::string& server, const std::string& path);
    // connect function
    int connect();
};

#endif /* URLDATA_H */
