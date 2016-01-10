#include "yal/ast/usertypenode.h"
#include "yal/ast/argumentdeclnode.h"

namespace yal
{

YAL_AST_NODE_ACCEPT_IMP(UserTypeMemberNode)

UserTypeMemberNode::UserTypeMemberNode(const SourceLocationInfo& loc,
           Type* type,
           const char* name):
    AstBaseNode(loc),
    _memberName(name),
    _memberType(type)
{

}



YAL_AST_NODE_ACCEPT_IMP(UserTypeMembersNode)


UserTypeMembersNode::UserTypeMembersNode(const SourceLocationInfo& loc):
    AstBaseNode(loc),
    _members()
{

}

void
UserTypeMembersNode::addUserTypeMember(UserTypeMemberNode* member)
{
    member->setParentNode(this);
    _members.emplace_back(member);
}


YAL_AST_NODE_ACCEPT_IMP(UserTypeDeclNode)

UserTypeDeclNode::UserTypeDeclNode(const SourceLocationInfo& loc,
                           const char* typeName,
                           UserTypeMembersNode *members):
    StatementNode(loc),
    _userTypeName(typeName),
    _members(members)
{
    _members->setParentNode(this);
}


UserTypeDeclNode::~UserTypeDeclNode()
{
    yal_free(const_cast<char*>(_userTypeName));
}
}
