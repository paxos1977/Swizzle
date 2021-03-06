#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/ImportValueState.hpp>

#include <boost/filesystem.hpp>
#include <iostream>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct ImportValueStateFixture
    {
        ImportValueStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::ImportValueState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(ImportValueStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public ImportValueStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::ImportFirstColon, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsEndStatement : public ImportValueStateFixture
    {
        WhenNextTokenIsEndStatement()
        {
            const Token t = Token("MyType", 0, 6, TokenType::string);
            const FileInfo fi = FileInfo("test.swizzle");

            tokenStack.push(TokenInfo(t, fi));

            boost::filesystem::fstream file(testFile, std::ios::out | std::ios::app);
        }

        ~WhenNextTokenIsEndStatement()
        {
            boost::filesystem::remove(testFile);
        }

        const boost::filesystem::path testFile = boost::filesystem::path("./MyType.swizzle");

        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatement, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().hasChildOf<nodes::Import>();
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsEndStatementAndFileHasPath : public ImportValueStateFixture
    {
        WhenNextTokenIsEndStatementAndFileHasPath()
        {
            const Token t1 = Token("foo", 0, 3, TokenType::string);
            const FileInfo fi1 = FileInfo("test.swizzle");

            const Token t2 = Token("bar", 0, 3, TokenType::string);
            const FileInfo fi2 = FileInfo("test.swizzle");

            const Token t3 = Token("MyType", 0, 6, TokenType::string);
            const FileInfo fi3 = FileInfo("test.swizzle");

            tokenStack.push(TokenInfo(t1, fi1));
            tokenStack.push(TokenInfo(t2, fi2));
            tokenStack.push(TokenInfo(t3, fi3));

            boost::filesystem::path base(t1.to_string());
            base /= t2.to_string();

            boost::filesystem::create_directories(base);
            boost::filesystem::fstream file(testFile, std::ios::out | std::ios::app);
        }

        ~WhenNextTokenIsEndStatementAndFileHasPath()
        {
            boost::filesystem::remove(testFile);
            boost::filesystem::remove_all("./foo");
        }

        const boost::filesystem::path testFile = boost::filesystem::path("./foo/bar/MyType.swizzle");

        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatementAndFileHasPath, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(3U, tokenStack.size());

        state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().hasChildOf<nodes::Import>();
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsEndStatementAndImportFileIsDirectory : public ImportValueStateFixture
    {
        WhenNextTokenIsEndStatementAndImportFileIsDirectory()
        {
            const Token t = Token("MyType", 0, 6, TokenType::string);
            const FileInfo fi = FileInfo("test.swizzle");

            tokenStack.push(TokenInfo(t, fi));

            boost::filesystem::create_directories(testFile);
        }

        ~WhenNextTokenIsEndStatementAndImportFileIsDirectory()
        {
            boost::filesystem::remove(testFile);
        }

        const boost::filesystem::path testFile = boost::filesystem::path("./MyType.swizzle");

        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatementAndImportFileIsDirectory, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsEndStatementAndTokenStackIsEmpty : public ImportValueStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatementAndTokenStackIsEmpty, verifyConsume)
    {
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsInvalid : public ImportValueStateFixture
    {
        const Token token = Token("blah", 0, 1, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
