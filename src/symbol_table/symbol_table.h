typedef struct symbolData
{
    char *name;
} SymbolData;

typedef struct symbolElement
{
    SymbolData* symbol;
    struct symbolElement *next;
} SymbolElement;


SymbolElement* createTable();
SymbolElement* put(SymbolElement* table, SymbolData* data);
SymbolData* get(SymbolElement* table, char* name);