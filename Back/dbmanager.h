#ifndef DATABASE
#define DATABASE

typedef struct Quote Quote;
struct Quote
{
    int QuoteId;
    char Name[100];
    char Country[100];
    char QuoteBody[1000];
};

typedef struct EQlist EQlist;
struct EQlist
{
    Quote quote;
    EQlist *next;
};

typedef struct Qlist Qlist;
struct Qlist
{
    EQlist *head;
    int ElementsNumber;
};

int GetQuotes(Qlist *Quotes, sqlite3 *db);
int AddQuote(sqlite3 *db, Quote quote);

#endif