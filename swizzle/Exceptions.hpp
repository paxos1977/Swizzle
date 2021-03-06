#pragma once

#include <boost/utility/string_view.hpp> 
#include <cstdint>
#include <stdexcept>
#include <string>

namespace swizzle { namespace lexer {
    class FileInfo;
    class TokenInfo;
    enum class TokenizerState : std::uint8_t;
    enum class TokenType : std::uint8_t;
}}

namespace swizzle { namespace parser {
    enum class ParserState : std::uint8_t;
}}

namespace swizzle {

    class InvalidStreamInput : public std::runtime_error
    {
    public:
        InvalidStreamInput(const std::string& sv);
    };

    class ParserError : public std::runtime_error
    {
    public:
        ParserError(const std::string& reason);
    };

    class StreamEmpty : std::runtime_error
    {
    public:
        StreamEmpty();
    };

    class StreamInputCausesOverflow : public std::runtime_error
    {
    public:
        StreamInputCausesOverflow(const std::string& s);
    };

    class StreamInputCausesUnderflow : public std::runtime_error
    {
    public:
        StreamInputCausesUnderflow(const std::string& s);
    };
    
    class TokenizerError : public std::runtime_error
    {
    public:
        TokenizerError(const std::string& reason);
    };
    
    class UnknownTokenizerState : public TokenizerError
    {
    public:
        UnknownTokenizerState(const lexer::TokenizerState state);
    };

    class TokenizerSyntaxError : public TokenizerError
    {
    public:
        TokenizerSyntaxError(const lexer::FileInfo& info, const std::string& reason);
    };

    class SyntaxError : public ParserError
    {
    public:
        SyntaxError(const std::string& error, const lexer::TokenInfo& found);
        SyntaxError(const std::string& error, const std::string& found, const lexer::FileInfo& info);
    };

    class UnknownParserState : public ParserError
    {
    public:
        UnknownParserState(const parser::ParserState state);
    };

}
