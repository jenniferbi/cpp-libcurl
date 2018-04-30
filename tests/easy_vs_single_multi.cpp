#include <stdio.h>
#include "handle.h"
#include "urldata.h"

#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>

int main(void)
{
    std::string url_name  ("http://www.cs.columbia.edu/wp-content/uploads/2018/04/vhacks-image-scenes.jpg");
    std::size_t pos = url_name.find_last_of("/");
    std::string filename="./results/easy_vs_single_multi.csv";
    std::ofstream output_file {filename, std::ios_base::trunc};
    output_file << "wall time easy, wall time single thread multi" << "\n";
    boost::timer::auto_cpu_timer timer(output_file, 3, "%w");

    auto foo = [](const unsigned char * s, size_t size)
        {
            // cout << "*************************************\n";
            // cout << size << " received\n";
            // cout << "*************************************\n";
            // cout << s;
            return 0;
        };

    //CURLPP_OPT_WRITEFN, foo);
    Easy easy_handle;
    easy_handle.setOpt(CURLPP_OPT_URL, url_name,
                       CURLPP_OPT_WRITEFN, foo);

    Multi multi_handle;
    multi_handle.setOpt(CURLPP_OPT_URL, url_name,
                        CURLPP_OPT_THREAD_POOL, 1,
                        CURLPP_OPT_WRITEFN, foo);

    timer.stop();

    for (int i=0; i<20; i++) {
        timer.start();
        easy_handle.perform();
        timer.stop();
        timer.report();
        output_file << ",";

        timer.start();
        multi_handle.perform();
        timer.stop();
        timer.report();
        output_file << "\n";
    }

    return 0;
}
