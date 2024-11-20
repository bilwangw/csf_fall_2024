#include <iostream>
#include "csapp.h"

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
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[1000];
  ssize_t n;

  std::string login = "LOGIN " + username + "\n";
  rio_writen(fd, login.data(), login.length());

  n = rio_readlineb(&rio, buf, sizeof(buf));
  // if (n > 0) {
  //   std::cout << buf;
  // }

  std::string get = "GET " + table + " " + key + "\n";
  rio_writen(fd, get.data(), get.length());

  n = rio_readlineb(&rio, buf, sizeof(buf));
  // if (n > 0) {
  //   std::cout << buf;
  // }

  std::string top = "TOP\n";
  const char* topc = "TOP\n";
  rio_writen(fd, top.data(), top.length());

  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (n > 0) {
    std::cout << buf;
  }
  
  std::string bye = "BYE\n";
  rio_writen(fd, bye.data(), bye.length());
  // const char* message = (login + get + top + bye).c_str();
  // rio_writen(fd, message, strlen(message));
  close(fd);
  return 0;
}
