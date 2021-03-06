#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/lexer/states/InitState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenizerState.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/lexer/FileInfo.hpp>

#include <boost/utility/string_view.hpp>
#include <deque>
#include <string>

namespace {
    using namespace swizzle::lexer;

    class CreateTokenCallback
    {
    public:
        CreateTokenCallback(std::deque<TokenInfo>& tokens)
            : tokens_(tokens)
        {
        }

        void operator()(const TokenInfo& info)
        {
            tokens_.push_back(info);
        }

        void operator()(TokenInfo&& info)
        {
            tokens_.emplace_back(info);
        }

    private:
        std::deque<TokenInfo>& tokens_;
    };

    struct InitStateFixture
    {
        std::deque<TokenInfo> tokens;
        CreateTokenCallback callback = CreateTokenCallback(tokens);

        Token token = Token(boost::string_view(), 0, 0, TokenType::numeric_literal);
        FileInfo info = FileInfo("testfile");

        states::InitState<CreateTokenCallback> state = states::InitState<CreateTokenCallback>(callback);

        std::size_t position = 0;
    };

    TEST_FIXTURE(InitStateFixture, verifyConstruction)
    {
    }

    struct WhenNextCharIs1 : public InitStateFixture
    {
        const std::string s = "1";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs1, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("1", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIs0 : public InitStateFixture
    {
        const std::string s = "0";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIs0, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::BeginHexLiteral, tokenState);
        CHECK_EQUAL(TokenType::hex_literal, token.type());
        CHECK_EQUAL("0", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsNegative : public InitStateFixture
    {
        const std::string s = "-";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNegative, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::NumericLiteral, tokenState);
        CHECK_EQUAL(TokenType::numeric_literal, token.type());
        CHECK_EQUAL("-", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsSingleQuote : public InitStateFixture
    {
        const std::string s = "'";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSingleQuote, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::CharLiteral, tokenState);
        CHECK_EQUAL(TokenType::char_literal, token.type());
        CHECK_EQUAL("'", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsDoubleQuote : public InitStateFixture
    {
        const std::string s = "\"";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsDoubleQuote, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::StringLiteral, tokenState);
        CHECK_EQUAL(TokenType::string_literal, token.type());
        CHECK_EQUAL("\"", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsSpace : public InitStateFixture
    {
        const std::string s = " ";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSpace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK_EQUAL(TokenType::whitespace, token.type());
        CHECK_EQUAL(" ", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsTab : public InitStateFixture
    {
        const std::string s = "\t";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsTab, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK_EQUAL(TokenType::whitespace, token.type());
        CHECK_EQUAL("\t", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsReturn : public InitStateFixture
    {
        const std::string s = "\r";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsReturn, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK_EQUAL(TokenType::whitespace, token.type());
        CHECK_EQUAL("\r", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsNewLine : public InitStateFixture
    {
        const std::string s = "\n";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsNewLine, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);
        CHECK_EQUAL(TokenType::whitespace, token.type());
        CHECK_EQUAL("\n", token.to_string());

        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsLeftBracket : public InitStateFixture
    {
        const std::string s = "[";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsLeftBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::l_bracket, tokens[0].token().type());
        CHECK_EQUAL("[", tokens[0].token().to_string());
    }

    struct WhenNextCharIsRightBracket : public InitStateFixture
    {
        const std::string s = "]";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBracket, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::r_bracket, tokens[0].token().type());
        CHECK_EQUAL("]", tokens[0].token().to_string());
    }

    struct WhenNextCharIsLeftBrace : public InitStateFixture
    {
        const std::string s = "{";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsLeftBrace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::l_brace, tokens[0].token().type());
        CHECK_EQUAL("{", tokens[0].token().to_string());
    }

    struct WhenNextCharIsRightBrace : public InitStateFixture
    {
        const std::string s = "}";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsRightBrace, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::r_brace, tokens[0].token().type());
        CHECK_EQUAL("}", tokens[0].token().to_string());
    }

    struct WhenNextCharIsDot : public InitStateFixture
    {
        const std::string s = ".";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsDot, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::dot, tokens[0].token().type());
        CHECK_EQUAL(".", tokens[0].token().to_string());
    }

    struct WhenNextCharIsEqual : public InitStateFixture
    {
        const std::string s = "=";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsEqual, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::equal, tokens[0].token().type());
        CHECK_EQUAL("=", tokens[0].token().to_string());
    }

    struct WhenNextCharIsAt : public InitStateFixture
    {
        const std::string s = "@";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsAt, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Attribute, tokenState);
        CHECK_EQUAL(TokenType::attribute, token.type());
        CHECK_EQUAL("@", token.to_string());
        
        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsComma : public InitStateFixture
    {
        const std::string s = ",";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsComma, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::comma, tokens[0].token().type());
        CHECK_EQUAL(",", tokens[0].token().to_string());
    }

    struct WhenNextCharIsColon : public InitStateFixture
    {
        const std::string s = ":";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::colon, tokens[0].token().type());
        CHECK_EQUAL(":", tokens[0].token().to_string());
    }

    struct WhenNextCharIsSemiColon : public InitStateFixture
    {
        const std::string s = ";";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSemiColon, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::Init, tokenState);

        REQUIRE CHECK_EQUAL(1U, tokens.size());
        CHECK_EQUAL(TokenType::end_statement, tokens[0].token().type());
        CHECK_EQUAL(";", tokens[0].token().to_string());
    }

    struct WhenNextCharIsSlash : public InitStateFixture
    {
        const std::string s = "/";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsSlash, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::FirstSlash, tokenState);
        CHECK_EQUAL(0U, tokens.size());
    }

    struct WhenNextCharIsAlpha : public InitStateFixture
    {
        const std::string s = "abc";
        const boost::string_view sv = boost::string_view(s);
    };

    TEST_FIXTURE(WhenNextCharIsAlpha, verifyConsume)
    {
        CHECK_EQUAL(0U, tokens.size());

        auto tokenState = state.consume(sv, position, info, token);

        CHECK_EQUAL(TokenizerState::BeginString, tokenState);
        CHECK_EQUAL(0U, tokens.size());
    }
}
