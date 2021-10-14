#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "lexer.h"

struct ParseNode {
    std::string name;
    std::string variant;
    std::string terminal; /**< If this is a terminal (consumed token), this stores its captured text. */
    size_t length; /**< The total number of tokens consumed to create this node (and all children). */
    std::map<std::string, std::vector<std::shared_ptr<ParseNode>>> children;

    /** Merge children from 'other' into this parse tree node, appending them to child lists. */
    void merge(const ParseNode& other);

    /** Add a child value with a given key, creating the child list if it doesn't exist. */
    void addChild(const std::string& key, std::shared_ptr<ParseNode>& value);
};

/** Produce a Parse Tree Node for a specific rule in a grammar set, consuming provided tokens. */
std::shared_ptr<ParseNode> matchGrammar(const Grammar& g, const std::string& rule, TokenSet& tokens);

/** Produce a Parse Tree Node for a specific *variant* of a rule in a grammar set, consuming provided tokens. */
std::shared_ptr<ParseNode> matchGrammarVariant(const Grammar& g, const std::string& rule, const std::string& variant,
                                               TokenSet& tokens);
