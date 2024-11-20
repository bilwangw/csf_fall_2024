#include <iostream>
#include "csapp.h"

int main(int argc, char **argv) {
  if ( argc != 6 && (argc != 7 || std::string(argv[1]) != "-t") ) {
    std::cerr << "Usage: ./incr_value [-t] <hostname> <port> <username> <table> <key>\n";
    std::cerr << "Options:\n";
    std::cerr << "  -t      execute the increment as a transaction\n";
    return 1;
  }

  int count = 1;

  bool use_transaction = false;
  if ( argc == 7 ) {
    use_transaction = true;
    count = 2;
  }

  std::string hostname = argv[count++];
  std::string port = argv[count++];
  std::string username = argv[count++];
  std::string table = argv[count++];
  std::string key = argv[count++];

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
  rio_writen(fd, login.c_str(), strlen(login.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));

  if(use_transaction) {
    std::string get = "BEGIN\n";
    rio_writen(fd, get.data(), get.length());
    n = rio_readlineb(&rio, buf, sizeof(buf));
  }

  
  std::string get = "GET " + table + " " + key + "\n";
  rio_writen(fd, get.data(), get.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));

  std::string push = "PUSH 1\n";
  rio_writen(fd, push.c_str(), strlen(push.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));

  std::string add = "ADD\n";
  rio_writen(fd, add.data(), add.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));


  std::string set = "SET " + table + " " + key + "\n";
  rio_writen(fd, set.c_str(), strlen(set.c_str()));
  n = rio_readlineb(&rio, buf, sizeof(buf));

  if(use_transaction) {
    std::string get = "COMMIT\n";
    rio_writen(fd, get.data(), get.length());
    n = rio_readlineb(&rio, buf, sizeof(buf));
  }

  std::string bye = "BYE\n";
  rio_writen(fd, bye.data(), bye.length());
  n = rio_readlineb(&rio, buf, sizeof(buf));


  close(fd);
  return 0;  
}
