#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <jsoncpp/json/json.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

#include "body.h"

class Json_parser
{
  private:
    static constexpr int secs_per_day{24*60*60};

    Json_parser() {};
    static body create_body_absolute(const Json::Value& properties);
    static body create_body_centralized(const Json::Value& properties);
  public:
    static std::vector<body> parse(std::string file_name, bool centralized);
};

#endif