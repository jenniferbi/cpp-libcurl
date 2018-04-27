#include <stdio.h>
#include "handle.h"
#include "urldata.h"
#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>

int main(void)
{
    auto foo = [](const unsigned char * s, size_t size)
        {
            cout << s;
            return 0;
        };

    std::string url_name  ("http://www.stroustrup.com/Bjarne2018.jpg");
    std::size_t pos = url_name.find_last_of("/");
    std::string filename="./results/thread_pool_size_test";
    filename.append(url_name.substr(pos+1));
    filename.append(".csv");
    std::ofstream output_file {filename, std::ios_base::app};
    output_file << "wall time, user cpu time, percent wall" << "\n";
    boost::timer::auto_cpu_timer t(output_file, "%w\n");

    Multi myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, url_name,
                    CURLPP_OPT_THREAD_POOL, 1);
    //CURLPP_OPT_WRITEFN, foo);
    myhandle.perform();

    return 0;
}
