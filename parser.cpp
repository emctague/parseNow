#include "parser.h"
using namespace std;

shared_ptr<ParseNode> matchGrammar(const Grammar& g, const string &rule, TokenSet &tokens)
{
    if (!tokens.empty() && tokens.front().first == rule) {
        auto v = tokens.front();
        tokens.pop_front();
        return make_shared<ParseNode>(ParseNode {rule, "", v.second, 1, {} });
    }

    shared_ptr<ParseNode> longestVariant = nullptr;
    TokenSet longestTokens;

    /* This usually means we expect a token that isn't the next token, so this is a mismatch. */
    if (!g.contains(rule)) return nullptr;

    /* Try to match each variant and find the longest match, if any. */
    for (auto& variant : g.at(rule)) {
        auto ts = tokens;
        auto res = matchGrammarVariant(g, rule, variant.first, ts);
        if (!res) continue;
        if (!longestVariant || res->length > longestVariant->length) {
            longestVariant = res;
            longestTokens = ts;
        }
    }

    tokens = longestTokens;

    return longestVariant;
}

std::shared_ptr<ParseNode>
matchGrammarVariant(const Grammar &g, const string &rule, const string &variant, TokenSet &tokens)
{
    shared_ptr<ParseNode> res = make_shared<ParseNode>(ParseNode {rule, variant, "", 0, {} });
    for (const auto& subRule : g.at(rule).at(variant)) {
        string ruleName = subRule;
        string ruleKey;

        bool flatten = ruleName[0] == '+';
        if (flatten) ruleName = ruleName.substr(1);

        auto split = ruleName.find(':');
        if (split != string::npos) {
            ruleKey = ruleName.substr(0, split);
            ruleName = ruleName.substr(split + 1);
        }

        auto r = matchGrammar(g, ruleName, tokens);
        if (!r) return nullptr;

        res->length += r->length;

        if (flatten) res->merge(*r);
        else if (!ruleKey.empty()) res->addChild(ruleKey, r);
    }

    return res;
}

void ParseNode::merge(const ParseNode &other)
{
    for (auto& k : other.children) {
        if (!children.contains(k.first)) children[k.first] = {};
        children[k.first].insert(children[k.first].end(), k.second.begin(), k.second.end());
    }
}

void ParseNode::addChild(const string &key, shared_ptr<ParseNode> &value)
{
    if (!children.contains(key)) children[key] = {};
    children[key].push_back(value);
}
