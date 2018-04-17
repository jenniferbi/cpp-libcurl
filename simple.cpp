#include <stdio.h>
#include "handle.h"
#include "urldata.h"

int main(void)
{
/*    Easy ehandle;
    Multi mhandle;
    Curl *easy = new Easy();
    Curl *multi = new Multi();*/

    Easy myhandle;
    myhandle.perform();


  /*curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }*/
  return 0;
}
