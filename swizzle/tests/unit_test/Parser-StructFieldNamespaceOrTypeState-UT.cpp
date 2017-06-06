#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructFieldNamespaceOrTypeState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructFieldNamespaceOrTypeStateFixture
    {
        StructFieldNamespaceOrTypeStateFixture()
        {
            nodeStack.push(ast.root());

            const auto structKeyword = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            auto node = detail::appendNode<nodes::Struct>(nodeStack, structKeyword, name, "my_namespace");
            nodeStack.push(node);
        }

        states::StructFieldNamespaceOrTypeState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructFieldNamespaceOrTypeStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public StructFieldNamespaceOrTypeStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceFirstColon, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsStringAndTypeIsU8 : public StructFieldNamespaceOrTypeStateFixture
    {
        WhenNextTokenIsStringAndTypeIsU8()
        {
            auto node = detail::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);

            const Token t = Token("u8", 0, 2, TokenType::type);
            const FileInfo f = FileInfo("test.swizzle");

            tokenStack.push(TokenInfo(t, f));
        }

        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsStringAndTypeIsU8, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: verify the AST
    }

    struct WhenNextTokenIsStringAndTypeIsU8AndTopOfStackIsNotStructField : public StructFieldNamespaceOrTypeStateFixture
    {
        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsStringAndTypeIsU8AndTopOfStackIsNotStructField, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsStringAndTypeIsUserTypeWithNamespace : public StructFieldNamespaceOrTypeStateFixture
    {
        WhenNextTokenIsStringAndTypeIsUserTypeWithNamespace()
        {
            auto node = detail::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);

            const std::string s = "foo::bar::MyType";

            const Token t1 = Token(s, 0, 3, TokenType::string);
            const FileInfo f1 = FileInfo("test.swizzle", LineInfo(1U, 1U), LineInfo(1U, 4U));
            tokenStack.push(TokenInfo(t1, f1));

            const Token t2 = Token(s, 5, 3, TokenType::string);
            const FileInfo f2 = FileInfo("test.swizzle", LineInfo(1U, 6U), LineInfo(1U, 9U));
            tokenStack.push(TokenInfo(t2, f2));

            const Token t3 = Token(s, 10, 6, TokenType::string);
            const FileInfo f3 = FileInfo("test.swizzle", LineInfo(1U, 11U), LineInfo(1U, 17U));
            tokenStack.push(TokenInfo(t3, f3));

            context.TypeCache["foo::bar::MyType"] = new Node();
        }

        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsStringAndTypeIsUserTypeWithNamespace, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(3U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: verify the AST
    }

    struct WhenNextTokenIsStringAndTypeIsUserTypeWithoutNamespace : public StructFieldNamespaceOrTypeStateFixture
    {
        WhenNextTokenIsStringAndTypeIsUserTypeWithoutNamespace()
        {
            auto node = detail::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);

            const Token t3 = Token("MyType", 0, 6, TokenType::string);
            const FileInfo f3 = FileInfo("test.swizzle", LineInfo(1U, 11U), LineInfo(1U, 17U));
            tokenStack.push(TokenInfo(t3, f3));

            context.CurrentNamespace = "foo::bar";
            context.TypeCache["foo::bar::MyType"] = new Node();
        }

        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsStringAndTypeIsUserTypeWithoutNamespace, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: verify the AST
    }

    struct WhenNextTokenIsInvalid : public StructFieldNamespaceOrTypeStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }
}