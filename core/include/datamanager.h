#ifndef DATAMANAGER
#define DATAMANAGER

#include "stdint.h"
#include "my/my_string.h"
#include "warp/vector.h"

class DataManager {
public:
    enum Error {
        SUCCESS = 0,
        INVALIDSYNTAX = 1
    };

    Error Test();
    Error Exec(my::String &statement);

private:

    enum Type {
        BOOL, //8 bit unsigned
        BYTE, //8 signed bits
        UBYTE, //8 unsigned bits
        SHORT, //16 signed bits
        USHORT, //16 unsigned bits
        INT, //32 signed bits
        UINT, //32 unsigned bits
        LONG, //64 bit signed
        ULONG, //64 bit unsigned
        STRING, //A string
        DATETIME //A custom representation of date time, of 64 bit length
        /* alias TEXT=>STRING */
        /* TIMESTAMP not supported due to limited range 2038 problem use DATETIME */
        /* alias CHAR=>BYTE */
    };

    enum Flags {
        PRIMARYKEY = 1,
        NOTNULL = 2,
        UNIQUE = 4,
        HASFORIEGNKEY = 8
    };

    typedef struct Column {
        Type type;
        my::String name;
        uint8_t flags;
    } Column;

    typedef struct Row {
        uint32_t rowId;
        warp::Vector<void *> variables;
    } Row;

    typedef struct Table {
        my::String name;
        warp::Vector<Column> * columns;
        warp::Vector<Row> * rows;
    } Table;

    warp::Vector<Table> root;

    typedef struct StatementPart {
        uint32_t nextOffset;
        my::String word;
    } StatementPart;

    bool getWord(my::String *statement, uint32_t offset, StatementPart * part);
    warp::Vector<Column> * buildColumns(my::String * statement, StatementPart * part);
    Error parseSyntax(my::String * statement);

};

#endif // DATAMANAGER

