#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class FieldLabel : public Node
    {
    public:
        FieldLabel(const lexer::TokenInfo& info);

        const lexer::TokenInfo& info() const;

        void colon(const lexer::TokenInfo& colon);
        const lexer::TokenInfo& colon() const;

        void accept(VisitorInterface& visitor) override;

    private:
        const lexer::TokenInfo info_;
        lexer::TokenInfo colon_;
    };
}}}
