#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast {

    AbstractSyntaxTree::AbstractSyntaxTree()
        : root_(new Node())
    {
    }

    const Node::smartptr AbstractSyntaxTree::root() const
    {
        return root_;
    }

    Node::smartptr AbstractSyntaxTree::root()
    {
        return root_;
    }

    void AbstractSyntaxTree::accept(VisitorInterface& visitor)
    {
        root_->accept(visitor);
    }
}}
