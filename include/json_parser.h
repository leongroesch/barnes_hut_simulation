#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <jsoncpp/json/json.h>
#include <SFML/Graphics.hpp>
#include <vector>

#include "body.h"

class Json_parser
{
  private:
    Json_parser() {};
    static body create_body(const Json::Value& properties);
  public:
    static std::vector<body> parse(std::string file_name);
};

#endif