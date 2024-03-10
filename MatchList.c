#include "MatchList.h"
#include <stdlib.h>
#include <stddef.h>

void MatchListAdd(MatchList* list, Match match)
{
    MatchList newList =
    {
        realloc(list->matches, (list->count + 1) * sizeof(Match)),
        list->count + 1
    };

    if (newList.matches != NULL)
    {
        newList.matches[list->count] = match;
        *list = newList;
    }
}

void MatchListFree(MatchList* list)
{
    free(list->matches);
    *list = (MatchList){ NULL };
}
