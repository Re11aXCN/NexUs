#include "NXNavigationModel.h"

#include <QIcon>
#include <QJsonObject>
#include <QMimeData>
#include <QIODevice>
#include "NXNavigationNode.h"
NXNavigationModel::NXNavigationModel(QObject* parent)
    : QAbstractItemModel{parent}
{
    _rootNode = new NXNavigationNode("root");
    _rootNode->setIsRootNode(true);
    _rootNode->setIsExpanderNode(true);
    _rootNode->setIsExpanded(true);
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

Qt::ItemFlags NXNavigationModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    auto flags = Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
    if (index.column() != 0)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
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

Qt::DropActions NXNavigationModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions NXNavigationModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

QStringList NXNavigationModel::mimeTypes() const
{
    return QStringList() << "application/x-nxnavigation-node";
}

QMimeData* NXNavigationModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* mimeData = new QMimeData();
    if (indexes.isEmpty()) return mimeData;

    QModelIndex index = indexes.first();
    NXNavigationNode* node = static_cast<NXNavigationNode*>(index.internalPointer());
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << node->getNodeKey() << node->getNodeTitle();
    mimeData->setData("application/x-nxnavigation-node", encodedData);
    return mimeData;
}

bool NXNavigationModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const
{
    if (!data->hasFormat("application/x-nxnavigation-node")
        || action != Qt::MoveAction
        || _nodesMap.size() <= 1)
        return false;

    return true;// 已经有dropEvent了，这里可以不用再次验证
    //const QModelIndex targetParentIndex = parent;
    //const QModelIndex targetIndex = index(row, column, targetParentIndex);
    //
    //QByteArray encodedData = data->data("application/x-nxnavigation-node");
    //QDataStream stream(&encodedData, QIODevice::ReadOnly);
    //QString nodeKey; stream >> nodeKey;
    //NXNavigationNode* targetParentNode = targetParentIndex.isValid() ?
    //    static_cast<NXNavigationNode*>(targetParentIndex.internalPointer()) : _rootNode;
    ///* NXNavigationNode* targetNode = targetIndex.isValid() ?
    //    static_cast<NXNavigationNode*>(targetIndex.internalPointer()) : nullptr;*/
    //NXNavigationNode* draggedNode = _nodesMap.value(nodeKey);
    //// 展开状态验证，根节点一定是展开状态,expenderNode不能拖拽
    ///*if (targetNode->getIsExpanderNode() &&
    //    !targetNode->getIsExpanded()) {
    //    return  ((targetParentNode->getIsRootNode() ? _rootNode
    //        : targetParentNode->getParentNode()) == draggedNode->getParentNode());
    //}*/
    //
    //// 类型组合验证
    //if (targetParentNode->getIsRootNode()) {
    //    // 根节点接受所有类型
    //    return true;
    //}
    //else if (draggedNode->getIsExpanderNode()) {
    //    // Expander只能插入到Expander或根节点下
    //    return targetParentNode->getIsExpanderNode();
    //}
    //else {
    //    // PageNode可以插入到任何Expander下
    //    return targetParentNode->getIsExpanderNode();
    //}
}

bool NXNavigationModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    if (!data->hasFormat("application/x-nxnavigation-node")
        || action != Qt::MoveAction
        || _nodesMap.size() <= 1)
        return false;

    int targetRow = row;
    const QModelIndex targetParentIndex = parent;
    const QModelIndex targetIndex = index(targetRow, column, targetParentIndex);

    QByteArray encodedData = data->data("application/x-nxnavigation-node");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString nodeKey, nodeTitle;
    stream >> nodeKey >> nodeTitle;
    qDebug() << "nodeKey: " << nodeKey << "\n nodeTitle: " << nodeTitle;
    NXNavigationNode* draggedNode = _nodesMap.value(nodeKey);
    int oldRow = draggedNode->getRow();
    // pagenode 是0 移到 expandernode 下的第0个节点会被跳出，不对
    if (!draggedNode || draggedNode->getRow() == targetRow) return false;

    NXNavigationNode* targetNode = static_cast<NXNavigationNode*>(targetIndex.internalPointer());
    NXNavigationNode* draggedParentNode = draggedNode->getParentNode();
    NXNavigationNode* targetParentNode = targetParentIndex.isValid() ?
        static_cast<NXNavigationNode*>(targetParentIndex.internalPointer()) : _rootNode;
    const bool isSameParent = (draggedParentNode == targetParentNode);

    // 计算实际插入位置
    int totalRow = targetParentNode->getChildrenNodes().count();
    int newRow = (targetRow == -1) ? totalRow : targetRow;
    if (isSameParent && newRow > totalRow - 1) {
        newRow--;  // 同一父节点下向后移动时的位置修正
    }

    beginMoveRows(draggedParentNode->getModelIndex(), draggedNode->getRow(), draggedNode->getRow(),
        targetParentIndex, newRow);
    draggedParentNode->removeChildNode(draggedNode);
    targetParentNode->insertChildNode(newRow, draggedNode);

    // 递归更新深度
    std::function<void(NXNavigationNode*, int)> recursiveUpdateDepth =
        [&recursiveUpdateDepth](NXNavigationNode* node, int baseDepth) {
        node->setDepth(baseDepth + 1);
        for (auto child : node->getChildrenNodes()) {
            recursiveUpdateDepth(child, baseDepth + 1);
        }
        };
    recursiveUpdateDepth(draggedNode, targetParentNode->getDepth());

    endMoveRows();

    return true;
}
