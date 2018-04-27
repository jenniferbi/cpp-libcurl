#include <stdio.h>
#include "handle.h"
#include "urldata.h"

#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>

int main(void)
{
    //std::string url_name  ("http://images.metmuseum.org/CRDImages/as/original/DP141263.jpg");
    //std::string url_name="https://images.cocodataset.org/zips/val2014.zip";
    //std::string url_name  ("https://www.iana.org/domains/reserved");
    std::string url_name  ("http://www.columbia.edu/cu/bulletin/uwb/");
    std::size_t pos = url_name.find_last_of("/");
    std::string filename="./results/simple_";
    filename.append(url_name.substr(pos+1));
    filename.append(".txt");
    std::ofstream output_file {filename, std::ios_base::app};
    boost::timer::auto_cpu_timer t(output_file, "%w\n");

    Easy myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, url_name,
                    CURLPP_OPT_SSLCERT, "ca.pem"
        );

    myhandle.perform();
    return 0;
}
