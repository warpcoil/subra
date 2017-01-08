#include "datamanager.h"
#include "oslib/error.h"

bool DataManager::getWord(my::String * statement, uint32_t offset, StatementPart * part) {

    uint32_t len = statement->length();

    uint32_t i=offset;

    my::String result;

    char r = statement->at(i);

    while ((i<len) && ((r == ' ') || (r == ',') || (r == '(') || (r == ')') || (r == '\r') || (r == '\n') || (r == '\t'))) {
        i++;
        r = statement->at(i);
    }

    r = statement->at(i);

    while ((i < len) && (r != ' ') && (r != ',') && (r != '(') && (r != ')') && (r != '\r') && (r != '\n') && (r != '\t')) {
        result += r;
        i++;
        r = statement->at(i);
    }

    //    while ((i<len) && ((r == ' ') || (r == ',') || (r == '(') || (r == ')') || (r == '\r') || (r == '\n') || (r == '\t'))) {
    //        i++;
    //        r = statement->at(i);
    //    }

    part->word = result;
    part->nextOffset = i;

    if (i >= len)
        return false; //EOF

    return true; //More Data

}

warp::Vector<DataManager::Column> * DataManager::buildColumns(my::String * statement, StatementPart * part) {

    warp::Vector<Column> * columns = new warp::Vector<Column>();

    while (getWord(statement, part->nextOffset, part)) {
        Column * column = new Column();

        column->name = part->word;
        column->flags = 0;

        //Get type[s]
        //and modifiers
        uint32_t prev = part->nextOffset;
        bool repeat = true;

        while ((repeat) && (getWord(statement, part->nextOffset, part))) {

            if (part->word.toUpper() == "BOOL") {
                column->type = BOOL;
                prev = part->nextOffset;
            } else if (part->word == "BYTE") {
                column->type = BYTE;
                prev = part->nextOffset;
            } else if (part->word == "CHAR") {
                column->type = BYTE;
                prev = part->nextOffset;
            } else if (part->word == "UBYTE") {
                column->type = UBYTE;
                prev = part->nextOffset;
            } else if (part->word == "SHORT") {
                column->type = SHORT;
                prev = part->nextOffset;
            } else if (part->word == "USHORT") {
                column->type = USHORT;
                prev = part->nextOffset;
            } else if (part->word == "INT") {
                column->type = INT;
                prev = part->nextOffset;
            } else if (part->word == "UINT") {
                column->type = UINT;
                prev = part->nextOffset;
            } else if (part->word == "ULONG") {
                column->type = ULONG;
                prev = part->nextOffset;
            } else if (part->word == "STRING") {
                column->type = STRING;
                prev = part->nextOffset;
            } else if (part->word == "TEXT") {
                column->type = STRING;
                prev = part->nextOffset;
            } else if (part->word == "DATETIME") {
                column->type = DATETIME;
                prev = part->nextOffset;
            } else if (part->word == "TIMESTAMP") {
                column->type = DATETIME;
                prev = part->nextOffset;
            } else if (part->word == "PRIMARY" /* KEY */) {

                StatementPart primaryKey;
                primaryKey.nextOffset = part->nextOffset;
                prev = part->nextOffset;

                getWord(statement, primaryKey.nextOffset, &primaryKey);

                if (primaryKey.word == "KEY") {
                    column->flags |= PRIMARYKEY;
                    part->nextOffset = primaryKey.nextOffset;
                    prev = part->nextOffset;
                } else {
                    message("Malformed Syntax: %s", primaryKey.word.c_str());
                    halt();
                }

            } else if (part->word == "NOT" /* NULL */) {

                StatementPart notNull;
                notNull.nextOffset = part->nextOffset;
                prev = part->nextOffset;

                getWord(statement, notNull.nextOffset, &notNull);

                if (notNull.word == "NULL") {
                    column->flags |= NOTNULL;
                    part->nextOffset = notNull.nextOffset;
                    prev = part->nextOffset;
                } else {
                    message("Malformed Syntax: %s", notNull.word.c_str());
                    halt();
                }

            } else if (part->word == "UNIQUE") {
                column->flags |= UNIQUE;
                prev = part->nextOffset;
            } else {

#if defined(DEBUG)
                message("Column: %s, Type: %i, Flags: %x\n", column->name.c_str(), column->type, column->flags);
#endif

                //Fall back => new column
                columns->push(column);
                part->nextOffset = prev;
                repeat = false;

            }
        }
    }

    return columns;

}

DataManager::Error DataManager::parseSyntax(my::String * statement) {

    statement->toUpper();

    StatementPart part;

    part.nextOffset = 0;

    while (getWord(statement, part.nextOffset, &part)) {

        if (part.word == "CREATE") {
            getWord(statement, part.nextOffset, &part);

            if (part.word == "TABLE") {

                getWord(statement, part.nextOffset, &part);

                Table * t = new Table();

                t->name = part.word;

                //Build up columns
                t->columns = buildColumns(statement, &part);

                message("Column Count: %i :", t->columns->size());

            } else {
                message("Unknown CREATE Statement: %s\n\n", statement->c_str());
                return INVALIDSYNTAX;
            }
        }
        else message("Unknown [%s]\n", part.word.c_str());


    }

    //getWord(statement, part.nextOffset, &part);
    //message("Unknown [%s]\n", part.word.c_str());

    return SUCCESS;
}

DataManager::Error DataManager::Test() {

    my::String testCreate("CREATE TABLE myTable(MyKey INT PRIMARY KEY, MyValue STRING)");

    return parseSyntax(&testCreate);

}

DataManager::Error DataManager::Exec(my::String &statement) {

    //Build ast (BASIC sql only)

}
