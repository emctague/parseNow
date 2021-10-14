#pragma once
#include <map>
#include <string>
#include <vector>
#include <regex>
#include <fstream>

using LexSpec = std::vector<std::pair<std::string, std::regex>>;
using GrammarVariants = std::map<std::string, std::vector<std::string>>;
using Grammar = std::map<std::string, GrammarVariants>;

LexSpec parseLexSpec(std::ifstream& in);
Grammar parseGrammar(std::ifstream& in);
