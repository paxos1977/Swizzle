#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>

namespace swizzle { namespace parser { namespace states {

    class ImportValueState : public ParserStateInterface
    {
    public:
        ParserState consume(const lexer::TokenInfo& token, NodeStack&, NodeStack& attributeStack, TokenStack& tokenStack, ParserStateContext&) override;
    };
}}}
