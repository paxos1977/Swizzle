#include <swizzle/ast/nodes/Namespace.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Namespace::Namespace(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& Namespace::info() const
    {
        return info_;
    }

    void Namespace::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
    {
        visitor(parent, *this);
        if(depth == Depth::One) return;
        
        for(auto& child : children())
        {
            auto parent = this;
            child->accept(visitor, *parent, depth);
        }
    }

}}}
