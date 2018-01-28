#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>

namespace swizzle { namespace parser { namespace states {

    class BitfieldNameState : public ParserStateInterface
    {
    public:
        ParserState consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context) override;
    };
}}}
