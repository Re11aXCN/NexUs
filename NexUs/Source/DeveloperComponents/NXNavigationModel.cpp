#include "NXNavigationModel.h"

#include <QIcon>
#include <QJsonObject>

#include "NXNavigationNode.h"
NXNavigationModel::NXNavigationModel(QObject* parent)
    : QAbstractItemModel{parent}
{
    _rootNode = new NXNavigationNode("root");
    _rootNode->setIsRootNode(true);
    _rootNode->setIsExpanderNode(true);
    _pSelectedNode = nullptr;
    _pSelectedExpandedNode = nullptr;
}

NXNavigationModel::~NXNavigationModel()
{
    delete _rootNode;
}

QModelIndex NXNavigationModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }
    NXNavigationNode* childNode = static_cast<NXNavigationNode*>(child.internalPointer());
    NXNavigationNode* parentNode = childNode->getParentNode();
    if (parentNode == _rootNode)
    {
        return QModelIndex();
    }
    else if (parentNode == nullptr)
    {
        return QModelIndex();
    }
    return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex NXNavigationModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    NXNavigationNode* parentNode;
    if (!parent.isValid())
    {
        parentNode = _rootNode;
    }
    else
    {
        parentNode = static_cast<NXNavigationNode*>(parent.internalPointer());
    }
    NXNavigationNode* childNode = nullptr;
    if (parentNode->getChildrenNodes().count() > row)
    {
        childNode = parentNode->getChildrenNodes().at(row);
    }
    if (childNode)
    {
        return createIndex(row, column, childNode);
    }
    return QModelIndex();
}

int NXNavigationModel::rowCount(const QModelIndex& parent) const
{
    NXNavigationNode* parentNode;
    if (parent.column() > 0)
    {
        return 0;
    }
    if (!parent.isValid())
    {
        parentNode = _rootNode;
    }
    else
    {
        parentNode = static_cast<NXNavigationNode*>(parent.internalPointer());
    }
    return parentNode->getChildrenNodes().count();
};

int NXNavigationModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant NXNavigationModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    return QVariant();
}

NodeOperateReturnTypeWithKey NXNavigationModel::addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome)
{
    NXNavigationNode* node = new NXNavigationNode(expanderTitle, _rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    return qMakePair(NXNavigationType::NodeOperateReturnType::Success, node->getNodeKey());
}

NodeOperateReturnTypeWithKey NXNavigationModel::addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeInvalid, QString{});
    }
    NXNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeTypeError, QString{});
    }
    if (parentNode->getDepth() > 10)
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeDepthLimit, QString{});
    }
    NXNavigationNode* node = new NXNavigationNode(expanderTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    return qMakePair(NXNavigationType::NodeOperateReturnType::Success, node->getNodeKey());
}

NodeOperateReturnTypeWithKey NXNavigationModel::addPageNode(const QString& pageTitle, NXIconType::IconName awesome)
{
    NXNavigationNode* node = new NXNavigationNode(pageTitle, _rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return qMakePair(NXNavigationType::NodeOperateReturnType::Success, node->getNodeKey());
}

NodeOperateReturnTypeWithKey NXNavigationModel::addPageNode(const QString& pageTitle, const QString& targetExpanderKey, NXIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeInvalid, QString{});
    }
    NXNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeTypeError, QString{});
    }
    if (parentNode->getDepth() > 10)
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeDepthLimit, QString{});
    }
    NXNavigationNode* node = new NXNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return qMakePair(NXNavigationType::NodeOperateReturnType::Success, node->getNodeKey());
}

NodeOperateReturnTypeWithKey NXNavigationModel::addPageNode(const QString& pageTitle, int keyPoints, NXIconType::IconName awesome)
{
    NXNavigationNode* node = new NXNavigationNode(pageTitle, _rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setKeyPoints(keyPoints);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return qMakePair(NXNavigationType::NodeOperateReturnType::Success, node->getNodeKey());
}

NodeOperateReturnTypeWithKey NXNavigationModel::addPageNode(const QString& pageTitle, const QString& targetExpanderKey, int keyPoints, NXIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeInvalid, QString{});
    }
    NXNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeTypeError, QString{});
    }
    if (parentNode->getDepth() > 10)
    {
        return qMakePair(NXNavigationType::NodeOperateReturnType::TargetNodeDepthLimit, QString{});
    }
    NXNavigationNode* node = new NXNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    node->setKeyPoints(keyPoints);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return qMakePair(NXNavigationType::NodeOperateReturnType::Success, node->getNodeKey());
}

QStringList NXNavigationModel::removeNavigationNode(const QString& nodeKey)
{
    QList<QString> removeKeyList;
    if (!_nodesMap.contains(nodeKey))
    {
        return removeKeyList;
    }
    NXNavigationNode* node = _nodesMap.value(nodeKey);
    NXNavigationNode* parentNode = node->getParentNode();
    if (node->getIsExpanderNode())
    {
        QList<NXNavigationNode*> childNodeList = node->getChildrenNodes();
        for (int i = 0; i < childNodeList.count(); i++)
        {
            NXNavigationNode* childNode = childNodeList[i];
            QList<QString> childRemoveKeyList = removeNavigationNode(childNode->getNodeKey());
            removeKeyList.append(childRemoveKeyList);
        }
    }
    else
    {
        removeKeyList.append(node->getNodeKey());
    }
    beginRemoveRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count() - 1, parentNode->getChildrenNodes().count() - 1);
    parentNode->removeChildNode(node);
    _nodesMap.remove(node->getNodeKey());
    endRemoveRows();
    return removeKeyList;
}


NXNavigationNode* NXNavigationModel::getNavigationNode(const QString& nodeKey) const
{
    if (_nodesMap.contains(nodeKey))
    {
        return _nodesMap.value(nodeKey);
    }
    return nullptr;
}

QList<NXNavigationNode*> NXNavigationModel::getRootExpanderNodes() const
{
    QList<NXNavigationNode*> expandedNodeList;
    for (auto node : _rootNode->getChildrenNodes())
    {
        if (node->getIsExpanderNode())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}

QList<NXNavigationNode*> NXNavigationModel::getRootExpandedNodes() const
{
    QList<NXNavigationNode*> expandedNodeList;
    for (auto node : _rootNode->getChildrenNodes())
    {
        if (node->getIsExpanderNode() && node->getIsExpanded())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}
