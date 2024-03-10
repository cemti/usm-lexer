#pragma once
#include "MatchList.h"
#include <stdbool.h>
#define COUNTOF(X) (sizeof((X)) / sizeof(*(X)))

typedef bool Consumer(const char* input, Match* match);
typedef const char* const Rules[];

bool ConsumeByPredicate(const char* input, Match* match, int predicate(int));
bool ConsumeByRules(const char* input, Match* match, Rules rules, int ruleCount, bool wordBoundary);

Match GetMatch(const char* input, int offset);
MatchList GetMatches(const char* input);

static Rules
    dataTypes =
    {
        "int",
        "float"
    },
    statements =
    {
        "if",
        "else"
    },
    operators =
    {
        "+",
        "-",
        "/",
        "*",
        "==",
        "=",
    },
    separators =
    {
        "(",
        ")",
        "{",
        "}",
        ",",
        ";"
    };

#define DECLARE_CONSUMERS(...)\
    Consumer __VA_ARGS__;\
    static Consumer* const consumers[] = { __VA_ARGS__ };

DECLARE_CONSUMERS(
    ConsumeWhitespace,
    ConsumeNumber,
    ConsumeStatement,
    ConsumeDataType,
    ConsumeIdentifier,
    ConsumeString,
    ConsumeComment,
    ConsumeOperator,
    ConsumeSeparator
)

#undef DECLARE_CONSUMERS