#include <iostream>
#include "csapp.h"
#include "exceptions.h"
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

  const char* hostname_c = hostname.c_str();
  const char* port_c = port.c_str();
  // open connection
  int fd = open_clientfd(hostname_c, port_c);
  if (fd < 0) {
    std::cerr << "Error: could not connect to server\n";
    close(fd);
  }

  //initialize buffers for reading
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
    return 1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf)); 
  if(error_handling(n,buf)) {
    return -1;
  }
  //PUSH message
  std::string push = "PUSH " + value + "\n";
  f = rio_writen(fd, push.c_str(), strlen(push.c_str()));
  if (f < 0) { // check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
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
  if(error_handling(n,buf)) {
    return -1;
  }
  //BYE message
  std::string bye = "BYE\n";
  f = rio_writen(fd, bye.data(), bye.length());
  if (f < 0) { /// check if write succeeds
    std::cerr << "Error: write failed\n";
    return -1;
  }
  n = rio_readlineb(&rio, buf, sizeof(buf));
  if(error_handling(n,buf)) {
    return -1;
  }  
  close(fd);
  return 0;

}
