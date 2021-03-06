#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class EscapedCharInStringLiteralState : public TokenizerStateInterface
    {
    public:
        EscapedCharInStringLiteralState(CreateTokenCallback)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);
            if((c == '\'') ||
               (c == '0') ||
               (c == 'a') ||
               (c == 'r') ||
               (c == 'n'))
            {
                token.expand(1);
                return TokenizerState::StringLiteral;
            }

            throw TokenizerSyntaxError(fileInfo, "Expected a valid escape sequence (\\a \\' \\r \\n \\0) found '\\" + std::string(1, c) + "'");
        }
    };
}}}
