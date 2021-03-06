#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>

namespace swizzle { namespace parser { namespace states {

    class EnumFieldEqualReadState : public ParserStateInterface
    {
    public:
        ParserState consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack& attributeStack, TokenStack& tokenStack, ParserStateContext& context) override;
    };
}}}
