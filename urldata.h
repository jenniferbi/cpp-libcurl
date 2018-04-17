#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef URLDATA_H
#define URLDATA_H
// abstract class 
class Protocol {
    public:
//        virtual void setup_connection(struct connectdata *);
        virtual int connect();
  //      virtual void disconnect(struct connectdata *);
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

using boost::asio::ip::tcp;
class HttpHandle : public Protocol {
  private:
    boost::asio::io_service* io_serv;
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
  
    // member fns
    void handle_resolve(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);
    void handle_write_request(const boost::system::error_code& err);
    void handle_read_status_line(const boost::system::error_code& err);
    void handle_read_headers(const boost::system::error_code& err);
    void handle_read_content(const boost::system::error_code& err);

  public:
    // constructor
    HttpHandle(boost::asio::io_service& io_service,
    const std::string& server, const std::string& path);
    // connect function
    int connect();
};

#endif /* URLDATA_H */
