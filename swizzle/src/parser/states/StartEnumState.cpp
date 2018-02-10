#include <swizzle/parser/states/StartEnumState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AttachAttributes.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartEnumState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(tokenStack.empty())
            {
                throw ParserError("Token Stack unexpectedly empty.");
            }

            const auto& info = tokenStack.top();
            const auto node = types::utils::appendNode<ast::nodes::Enum>(nodeStack, info, token, context.CurrentNamespace);

            types::utils::attachAttributes(attributeStack, node);

            const auto& en = static_cast<ast::nodes::Enum&>(*node);
            context.SymbolTable.insert(en.name(), types::SymbolInfo(en.name(), types::SymbolType::Enum, node));

            nodeStack.push(node);
            tokenStack.pop();
            
            return ParserState::EnumName;
        }

        throw SyntaxError("Expected enum name", token);
    }
}}}
