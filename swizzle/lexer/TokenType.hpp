#pragma once 
#include <cstdint>
#include <ostream>

namespace swizzle { namespace lexer {

    enum class TokenType : uint8_t {

        string,                 // variable or type name

        string_literal,         // "string literal"
        char_literal,           // 'c'
        numeric_literal,        // 42
        float_literal,          // 42.4
        hex_literal,            // 0x0a

        attribute,              // @attribute
        attribute_block,        // @attribute{block}

        keyword,                // import | namespace | using | struct | enum | const |
        type,                   // variable_block | u8 | i8 | u16 | i16 | u32 | i32 | u64 | i64 | bitfield

        l_brace,                // {
        r_brace,                // }

        l_bracket,              // [
        r_bracket,              // ]

        equal,                  // =
        end_statement,          // ;
        dot,                    // .
        comma,                  // ,
        colon,                  // :    scope operator :: will be handled by parser.

        comment,
        multiline_comment,

        whitespace,             // space \t \r \n
    };

    std::ostream& operator<<(std::ostream& os, const TokenType type);
	
    TokenType CharToTokenType(const char c);
}}
