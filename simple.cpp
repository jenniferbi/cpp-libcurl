#include <stdio.h>
#include "handle.h"
#include "urldata.h"

int main(void)
{

    Easy myhandle;
    myhandle.setOpt(CURLPP_OPT_URL, "images.metmuseum.org",
      CURLPP_OPT_PATH, "/CRDImages/as/original/DP141263.jpg",
      CURLPP_OPT_TIMEOUT, 1000
    );

    myhandle.perform();
  return 0;
}
