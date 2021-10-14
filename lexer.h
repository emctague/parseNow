#pragma once
#include "spec.h"
#include <list>

using TokenSet = std::list<std::pair<std::string, std::string>>;

TokenSet tokenize(std::ifstream& in, const LexSpec& spec);
