#include <iostream>
#include <nlohmann/json.hpp>
#include "spec.h"
#include "lexer.h"
#include "parser.h"

using namespace std;
using json = nlohmann::json;

json toJSON(const shared_ptr<ASTNode>& node);

int main()
{
    std::ifstream in("../rules.txt");
    auto lexSpec = parseLexSpec(in);
    auto tokens = tokenize("../test.txt", lexSpec);
    auto grammar = parseGrammar(in);
    auto results = matchGrammar(grammar, "root", tokens);
    std::cout << toJSON(results).dump(4) << std::endl;
}

json toJSON(const shared_ptr<ASTNode>& node) {
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
