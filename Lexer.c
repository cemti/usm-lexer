#include "Lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static inline int isnotspace(int character) { return !isspace(character); }

bool ConsumeByPredicate(const char* input, Match* match, int predicate(int))
{
    while (predicate(input[MatchOffset(match)]))
        ++match->length;

    return match->length > 0;
}

bool ConsumeByRules(const char* input, Match* match, Rules rules, int ruleCount, bool wordBoundary)
{
    for (int i = 0; i < ruleCount; ++i)
    {
        int length = (int)strlen(rules[i]);

        if (strncmp(input + match->start, rules[i], length) == 0)
        {
            if (wordBoundary && isalnum(input[match->start + length]))
                continue;

            match->length = length;
            return true;
        }
    }

    return false;
}

bool ConsumeNumber(const char* input, Match* match)
{
    bool success = ConsumeByPredicate(input, match, isdigit);
    
    if (input[MatchOffset(match)] == '.')
    {
        ++match->length;
        success |= ConsumeByPredicate(input, match, isdigit);
    }

    return success;
}

bool ConsumeDataType(const char* input, Match* match)
{
    return ConsumeByRules(input, match, dataTypes, COUNTOF(dataTypes), true);
}

bool ConsumeStatement(const char* input, Match* match)
{
    return ConsumeByRules(input, match, statements, COUNTOF(statements), true);
}

bool ConsumeIdentifier(const char* input, Match* match)
{
    return ConsumeByPredicate(input, match, isalnum);
}

bool ConsumeString(const char* input, Match* match)
{
    if (input[match->start] == '"')
    {
        ++match->length;

        for (; ; )
        {
            int offset = MatchOffset(match);

            if (input[offset] == '\0')
                return false;

            if (input[offset] == '"')
            {
                ++match->length;
                break;
            }

            if (input[offset] == '\\' && input[offset + 1] != '\0')
                match->length += 2;
            else
                ++match->length;
        }

        return true;
    }

    return false;
}

bool ConsumeWhitespace(const char* input, Match* match)
{
    return ConsumeByPredicate(input, match, isspace);
}

bool ConsumeOperator(const char* input, Match* match)
{
    return ConsumeByRules(input, match, operators, COUNTOF(operators), false);
}

bool ConsumeSeparator(const char* input, Match* match)
{
    return ConsumeByRules(input, match, separators, COUNTOF(separators), false);
}

static bool ConsumeSingleLineComment(const char* input, Match* match)
{
    int offset = MatchOffset(match);

    if (input[offset] == '/' && input[offset + 1] == '/')
    {
        match->length += 2;

        do
        {
            offset = MatchOffset(match);

            if (input[offset] == '\0')
                break;

            ++match->length;
        } while (input[offset] != '\n');

        return true;
    }

    return false;
}

static bool ConsumeMultiLineComment(const char* input, Match* match)
{
    int offset = MatchOffset(match);

    if (input[offset] == '/' && input[offset + 1] == '*')
    {
        match->length += 2;

        for (; ; )
        {
            offset = MatchOffset(match);

            if (input[offset] == '\0')
                return false;

            if (input[offset] == '*' && input[offset + 1] == '/')
            {
                match->length += 2;
                break;
            }

            ++match->length;
        }

        return true;
    }

    return false;
}

bool ConsumeComment(const char* input, Match* match)
{
    return ConsumeSingleLineComment(input, match) || ConsumeMultiLineComment(input, match);
}

bool ConsumeUnknown(const char* input, Match* match)
{
    return ConsumeByPredicate(input, match, isnotspace);
}

Match GetMatch(const char* input, int offset)
{
    for (int i = 0; i < COUNTOF(consumers); ++i)
    {
        Match match = { offset };

        if (!consumers[i](input, &match))
            continue;

        match.group = (Group)(i + 1);
        return match;
    }

    Match match = { offset };
    ConsumeUnknown(input, &match);
    return match;
}

MatchList GetMatches(const char* input)
{
    MatchList list = { NULL };
    int offset = 0;
    
    while (input[offset] != '\0')
    {
        Match match = GetMatch(input, offset);
        offset += match.length;

        if (match.group == Whitespace)
            continue;

        MatchListAdd(&list, match);
    }

    return list;
}