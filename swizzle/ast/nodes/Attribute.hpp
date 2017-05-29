#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class Attribute : public Node
    {
    public:
        Attribute(const lexer::TokenInfo& info);

        const lexer::TokenInfo& info() const;

        void accept(VisitorInterface& visitor);

    private:
        const lexer::TokenInfo info_;
    };
}}}
