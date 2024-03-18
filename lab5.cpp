/* Copyright 2020 Samuel Fisher
* Lab5
* CSE278 - Section B
* Professor Vendome
* September 23, 2020
* This program allows creates a server with the port
* number specified by command line arguments. After
* connecting to the server, it will return the date
* and time to the client.
*/
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Function to return date
std::string make_daytime_string() {
  char date[26];
  std::time_t now = std::time(0);
  return ctime_r(&now, date);
}

int checkPort(std::string port) {
  int portNum;

  try {
    // Try to convert convert the input
    portNum = stoi(port);
//    std::cout << portNum;
//    if (portNum < 1025 || portNum > 65535) {
//
//    }
  }
  catch (std::exception& e) {
    // Provide some error message and return -1
    std::cerr << "Must supply port number\n";
    return -1;
  }

  return portNum;
}


int main(int argc, char *argv[]) {
    // Check user inputs for port number
    if (argv[1] == NULL) {
        std::cerr << "Must supply port number\n";
        return 0;
    }
    std::string port = argv[1];
    // Validate input is numeric
    int result = checkPort(port);
    // Check port number is valid
    if (!(result >= 0) || !(result <= 65535)) {
        return 0;
    }
    if (result < 1025 && result >= 0) {
        std::cout << "Cannot use privileged ports\n";
        return 0;
    }
    // Create the endpoint for the server
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), result));
    // Create the socket (and any other necessary objects)
    tcp::socket socket(io_service);
    // Wait and listen for a client to connect
    acceptor.accept(socket);
    // Prepare message to send back to client
    std::string final = make_daytime_string();
    boost::system::error_code ignored_error;
    // Write the message for current time to the socket
    write(socket, boost::asio::buffer(final), ignored_error);
    return 0;
}

