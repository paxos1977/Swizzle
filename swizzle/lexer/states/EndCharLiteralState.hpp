#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class EndCharLiteral
        : public TokenizerStateInterface
        , public TokenProducer<CreateTokenCallback>
    {
    public:
        EndCharLiteral(CreateTokenCallback createToken)
            : TokenProducer<CreateTokenCallback>(createToken)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& filePosition, Token& token) override
        {
            const char c = source.at(position);
            if(c == '\'')
            {
                this->produceToken(token, filePosition);
                token = ResetToken(source, position);

                return TokenizerState::Init;
            }

            throw TokenizerSyntaxError(filePosition, "Expected ' to terminate character literal, found '" + std::string(1, c) + "'");
        }
    };
}}}