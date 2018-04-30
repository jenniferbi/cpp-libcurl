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

## Running the tests
- Go to `build` directory.
- Run make to compile examples and tests. The executables will be placed in `build/bin`
- To run a test comparing the Easy handle with a single threaded Multi handle, run `./bin/easy_vs_single_multi`. The results of this test will be in a csv file in `build/results`.
- To run a whole bunch of executables comparing hURL and cURL, run `../tests/test.sh`.
This will download a bunch of files placing them in `build/hURLed` and write the timing results to specific csv files found in `build/results`.

NOTE: we have not yet resolved dependency issues with OpenSSL, so it is highly likely that you will not be able to build the ssl examples, or the ssl handshake will fail.
