#include "Lexer.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_TEXT_LENGTH 10240

static char buffer[MAX_TEXT_LENGTH + 1];

static int comparator(const Match* lhs, const Match* rhs)
{
    return lhs->group - rhs->group;
}

static int ImportText(const char* fileName)
{
    FILE* file = fopen(fileName, "r");

    if (file == NULL)
        return -1;

    for (int i = 0; i < MAX_TEXT_LENGTH; ++i)
    {
        char character = fgetc(file);

        if (character == -1)
            break;

        buffer[i] = character;
    }

    fclose(file);
    return 0;
}

static void DumpMatches(const MatchList list)
{
    for (int i = 0; i < list.count; ++i)
    {
        Match match = list.matches[i];

        printf("%.*s\n"
            " %s (%d:%d)\n",
            match.length, buffer + match.start,
            groups[match.group], match.start, match.length);
    }
}

int main(void)
{
    if (ImportText("input.txt") != 0)
        return -1;

    MatchList list = GetMatches(buffer);
    DumpMatches(list);
    MatchListFree(&list);
}