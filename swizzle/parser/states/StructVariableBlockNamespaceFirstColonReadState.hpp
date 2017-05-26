#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>

namespace swizzle { namespace parser { namespace states {

    class StructVariableBlockNamespaceFirstColonReadState : public ParserStateInterface
    {
    public:
        ParserState consume(const lexer::TokenInfo& token, ast::AbstractSyntaxTree& ast, NodeStack& nodeStack, TokenStack& tokenStack) override;
    };
}}}
