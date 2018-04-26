#include <stdio.h>
#include "handle.h"
#include "urldata.h"

int main(void)
{

    Easy myhandle;
    // myhandle.setOpt(CURLPP_OPT_HOST, "images.metmuseum.org",
    //   CURLPP_OPT_PATH, "/CRDImages/as/original/DP141263.jpg",
    // myhandle.setOpt(CURLPP_OPT_HOST, "askubuntu.com",
    //                 CURLPP_OPT_PATH, "/questions/211038/cant-find-openssl",
     myhandle.setOpt(CURLPP_OPT_HOST, "upload.wikimedia.org",
                     CURLPP_OPT_PATH, "/wikipedia/commons/thumb/e/e0/SNice.svg/1200px-SNice.svg.png",
    //myhandle.setOpt(CURLPP_OPT_HOST, "images.cocodataset.org",
    //                                CURLPP_OPT_PATH, "/zips/val2014.zip",
                    CURLPP_OPT_SCHEME, CURLPP_OPT_HTTPS,
      CURLPP_OPT_SSLCERT, "ca.pem"
    );

    myhandle.perform();
    return 0;
}
