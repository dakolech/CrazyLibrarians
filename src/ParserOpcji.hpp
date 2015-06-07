#ifndef PARSER_OPCJI
#define PARSER_OPCJI

#include <map>
#include <string>

using namespace std;

class ParserOpcji {
    ParserOpcji()                 = delete;
    ParserOpcji(ParserOpcji const&)    = delete;
    ParserOpcji operator=(ParserOpcji const&) = delete;

    public:
        static map<string, string> zczytajOpcjeUruchomienia(int argc, char *argv[]);
};

#endif
