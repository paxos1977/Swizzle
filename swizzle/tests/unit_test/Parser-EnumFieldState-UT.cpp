#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/EnumFieldState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct EnumFieldStateFixture
    {
        EnumFieldStateFixture()
        {
            nodeStack.push(ast.root());

            const auto enumInfo = TokenInfo(Token("enum", 0, 3, TokenType::keyword), FileInfo("test.swizzle"));
            const auto enumName = TokenInfo(Token("my_enum", 0, 7, TokenType::string), FileInfo("test.swizzle"));

            auto node = swizzle::parser::detail::appendNode<nodes::Enum>(nodeStack, enumInfo, enumName, "my_namespace");
            nodeStack.push(node);

            const TokenInfo info(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle"));
            const TokenInfo underlying(Token("u8", 0, 2, TokenType::type), FileInfo("test.swizzle"));

            node = swizzle::parser::detail::appendNode<nodes::EnumField>(nodeStack, info, underlying);
            nodeStack.push(node);
        }

        states::EnumFieldState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(EnumFieldStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsComma : public EnumFieldStateFixture
    {
        const Token token = Token(",", 0, 1, TokenType::comma);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsComma, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK_EQUAL(6, context.CurrentEnumValue.value().which());
        CHECK_EQUAL(0U, boost::get<std::uint64_t>(context.CurrentEnumValue.value()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK_EQUAL(6, context.CurrentEnumValue.value().which());
        CHECK_EQUAL(1U, boost::get<std::uint64_t>(context.CurrentEnumValue.value()));   // ensure the current enum value was incremented

        auto matcher = Matcher().getChildrenOf<nodes::EnumField>().bind("fields");
        REQUIRE CHECK(matcher(nodeStack.top()));

        auto node = matcher.bound("fields_0");
        const auto& field = dynamic_cast<nodes::EnumField&>(*node);
        const auto& value = field.value();

        REQUIRE CHECK_EQUAL(6, value.which());
        CHECK_EQUAL(0U, boost::get<std::uint64_t>(value));
    }

    struct WhenNextTokenIsEqual : public EnumFieldStateFixture
    {
        const Token token = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEqual, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumFieldEqualRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public EnumFieldStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsCommaButTopOfStackIsNotFieldEnum : public EnumFieldStateFixture
    {
        WhenNextTokenIsCommaButTopOfStackIsNotFieldEnum()
        {
            nodeStack.pop();
        }

        const Token token = Token(",", 0, 1, TokenType::comma);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsCommaButTopOfStackIsNotFieldEnum, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsCommaButValueWillOverflowEnumType : public WhenNextTokenIsComma
    {
        WhenNextTokenIsCommaButValueWillOverflowEnumType()
        {
            EnumValueType val = std::numeric_limits<std::uint8_t>::max();
            context.CurrentEnumValue = val;
        }
    };

    TEST_FIXTURE(WhenNextTokenIsCommaButValueWillOverflowEnumType, verifyConsumer)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
