#include "spec.h"
#include <sstream>
using namespace std;

LexSpec parseLexSpec(ifstream& in)
{
    LexSpec spec;
    string str;
    while (getline(in, str)) {
        if (str.empty() || str[0] == '#') continue;
        if (str == "END LEX") break;
        stringstream ss(str);

        string key, separator, value;
        ss >> key >> separator;
        if (separator != "<-") throw std::runtime_error("Expected <- separator in line: " + str);

        getline(ss, value);
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](char c) { return !std::isspace(c); }));

        spec.emplace_back(key, regex("^" + value, regex_constants::ECMAScript | regex_constants::icase));
    }

    return spec;
}

Grammar parseGrammar(ifstream& in)
{
    Grammar grammar;

    string str;

    while (getline(in, str)) {
        if (str.empty() || str[0] == '#') continue;
        stringstream ss(str);

        string rule, variant, separator;
        ss >> rule >> variant >> separator;
        if (separator != "<-") throw std::runtime_error("Expected <- separator in line: " + str);

        if (!grammar.contains(rule)) grammar[rule] = {};

        grammar[rule][variant] = {};
        auto& v = grammar[rule][variant];

        string token;
        while (ss >> token) {
            v.emplace_back(token);
        }
    }

    return grammar;
}
