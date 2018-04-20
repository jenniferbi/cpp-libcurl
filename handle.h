#include <algorithm>
#include <iostream>
#include <istream>
#include <ostream>
#include <vector>
#include <string>
#include <asio.hpp>
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
//        virtual void initialize();
        virtual void perform() { } ;
  //      virtual void setOpt(); // variadic options
   //     virtual void cleanup();
};


class Easy : public Curl {
    public:
     //   void initialize() override;
        void perform() override;
       // void setOpt() override; // variadic options
       // void cleanup() override;

};

//X(std::initializer_list<U>): v(il) { }

class Multi : public Curl {
    public:
       // void initialize() override;
        void perform() override;
       // void setOpt() override; // variadic options
        //void cleanup() override;
    private:
        size_t thread_pool_size = 2;
        vector<shared_ptr<httphand> > easy_transfers; 
};




#endif /* HANDLE_H */
