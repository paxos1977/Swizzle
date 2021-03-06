#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/InitState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct InitStateFixture
    {
        InitStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::InitState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(InitStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsComment : public InitStateFixture
    {
        const Token token = Token("// this is a comment", 0, 20, TokenType::comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsComment, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::Init, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().getChildrenOf<nodes::Comment>().bind("comment");
        matcher(nodeStack.top());

        const auto commentNode = matcher.bound("comment_0");
        REQUIRE CHECK(commentNode);

        const auto comment = static_cast<nodes::Comment&>(*commentNode);
        CHECK_EQUAL(TokenType::comment, comment.info().token().type());
        CHECK_EQUAL("// this is a comment", comment.info().token().to_string());
    }

    struct WhenNextTokenIsMultilineComment : public InitStateFixture
    {
        const Token token = Token("// this is a \\\n multiline comment", 0, 33, TokenType::multiline_comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::Init, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().getChildrenOf<nodes::MultilineComment>().bind("comment");
        matcher(nodeStack.top());

        const auto commentNode = matcher.bound("comment_0");
        REQUIRE CHECK(commentNode);

        const auto& comment = static_cast<nodes::MultilineComment&>(*commentNode);
        CHECK_EQUAL(TokenType::multiline_comment, comment.info().token().type());
        CHECK_EQUAL("// this is a \\\n multiline comment", comment.info().token().to_string());
    }

    struct WhenNextTokenIsImport : public InitStateFixture
    {
        const Token token = Token("import", 0, 6, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsImport, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartImport, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsNamespace : public InitStateFixture
    {
        const Token token = Token("namespace", 0, 9, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNamespace, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartNamespace, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public InitStateFixture
    {
        const Token token = Token("struct", 0, 6, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
