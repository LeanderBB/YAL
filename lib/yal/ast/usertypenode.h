#ifndef __YAL_USERTYPENODE_H__
#define __YAL_USERTYPENODE_H__

#include "yal/ast/astbasenode.h"

namespace yal
{

class UserTypeMemberNode : public AstBaseNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(UserTypeMemberNode)

    UserTypeMemberNode(const SourceLocationInfo& loc,
               Type* type,
               const char* name);

    virtual ~UserTypeMemberNode() {}

    const char* memberName() const
    {
        return _memberName;
    }

    Type* memberType() const
    {
        return _memberType;
    }

private:
    const char* _memberName;
    Type* _memberType;
};

typedef YALVector<std::unique_ptr<UserTypeMemberNode>> UserTypeMemberVec_t;


class UserTypeMembersNode: public AstBaseNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(UserTypeMembersNode)

    UserTypeMembersNode(const SourceLocationInfo& loc);

    virtual ~UserTypeMembersNode() {}

    void addUserTypeMember(UserTypeMemberNode* member);


    size_t memberCount() const
    {
        return _members.size();
    }

    UserTypeMemberVec_t& members()
    {
        return _members;
    }

private:
    UserTypeMemberVec_t _members;
};


class UserTypeDeclNode : public StatementNode
{
public:

    YAL_AST_NODE_ACCEPT_HDR(UserTypeDeclNode)

    UserTypeDeclNode(const SourceLocationInfo& loc,
                 const char* typeName,
                 UserTypeMembersNode* members);

    virtual ~UserTypeDeclNode();


    UserTypeMembersNode* members() const
    {
        return _members.get();
    }

    const char* userTypeName() const
    {
        return _userTypeName;
    }

protected:
    const char* _userTypeName;
    std::unique_ptr<UserTypeMembersNode> _members;
};

}
#endif
