#pragma once

typedef enum Group
{
    Unknown,
    Whitespace,
    Number,
    Statement,
    DataType,
    Identifier,
    String,
    Comment,
    Operator,
    Separator
} Group;

static const char* const groups[] =
{
    "necunoscut",
    "spatiu liber",
    "numar",
    "instructiune",
    "tip de date",
    "identificator",
    "sir de caractere",
    "comentariu",
    "operator",
    "separator"
};

typedef struct Match
{
    int start, length;
    Group group;
} Match;

static inline int MatchOffset(const Match* match)
{
    return match->start + match->length;
}

typedef struct MatchList
{
    struct Match* matches;
    int count;
} MatchList;

void MatchListAdd(MatchList* list, Match match);
void MatchListFree(MatchList* list);