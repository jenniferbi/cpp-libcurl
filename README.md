# cpp-libcurl
hURL is a file transfer library supporting HTTP protocol, inspired by the cURL command line tool and libcurl library, implemented with Asio 1.12.1 (non-Boost).

## Navigating the repo
- Design document contains our design principles and broader motivation.
- The reference manual is a comprehensive listing of features and usage. It includes a tutorial.
- The source is located in /src and /include

## Installing the library
- Create a build directory: `mkdir build`
- do `cmake ..`, specifying any include paths for Asio, Boost, or OpenSSL (usually not necessary if your PATH environment variable is configured already.
- `make`

NOTE: we have not yet resolved dependency issues with OpenSSL, so it is highly likely that you will not be able to build the ssl examples, or the ssl handshake will fail.
