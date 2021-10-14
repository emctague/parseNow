#pragma once
#include "spec.h"
#include <list>

using TokenSet = std::list<std::pair<std::string, std::string>>;

TokenSet tokenize(const std::string& path, const LexSpec& spec);
