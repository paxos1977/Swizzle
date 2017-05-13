#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class CharLiteral : public TokenizerStateInterface
    {
    public:
        CharLiteral(CreateTokenCallback)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& filePosition, Token& token) override
        {
            const char c = source.at(position);
            if(c == '\\')
            {
                return TokenizerState::EscapedCharInCharLiteral;
            }

            token.expand(source);
            filePosition.advanceBy(c);

            return TokenizerState::EndCharLiteral;
        }
    };
}}}