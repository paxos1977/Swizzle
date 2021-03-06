#include <swizzle/parser/states/BitfieldUnderlyingTypeState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldUnderlyingTypeState::consume(const lexer::TokenInfo& token, NodeStack&, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::l_brace)
        {
            return ParserState::BitfieldStartScope;
        }

        throw SyntaxError("Expected '{'", token);
    }

}}}
