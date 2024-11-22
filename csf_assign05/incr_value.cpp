#include <iostream>
#include "csapp.h"
#include <algorithm>
#include <string>

//helper function to handle errors (returns true if there is an error)
bool error_handling(ssize_t n, char* buf){
  std::string output = buf;
  if(n < 0) {
    std::cerr << "Error: read failed\n";
    return 1;
  }
  if(output.find("ERROR") != std::string::npos) { //error handling for Error message
    std::string err_msg = output.substr(6,std::string::npos);
    //remove " " from quoted text
    err_msg.erase(std::remove(err_msg.begin(), err_msg.end(), '\"'), err_msg.end());
    std::cerr << "Error: " << err_msg;
    return 1;
  }
  else if(output.find("FAILED") != std::string::npos) { //error handling for Failed message
    std::string err_msg = output.substr(7,std::string::npos);
    //remove " " from quoted text
    err_msg.erase(std::remove(err_msg.begin(), err_msg.end(), '\"'), err_msg.end());
    std::cerr << "Error: " << err_msg;
    return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  if ( argc != 6 && (argc != 7 || std::string(argv[1]) != "-t") ) {
    std::cerr << "Usage: ./incr_value [-t] <hostname> <port> <username> <table> <key>\n";
    std::cerr << "Options:\n";
    std::cerr << "  -t      execute the increment as a transaction\n";
    return 1;
  }

  // initialize count and transaction vars
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

  const char* hostname_c = hostname.c_str();
  const char* port_c = port.c_str();
  //open connection
  int fd = open_clientfd(hostname_c, port_c);

  // initialize buffers for reading server response
  rio_t rio;
  rio_readinitb(&rio, fd);
  char buf[1000];
  ssize_t n;
  ssize_t f;

  //LOGIN message
  std::string login = "LOGIN " + username + "\n";
  f = rio_writen(fd, login.c_str(), strlen(login.c_str()));
  if (f < 0) { // check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (error_handling(n, buf)) { // handle all other errors
    return -1;
  }

  if(use_transaction) {
    //BEGIN message
    std::string begin = "BEGIN\n";
    f = rio_writen(fd, begin.data(), begin.length());
    if (f < 0) { // check if write succeeds
      std::cerr << "Error: write failed\n";
    }
    n = rio_readlineb(&rio, buf, sizeof(buf));
    if (error_handling(n, buf)) { // handle all other errors
      return -1;
    }
  }

  //GET message
  std::string get = "GET " + table + " " + key + "\n";
  f = rio_writen(fd, get.data(), get.length());
  if (f < 0) { // check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (error_handling(n, buf)) { // handle all other errors
    return -1;
  }
  //PUSH message (1 for increment)
  std::string push = "PUSH 1\n";
  f = rio_writen(fd, push.c_str(), strlen(push.c_str()));
  if (f < 0) { // check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (error_handling(n, buf)) { // handle all other errors
    return -1;
  }
  //ADD message
  std::string add = "ADD\n";
  f = rio_writen(fd, add.data(), add.length());
  if (f < 0) { // check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (error_handling(n, buf)) { // handle all other errors
    return -1;
  }
  //SET message
  std::string set = "SET " + table + " " + key + "\n";
  f = rio_writen(fd, set.c_str(), strlen(set.c_str()));
  if (f < 0) { // check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (error_handling(n, buf)) { // handle all other errors
    return -1;
  }

  if(use_transaction) {
    //COMMIT message
    std::string commit = "COMMIT\n";
    f = rio_writen(fd, commit.data(), commit.length());
    if (f < 0) { // check if write succeeds
      std::cerr << "Error: write failed\n";
      return -1;
    }
    n = rio_readlineb(&rio, buf, sizeof(buf));
    if (error_handling(n, buf)) { // handle all other errors
      return -1;
    }
  }
  //BYE message
  std::string bye = "BYE\n";
  f = rio_writen(fd, bye.data(), bye.length());
  if (f < 0) { // check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if (error_handling(n, buf)) { // handle all other errors
    return -1;
  }

  close(fd);
  return 0;  
}
