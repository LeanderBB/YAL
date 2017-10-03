#pragma once
#include <memory>
namespace yal {

    class Log;
    class Lexer;
    class Parser
    {
    public:
        Parser(Lexer& lexer,
               Log& log);


        bool run();

        Log& getLog() {
            return m_log;
        }

        void logParseFailure();

    private:
        std::unique_ptr<void, void(*)(void*)> m_parserImpl;
        Lexer& m_lexer;
        Log& m_log;
        bool m_syntaxError;
    };

}
