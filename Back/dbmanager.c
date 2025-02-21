#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "dbmanager.h"

int GetQuotes(Qlist *Quotes, sqlite3 *db)
{
    (*Quotes).ElementsNumber = 0;
    (*Quotes).head = NULL;

    EQlist *P = NULL;
    EQlist *Q = NULL;

    sqlite3_stmt *stmt;
    const char *query = "SELECT * FROM Quotes;";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            (*Quotes).head = malloc(sizeof(EQlist));
            (*Quotes).head->quote.QuoteId = sqlite3_column_int(stmt, 0);
            strcpy((*Quotes).head->quote.Name, (const char*)sqlite3_column_text(stmt, 1));
            strcpy((*Quotes).head->quote.Country, (const char*)sqlite3_column_text(stmt, 2));
            strcpy((*Quotes).head->quote.QuoteBody, (const char*)sqlite3_column_text(stmt, 3));

            (*Quotes).head->next = NULL;
            P = (*Quotes).head;
            (*Quotes).ElementsNumber += 1;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Q = malloc(sizeof(EQlist));
            Q->quote.QuoteId = sqlite3_column_int(stmt, 0);
            strcpy(Q->quote.Name, (const char*)sqlite3_column_text(stmt, 1));
            strcpy(Q->quote.Country, (const char*)sqlite3_column_text(stmt, 2));
            strcpy(Q->quote.QuoteBody, (const char*)sqlite3_column_text(stmt, 3));

            P->next = Q;
            P = Q;
            (*Quotes).ElementsNumber += 1;
        }

        if(Q)
        {
            Q->next = NULL;
        }
        
    }

    sqlite3_finalize(stmt);

    return 0;
    
}

int AddQuote(sqlite3 *db, Quote quote)
{
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, 
        "INSERT INTO Quotes (Name, Country, QuoteBody) VALUES (?, ?, ?)", 
        -1, &stmt, NULL);
    
    sqlite3_bind_text(stmt, 1, quote.Name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, quote.Country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, quote.QuoteBody, -1, SQLITE_STATIC);
    
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return 0;
}