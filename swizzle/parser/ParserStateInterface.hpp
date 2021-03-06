#pragma once 
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserState.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace parser {

    class ParserStateInterface
    {
    public:
        virtual ~ParserStateInterface(){}
        virtual ParserState consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack& attributeStack, TokenStack& tokenStack, ParserStateContext& context) = 0;
    };
}}
