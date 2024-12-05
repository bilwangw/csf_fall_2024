#include <utility>
#include <sstream>
#include <cassert>
#include <map>
#include "exceptions.h"
#include "message_serialization.h"
#include <iostream>

void MessageSerialization::encode( const Message &msg, std::string &encoded_msg )
{
  MessageType message = msg.get_message_type();
  encoded_msg = "";
  // encode the message depending on what the message type (each will have different number of arguments)
  switch(message) {
    case MessageType::LOGIN:
      encoded_msg += "LOGIN " + msg.get_arg(0);
      break;
    case MessageType::CREATE:
      encoded_msg += "CREATE " + msg.get_arg(0);
      break;
    case MessageType::PUSH:
      encoded_msg += "PUSH ";
      encoded_msg += msg.get_arg(0);
      break;
    case MessageType::POP:
      encoded_msg += "POP";
      break;
    case MessageType::TOP:
      encoded_msg += "TOP";
      break;
    case MessageType::SET:
      encoded_msg += "SET " + msg.get_arg(0) + " " + msg.get_arg(1);
      break;
    case MessageType::GET:
      encoded_msg += "GET " + msg.get_arg(0) + " " + msg.get_arg(1);
      break;
    case MessageType::ADD:
      encoded_msg += "ADD";
      break;
    case MessageType::MUL:
      encoded_msg += "MUL";
      break;
    case MessageType::SUB:
      encoded_msg += "SUB";
      break;
    case MessageType::DIV:
      encoded_msg += "DIV";
      break;
    case MessageType::BEGIN:
      encoded_msg += "BEGIN";
      break;
    case MessageType::COMMIT:
      encoded_msg += "COMMIT";
      break;
    case MessageType::BYE:
      encoded_msg += "BYE";
      break;
    case MessageType::OK:
      encoded_msg += "OK";
      break;
    case MessageType::FAILED:
      encoded_msg += "FAILED " + '"' + msg.get_arg(0) + '"';
      break;
    case MessageType::ERROR:
      encoded_msg += "ERROR " + '"' + msg.get_arg(0) + '"';
      break;
    case MessageType::DATA:
      encoded_msg += "DATA " + msg.get_arg(0);
      break;
    default:
      //throw error
      throw InvalidMessage("Message type has no match");
      break;

  }
  // add a newline character at the end
  encoded_msg += "\n";
  // if the message exceeds buffer length, throw error
  if (encoded_msg.length() > Message::MAX_ENCODED_LEN) {
    throw InvalidMessage("Invalid message: Message exceeds maximum limit");
  }
}

void MessageSerialization::decode( const std::string &encoded_msg, Message &msg )
{
  //check for errors and throw corresponding exceptions
  if (!msg.is_valid()) {
    throw InvalidMessage("Invalid arguments (number and/or format)");
    return;
  }
  if (encoded_msg.length() > Message::MAX_ENCODED_LEN) {
    throw InvalidMessage("Invalid message: Message exceeds maximum limit");
    return;
  }
  if (encoded_msg.back() != '\n') {
    throw InvalidMessage("Invalid arguments (number and/or format)");
    return;
  }
  const Message new_msg;
  msg = new_msg;

  //read in each message and push to stack depending on message type
  std::string word;
  std::string message_type;
  std::istringstream iss(encoded_msg);
  iss >> message_type;
  iss >> word;
  if (message_type == "LOGIN") {
    msg.set_message_type(MessageType::LOGIN);
    msg.push_arg(word);
  }
  else if (message_type == "CREATE") {
    msg.set_message_type(MessageType::CREATE);
    msg.push_arg(word);
  }
  else if (message_type == "PUSH") {
    msg.set_message_type(MessageType::PUSH);
    msg.push_arg(word);
  }
  else if (message_type == "POP") {
    msg.set_message_type(MessageType::POP);
  }
  else if (message_type == "TOP") {
    msg.set_message_type(MessageType::TOP);
  }
  else if (message_type == "SET") {
    msg.set_message_type(MessageType::SET);
    msg.push_arg(word);
    iss >> word;
    msg.push_arg(word);
  }
  else if (message_type == "GET") {
    msg.set_message_type(MessageType::GET);
    msg.push_arg(word);
    iss >> word;
    msg.push_arg(word);
  }
  else if (message_type == "ADD") {
    msg.set_message_type(MessageType::ADD);
  }
  else if (message_type == "MUL") {
    msg.set_message_type(MessageType::MUL);
  }
  else if (message_type == "SUB") {
    msg.set_message_type(MessageType::SUB);
  }
  else if (message_type == "DIV") {
    msg.set_message_type(MessageType::DIV);
  }
  else if (message_type == "BEGIN") {
    msg.set_message_type(MessageType::BEGIN);
  }
  else if (message_type == "COMMIT") {
    msg.set_message_type(MessageType::COMMIT);
  }
  else if (message_type == "BYE") {
    msg.set_message_type(MessageType::BYE);
  }
  else if (message_type == "OK") {
    msg.set_message_type(MessageType::OK);
  }
  else if (message_type == "FAILED") {
    msg.set_message_type(MessageType::FAILED);
    // use substr to isolate message contents
    word = encoded_msg.substr(8,encoded_msg.length()-10);
    msg.push_arg(word);
  }
  else if (message_type == "ERROR") {
    msg.set_message_type(MessageType::ERROR);
    // use substr to isolate message contents
    word = encoded_msg.substr(8,encoded_msg.length()-10);
    msg.push_arg(word);
  }
  else if (message_type == "DATA") {
    msg.set_message_type(MessageType::DATA);
    msg.push_arg(word);
  }
  else {
    // if message type is none of the above, throw error
    std::string errMsg = "Unknown message type " + message_type;
    throw InvalidMessage(errMsg);
  }
}
