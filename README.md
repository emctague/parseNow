# parseNow

This is a combined lexer and parser that reads grammar rules from
a definition file, uses those grammar rules to parse a second input
file, and outputs a JSON syntax tree for the parsed input.

## Usage

```
$ parseNow [rules-file] [input-file]
```

## Compilation

1. Install [nlohmann::json](https://github.com/nlohmann/json) where CMake can find it
   (recommend using vcpkg for this.)
2. `mkdir build`
3. `cd build`
4. `cmake ..`


## Grammar File Format

The grammar file consists of:

 - Comment lines - lines beginning with `#`.
 - Any number of Lexer Rule lines.
 - An `END LEX` line that separates lexer rules from parser rules.
 - Any number of Parser Rule lines.

### Example

The following is an example grammar file, which parses a sequence of
pairs in the form `( alphabetic-key numeric-value )`:

```
# The stuff down here is lexer definitions as regexes.
# The LONGEST match is used to construct each token.
KEYWORD [a-z][a-z0-9]+
# This is a comment line. They can appear anywhere in the file.
LBRACE \(
RBRACE \)
INTEGER [0-9]+
# The special '*' token type is used to discard values without error
* [ \t\r\n]

# This means the lexing section is done:
END LEX

# Grammar Rules

# When we want to see the value of a token in the output parse tree,
# we provide a name for that token to be stored in
# (e.g. "key" and "value" here).
keyvalue _ : LBRACE key:KEYWORD value:INTEGER RBRACE

# We can differentiate between multiple versions of a nonterminal
# based on their "variant" name, e.g. 'multi' or 'stop'. This is
# included in the output parse tree node's _variant property. If we don't care
# or only have one variant we might use `_`.
#
# The special '+' prefix on an element means we want to flatten
# the resulting parse tree node into this one, which is useful for building
# lists such as the 'sets' list we're making here!
sets multi : sets:keyvalue +sets
sets stop  :

# root is the rule applied to the whole input file.
# We use '+' again here to flatten so the resulting parse tree is more
# readable.
root _ : +sets
```

## Parse Tree Format

The resulting parse tree is a sequence of nested JSON objects in the form:

```json
{
  "_type": "<rule-name>",
  "_variant": "<variant-name>",
  "<any-saved-key-name>": [
    ...nested parse tree nodes, or strings representing the captured
    value of terminal/tokens. 
  ]
}
```

### Example

If we parsed the above example rule definition on this code:

```
(foo 132)
(bar 29392)(baz 923091)
```

We would receive this output:

```json
{
  "_type": "root",
  "_variant": "_",
  "sets": [
    {
      "_type": "keyvalue",
      "_variant": "_",
      "key": [
        "foo"
      ],
      "value": [
        "132"
      ]
    },
    {
      "_type": "keyvalue",
      "_variant": "_",
      "key": [
        "bar"
      ],
      "value": [
        "29392"
      ]
    },
    {
      "_type": "keyvalue",
      "_variant": "_",
      "key": [
        "baz"
      ],
      "value": [
        "923091"
      ]
    }
  ]
}
```