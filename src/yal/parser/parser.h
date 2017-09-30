#pragma once
#include <memory>
namespace yal {

    class Lexer;
    class Parser
    {
    public:
        Parser(Lexer& lexer);


        bool run();

    private:
        std::unique_ptr<void, void(*)(void*)> m_parserImpl;
        Lexer& m_lexer;
    };

}
