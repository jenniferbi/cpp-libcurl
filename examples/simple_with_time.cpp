#include <stdio.h>
#include "handle.h"
#include "urldata.h"

#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>

int main(void)
{
    std::ofstream output_file {"./results/simple.txt", std::ios_base::app};
    // Not sure if we want to print out all these time values
    boost::timer::auto_cpu_timer t(output_file, "%w\n");

    Easy myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, "images.metmuseum.org",
                    CURLPP_OPT_PATH, "/CRDImages/as/original/DP141263.jpg",
                    CURLPP_OPT_TIMEOUT, 1000
        );

    myhandle.perform();
    return 0;
}
