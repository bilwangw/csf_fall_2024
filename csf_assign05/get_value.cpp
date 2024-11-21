#include <iostream>
#include "csapp.h"

bool error_handling(ssize_t n, char* buf){
  std::string output = buf;
  // if(n < 0) {
  //   std::cerr << "Error: read failed\n";
  //   return 1;
  // }
  if(output.find("ERROR") != std::string::npos) { //error handling
    std::cerr << "Error: " << output.substr(6,std::string::npos);
    return 1;
  }
  else if(output.find("FAILED") != std::string::npos) { //error handling
    std::cerr << "Error: " << output.substr(7,std::string::npos);
    return 1;
  }
  else if(n < 0) {
    std::cerr << "Error: read failed\n";
    return 1;
  }
  return 0;
}

int main(int argc, char **argv)
{
  if ( argc != 6 ) {
    std::cerr << "Usage: ./get_value <hostname> <port> <username> <table> <key>\n";
    return 1;
  }

  std::string hostname = argv[1];
  std::string port = argv[2];
  std::string username = argv[3];
  std::string table = argv[4];
  std::string key = argv[5];

  // TODO: implement
  const char* hostname_c = hostname.c_str();
  const char* port_c = port.c_str();
  int fd = open_clientfd(hostname_c, port_c);
  if (fd < 0) {
    std::cerr << "Error: could not connect to server\n";
  }
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[1000];
  ssize_t n;
  std::string output;

  std::string login = "LOGIN " + username + "\n";
  rio_writen(fd, login.data(), login.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }
  // output = buf;
  // if(n < 0) {
  //   std::cerr << "Error: login failed\n";
  //   return -1;
  // }
  // if(output.find("ERROR") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << output.substr(6,std::string::npos);
  //   return -1;
  // }
  // else if(output.find("FAILED") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << output.substr(7,std::string::npos);
  //   return -1;
  // }


  std::string get = "GET " + table + " " + key + "\n";
  rio_writen(fd, get.data(), get.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }
  // output = buf;
  // if(n < 0) {
  //   std::cerr << "Error: get failed\n";
  //   return -1;
  // }
  // else if(output.find("ERROR") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << output.substr(6,std::string::npos);
  //   return -1;
  // }
  // else if(output.find("FAILED") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << output.substr(7,std::string::npos);
  //   return -1;
  // }


  std::string top = "TOP\n";
  rio_writen(fd, top.data(), top.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));
  std::string data = buf;
  // if(n < 0) {
  //   std::cerr << "Error: top failed\n";
  //   return -1;
  // }
  // else if(data.find("ERROR") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << data.substr(6,std::string::npos);
  //   return -1;
  // }
  // else if(output.find("FAILED") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << data.substr(7,std::string::npos);
  //   return -1;
  // }
  if(error_handling(n,buf)) {
    return -1;
  }
  else if (n > 0) {
    std::cout << data.substr(5,std::string::npos); // get only the value from the line "DATA <value>"
  }
  
  std::string bye = "BYE\n";
  rio_writen(fd, bye.data(), bye.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }
  // output = buf;
  // if(n < 0) {
  //   std::cerr << "Error: bye failed\n";
  //   return -1;
  // }
  // else if(output.find("ERROR") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << output.substr(6,std::string::npos);
  //   return -1;
  // }
  // else if(output.find("FAILED") != std::string::npos) { //error handling
  //   std::cerr << "Error: " << output.substr(7,std::string::npos);
  //   return -1;
  // }

  close(fd);
  return 0;
}
