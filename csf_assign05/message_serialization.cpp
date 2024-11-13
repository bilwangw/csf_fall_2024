#include <utility>
#include <sstream>
#include <cassert>
#include <map>
#include "exceptions.h"
#include "message_serialization.h"
#include <iostream>

void MessageSerialization::encode( const Message &msg, std::string &encoded_msg )
{

  // TODO: implement
  MessageType message = msg.get_message_type();
  encoded_msg = "";

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
      break;

  }
  encoded_msg += "\n";
  //std::cout << encoded_msg;
  if (encoded_msg.length() > Message::MAX_ENCODED_LEN) {
    throw InvalidMessage("Invalid message: Message exceeds maximum limit");
  }
}

void MessageSerialization::decode( const std::string &encoded_msg, Message &msg )
{
  if (!msg.is_valid()) {
    throw std::invalid_argument("Invalid message: The message is invalid");
  }
  if (encoded_msg.length() > Message::MAX_ENCODED_LEN) {
    throw std::invalid_argument("Invalid message: Message exceeds maximum limit");
  }
  if (encoded_msg.back() != '\n') {
    throw std::invalid_argument("Invalid message: The message does not end in a newline character");
  }
  // TODO: implement
}
