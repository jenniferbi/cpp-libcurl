#include <stdio.h>
#include "handle.h"
#include "urldata.h"

#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>

int main(int argc, char** argv)
{
    std::string url_name  (argv[1]);
    std::size_t pos = url_name.find_last_of("/");
    std::string filename="./results/simple_";
    filename.append(url_name.substr(pos+1));
    filename.append(".csv");
    std::ofstream output_file {filename, std::ios_base::app};
    boost::timer::auto_cpu_timer t(output_file, "%w\n");

    Easy myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, url_name,
                    CURLPP_OPT_SSLCERT, "ca.pem"
        );

    myhandle.perform();
    return 0;
}
