#pragma once 
#include <boost/utility/string_view.hpp>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace types { namespace utils {

    std::uint64_t setValueFromChar(const boost::string_view& underlying, const lexer::TokenInfo& token);
}}}