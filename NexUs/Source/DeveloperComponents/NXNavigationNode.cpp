﻿#include "NXNavigationNode.h"

#include <QUuid>

NXNavigationNode::NXNavigationNode(const QString& nodeTitle, NXNavigationNode* parent)
    : QObject(parent)
{
    _pDepth = 0;
    _pKeyPoints = 0;
    _nodeKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _nodeTitle = nodeTitle;
    _pIsRootNode = false;
    _pIsFooterNode = false;
    _pIsHasFooterPage = false;
    _pParentNode = parent;
    _pIsExpanderNode = false;
    _pIsVisible = false;
}

NXNavigationNode::~NXNavigationNode()
{
    //qDebug() << std::format("Title: {} Key: {}\n", _nodeTitle.toStdString(), _nodeKey.toStdString());
    //qDebug() << _nodeTitle + " Prepare to delete " __FUNCTION__ "\n";
    //qDeleteAll(_pChildrenNodes);
    //qDebug() << _nodeTitle + " Already delete " __FUNCTION__ "\n";
}

QString NXNavigationNode::getNodeKey() const
{
    return _nodeKey;
}


void NXNavigationNode::setNodeTitle(const QString& nodeTitle)
{
    _nodeTitle = nodeTitle;
}
QString NXNavigationNode::getNodeTitle() const
{
    return _nodeTitle;
}

void NXNavigationNode::setIsExpanded(bool isExpanded)
{
    _isExpanded = isExpanded;
    setChildVisible(isExpanded);
}

bool NXNavigationNode::getIsExpanded() const
{
    return _isExpanded;
}

void NXNavigationNode::setChildVisible(bool isVisible)
{
    if (isVisible)
    {
        for (auto node : _pChildrenNodes)
        {
            node->setIsVisible(isVisible);
            if (node->getIsExpanderNode() && !node->getIsExpanded())
            {
                continue;
            }
            node->setChildVisible(isVisible);
        }
    }
    else
    {
        for (auto node : _pChildrenNodes)
        {
            node->setChildVisible(isVisible);
            node->setIsVisible(isVisible);
        }
    }
}

bool NXNavigationNode::getIsHasChild() const
{
    if (_pChildrenNodes.count() > 0)
    {
        return true;
    }
    return false;
}

bool NXNavigationNode::getIsHasPageChild() const
{
    if (_pChildrenNodes.count() == 0)
    {
        return false;
    }
    for (auto childNode : _pChildrenNodes)
    {
        if (!childNode->getIsExpanderNode())
        {
            return true;
        }
        if (childNode->getIsHasPageChild())
        {
            return true;
        }
    }
    return false;
}

void NXNavigationNode::appendChildNode(NXNavigationNode* childNode)
{
    if (_pIsExpanderNode)// 根节点也是ExpanderNode
    {
        _pChildrenNodes.append(childNode);
    }
}

void NXNavigationNode::removeChildNode(NXNavigationNode* childNode)
{
    if (_pIsExpanderNode)
    {
        _pChildrenNodes.removeOne(childNode);
    }
}

void NXNavigationNode::insertChildNode(int row, NXNavigationNode* childNode)
{
    if (row < 0 || row > _pChildrenNodes.count()) return;
    _pChildrenNodes.insert(row, childNode);
    childNode->setParentNode(this);
    childNode->setParent(this);
}

bool NXNavigationNode::getIsChildHasKeyPoints() const
{
    for (auto childNnode : _pChildrenNodes)
    {
        if (childNnode->getKeyPoints())
        {
            return true;
        }
        if (childNnode->getIsChildHasKeyPoints())
        {
            return true;
        }
    }
    return false;
}

NXNavigationNode* NXNavigationNode::getOriginalNode()
{
    if (this->getParentNode()->getIsRootNode())
    {
        return this;
    }
    else
    {
        NXNavigationNode* originalNode = this->getParentNode();
        while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode())
        {
            originalNode = originalNode->getParentNode();
        }
        return originalNode;
    }
}

bool NXNavigationNode::getIsChildNode(NXNavigationNode* node) const
{
    if (_pChildrenNodes.count() > 0)
    {
        if (_pChildrenNodes.contains(node))
        {
            return true;
        }
        for (auto childNnode : _pChildrenNodes)
        {
            if (childNnode->getIsChildNode(node))
            {
                return true;
            }
        }
    }
    return false;
}

int NXNavigationNode::getRow() const
{
    if (_pParentNode)
    {
        return _pParentNode->getChildrenNodes().indexOf(const_cast<NXNavigationNode*>(this));
    }
    return 0;
}

void NXNavigationNode::swapShowInfo(NXNavigationNode* other) 
{
    if(this == other || other == nullptr) return;
    std::swap(this->_pAwesome, other->_pAwesome);
    std::swap(this->_pKeyPoints, other->_pKeyPoints);
    std::swap(this->_nodeTitle, other->_nodeTitle);
    std::swap(this->_nodeKey, other->_nodeKey);
}
