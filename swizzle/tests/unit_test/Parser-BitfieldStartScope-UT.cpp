#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/BitfieldStartScopeState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct BitfieldStartScopeStateFixture
    {
        BitfieldStartScopeStateFixture()
        {
            nodeStack.push(ast.root());

            const auto bitfieldInfo = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            const auto bitfieldName = TokenInfo(Token("my_bitfield", 0, 11, TokenType::string), FileInfo("test.swizzle"));

            const auto node = swizzle::parser::detail::appendNode<nodes::Bitfield>(nodeStack, bitfieldInfo, bitfieldName, "my_namespace");
            nodeStack.push(node);
        }

        states::BitfieldStartScopeState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(BitfieldStartScopeStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsComment : public BitfieldStartScopeStateFixture
    {
        const Token token = Token("// comment", 0, 10, TokenType::comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::Comment>();

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsMultilineComment : public BitfieldStartScopeStateFixture
    {
        const Token token = Token("// comment \\\n comment2", 0, 21, TokenType::multiline_comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::MultilineComment>();

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsFieldName : public BitfieldStartScopeStateFixture
    {
        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsFieldName, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(detail::nodeStackTopIs<nodes::Bitfield>(nodeStack));
        auto matcher = Matcher().hasChildOf<nodes::BitfieldField>();
        CHECK(!matcher(nodeStack.top()));       // nodeStack.top() is bitfield

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldField, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(detail::nodeStackTopIs<nodes::BitfieldField>(nodeStack));
    }

    struct WhenNextTokenIsRightBrace : public BitfieldStartScopeStateFixture
    {
        WhenNextTokenIsRightBrace()
        {
            auto& top = dynamic_cast<nodes::Bitfield&>(*nodeStack.top());
            top.append(new Node());
        }

        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBrace, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public BitfieldStartScopeStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsFieldNameAndTopOfStackIsNotBitfield : public BitfieldStartScopeStateFixture
    {
        WhenNextTokenIsFieldNameAndTopOfStackIsNotBitfield()
        {
            nodeStack.pop();
        }

        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsFieldNameAndTopOfStackIsNotBitfield, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsRightBraceAndTopOfStackIsNotBitfield : public BitfieldStartScopeStateFixture
    {
        WhenNextTokenIsRightBraceAndTopOfStackIsNotBitfield()
        {
            nodeStack.pop();
        }

        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBraceAndTopOfStackIsNotBitfield, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsRightBraceButNoFieldsWereCreated : public BitfieldStartScopeStateFixture
    {
        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBraceButNoFieldsWereCreated, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
