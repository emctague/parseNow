#include "lexer.h"
using namespace std;

TokenSet tokenize(const string &path, const LexSpec &spec)
{
    TokenSet tokens;
    std::ifstream in(path, std::ios::ate);
    assert(in.is_open());

    size_t codeLen = in.tellg();
    in.seekg(0, std::ios::beg);
    std::string code(codeLen, ' ');
    in.read(code.data(), (streamsize)codeLen);
    in.close();

    while (!code.empty()) {
        size_t longestMatch = 0;
        std::string longestMatchType;
        for (auto& type : spec) {
            cmatch m;
            if (regex_search(code.c_str(), m, type.second)) {
                if (m[0].length() > longestMatch) {
                    longestMatch = m[0].length();
                    longestMatchType = type.first;
                }
            }
        }

        if (longestMatch == 0) {
            throw std::runtime_error("No match at: " + code);
        }

        // All tokens except the discard (*) token type should be stored as the new longest match
        if (longestMatchType != "*")
            tokens.emplace_back(longestMatchType, code.substr(0, longestMatch));

        code = code.substr(longestMatch);
    }

    return tokens;
}
