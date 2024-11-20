#include <iostream>
#include "csapp.h"

int main(int argc, char **argv)
{
  if (argc != 7) {
    std::cerr << "Usage: ./set_value <hostname> <port> <username> <table> <key> <value>\n";
    return 1;
  }

  std::string hostname = argv[1];
  std::string port = argv[2];
  std::string username = argv[3];
  std::string table = argv[4];
  std::string key = argv[5];
  std::string value = argv[6];

  // TODO: implement
  const char* hostname_c = hostname.c_str();
  const char* port_c = port.c_str();
  int fd = open_clientfd(hostname_c, port_c);

  // output for debugging
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[1000];
  ssize_t n;

  std::string login = "LOGIN " + username + "\n";
  rio_writen(fd, login.c_str(), login.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (n > 0) {
    std::cout << buf << "login\n";
  }
  
  std::string push = "PUSH " + value + "\n";
  rio_writen(fd, push.c_str(), strlen(push.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (n > 0) {
    std::cout << buf << "push\n";
  }

  std::string set = "SET " + table + " " + key + "\n";
  rio_writen(fd, set.c_str(), strlen(set.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (n > 0) {
    std::cout << buf << "set\n";
  }

  std::string bye = "BYE\n";
  rio_writen(fd, bye.c_str(), strlen(bye.c_str()));

  // const char* message = (login + push + set + bye).c_str();
  // rio_writen(fd, message, strlen(message));
  close(fd);
  return 0;

}
