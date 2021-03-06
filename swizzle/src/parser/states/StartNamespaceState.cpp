#include <swizzle/parser/states/StartNamespaceState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartNamespaceState::consume(const lexer::TokenInfo& token, NodeStack&, NodeStack&, TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            tokenStack.push(token);
            return ParserState::NamespaceValue;
        }

        throw SyntaxError("Expecting namespace name", token);
    }
}}}
