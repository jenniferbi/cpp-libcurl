#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>

#ifndef URLDATA_H
#define URLDATA_H
// abstract class
using asio::ip::tcp;
class protocol {
    public:
    // member fns, all pure virtual
    virtual void handle_connect(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator) = 0;
    virtual void handle_resolve(const asio::error_code& err,
        tcp::resolver::iterator endpoint_iterator) = 0;
    virtual void handle_write_request() = 0;//const asio::error_code& err);
    virtual void handle_read_status_line() = 0;//const asio::error_code& err);
    virtual void handle_read_headers() = 0;//const asio::error_code& err);
    virtual void handle_read_content() = 0;//const asio::error_code& err);
    virtual void handle_stop() = 0;//const asio::error_code& err);

        virtual void connect()=0;
};
//        virtual void setup_connection(struct connectdata *);
  //      virtual void disconnect(struct connectdata *);

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

#define CURLPP_OPT_URL    100
#define CURLPP_OPT_PATH   101
#define CURLPP_OPT_PORT   102
#define CURLPP_OPT_VERSION 103
#define CURLPP_OPT_MAXFILE 104
#define CURLPP_OPT_TIMEOUT 105
#define CURLPP_OPT_SSLCERT 106
struct UserDefined : std::enable_shared_from_this<UserDefined>{
    std::string url;
    std::string path;
    long timeout = -1;
    long use_port; /* TODO */
    long httpversion; /* TODO */
    std::size_t maxfile = -1; /* bytes*/
    std::string clientcert;
};

// helper functions
// must be inlined if used in callback in any handle, since those are in header file
inline void print_timeout(long ms)
{
    std::cerr << "Operation timed out after " << ms << "ms\n";
}


class httphand : public protocol/*, public std::enable_shared_from_this<httphand>*/ {
  private:
    asio::signal_set signals_;
    asio::io_service* io_serv;
    tcp::socket socket_;
    asio::streambuf request_;
    asio::streambuf response_;
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
    tcp::resolver resolver_;
    httphand(asio::io_service& io_service,
    const std::string& server, const std::string& path,
    const std::size_t maxsize);
    // connect functions
    void connect();
};

#endif /* URLDATA_H */
