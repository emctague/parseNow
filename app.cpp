#include <iostream>
#include <nlohmann/json.hpp>
#include "spec.h"
#include "lexer.h"
#include "parser.h"

using namespace std;
using json = nlohmann::json;

json toJSON(const shared_ptr<ParseNode>& node);

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [rules-file] [source-file]" << std::endl;
        return 1;
    }

    std::ifstream in(argv[1]);
    if (!in.is_open()) {
        std::cerr << "Unable to open: " << argv[1] << std::endl;
        return 2;
    }

    try
    {
        auto lexSpec = parseLexSpec(in);
        auto grammar = parseGrammar(in);
        in.close();

        in.open(argv[2]);
        if (!in.is_open())
        {
            std::cerr << "Unable to open: " << argv[2] << std::endl;
            return 3;
        }
        auto tokens = tokenize(in, lexSpec);
        in.close();

        auto results = matchGrammar(grammar, "root", tokens);
        if (!results)
        {
            std::cerr << "Failed to match any grammar rules" << std::endl;
            return 4;
        }

        if (!tokens.empty())
        {
            std::cerr << "Didn't consume every token: " << tokens.size() << " remaining!" << std::endl;
            return 5;
        }
        std::cout << toJSON(results).dump(4) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

json toJSON(const shared_ptr<ParseNode>& node) {
    if (!node) { return nullptr; }
    if (!node->terminal.empty()) { return node->terminal; }

    json obj;
    obj["_type"] = node->name;
    obj["_variant"] = node->variant;

    for (auto& nc : node->children) {
        auto arr = json::array();

        for (auto& c : nc.second) {
            arr.push_back(toJSON(c));
        }

        obj[nc.first] = arr;
    }

    return obj;
}
