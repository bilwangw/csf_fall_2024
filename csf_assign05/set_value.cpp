#include <iostream>
#include "csapp.h"

bool error_handling(ssize_t n, char* buf){
  std::string output = buf;
  if(n < 0) {
    std::cerr << "Error: read failed\n";
    return 1;
  }
  if(output.find("ERROR") != std::string::npos) { //error handling
    std::cerr << "Error: " << output.substr(6,std::string::npos);
    return 1;
  }
  else if(output.find("FAILED") != std::string::npos) { //error handling
    std::cerr << "Error: " << output.substr(7,std::string::npos);
    return 1;
  }
  return 0;
}

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
  if (fd < 0) {
    std::cerr << "Error: could not connect to server\n";
  }

  // output for debugging
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[1000];
  ssize_t n;
  std::string login = "LOGIN " + username + "\n";
  rio_writen(fd, login.c_str(), strlen(login.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }

  std::string push = "PUSH " + value + "\n";
  rio_writen(fd, push.c_str(), strlen(push.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }

  std::string set = "SET " + table + " " + key + "\n";
  rio_writen(fd, set.c_str(), strlen(set.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }

  std::string bye = "BYE\n";
  rio_writen(fd, bye.data(), bye.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }


  close(fd);
  return 0;

}
