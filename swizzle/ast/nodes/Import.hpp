#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <boost/filesystem/path.hpp>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class Import : public Node
    {
    public:
        Import(const lexer::TokenInfo& info, const boost::filesystem::path& path);

        const lexer::TokenInfo& info() const;
        const boost::filesystem::path& path() const;

        void accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth = Node::Depth::All) override;
        
    private:
        const lexer::TokenInfo info_;
        const boost::filesystem::path importPath_;
    };
}}}
