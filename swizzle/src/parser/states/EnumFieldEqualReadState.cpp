#include <swizzle/parser/states/EnumFieldEqualReadState.hpp>

#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/types/SetValue.hpp>

#include <sstream>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumFieldEqualReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(!detail::nodeStackTopIs<ast::nodes::EnumField>(nodeStack))
        {
            throw ParserError("Internal parser error, top of stack is not ast::nodes::EnumField");
        }

        auto& enumField = static_cast<ast::nodes::EnumField&>(*nodeStack.top());
        nodeStack.pop(); // enumField is attached to Enum, so it shouldn't go out of scope here

        if(!detail::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
        {
            throw ParserError("Internal parser error, node below top of stack is not ast::nodes::Enum");
        }

        const auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
        const auto underlying = top.underlying();

        if((type == lexer::TokenType::hex_literal) ||
           (type == lexer::TokenType::numeric_literal))
        {
            enumField.value(types::setValue(underlying.token().value(), token.token().value()));
            context.CurrentEnumValue = enumField.value();

            return ParserState::EnumFieldValueRead;
        }

        if(type == lexer::TokenType::char_literal)
        {
            auto trimValue = token.token().value();
            trimValue.remove_prefix(1); // remove leading '
            trimValue.remove_suffix(1); // remove trailing '

            enumField.value(types::setValue(underlying.token().value(), trimValue));
            context.CurrentEnumValue = enumField.value();
            
            return ParserState::EnumFieldValueRead;
        }

        std::stringstream ss;
        ss << type;

        throw SyntaxError("Expected literal value, found " + ss.str() + " (" + token.token().to_string() + ")", token);
    }
}}}
