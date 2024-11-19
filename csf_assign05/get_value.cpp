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

  std::string login = "LOGIN " + username + "\n";
  rio_writen(fd, login.c_str(), strlen(login.c_str()));
  std::string get = "GET " + table + " " + key + "\n";
  rio_writen(fd, get.c_str(), strlen(get.c_str()));
  std::string top = "TOP\n";
  rio_writen(fd, top.c_str(), strlen(top.c_str()));
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[1000];
  ssize_t n = rio_readnb(&rio, buf, sizeof(buf));
  //while (n > 0) {
    std::cout << buf;
    //n = rio_readlineb(&rio, buf, sizeof(buf));
  //}
  
  std::string bye = "BYE\n";
  rio_writen(fd, bye.c_str(), bye.length());
  // const char* message = (login + get + top + bye).c_str();
  // rio_writen(fd, message, strlen(message));
  close(fd);
  return 0;
}
