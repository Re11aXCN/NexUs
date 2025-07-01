#include "NXFooterModel.h"

#include <QUuid>

#include "NXNavigationNode.h"
NXFooterModel::NXFooterModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _pSelectedNode = nullptr;
}

NXFooterModel::~NXFooterModel()
{
    qDeleteAll(_footerNodeList);
}

int NXFooterModel::rowCount(const QModelIndex& parent) const
{
    return _footerNodeList.count();
}

QVariant NXFooterModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        if (index.row() < _footerNodeList.count())
        {
            return QVariant::fromValue(_footerNodeList[index.row()]);
        }
    }
    return QVariant();
}

NodeOperateReturnTypeWithKey NXFooterModel::addFooterNode(const QString& footerTitle, bool isHasFooterPage, int keyPoints, NXIconType::IconName awesome)
{
    if (_footerNodeList.count() >= 3)
    {
        return qMakePair(NXNavigationType::FooterUpperLimit, QString{});
    }
    NXNavigationNode* node = new NXNavigationNode(footerTitle);
    node->setKeyPoints(keyPoints);
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setAwesome(awesome);
    beginResetModel();
    _footerNodeList.append(node);
    endResetModel();
    node->setModelIndex(this->index(_footerNodeList.count() - 1));
    return qMakePair(NXNavigationType::Success, node->getNodeKey());
}

int NXFooterModel::getFooterNodeCount() const
{
    return _footerNodeList.count();
}

NXNavigationNode* NXFooterModel::getNavigationNode(const QString& footerKey)
{
    for (auto node : _footerNodeList)
    {
        if (node->getNodeKey() == footerKey)
        {
            return node;
        }
    }
    return nullptr;
}

void NXFooterModel::removeNavigationNode(const QString& footerKey)
{
    for (auto node : _footerNodeList)
    {
        if (node->getNodeKey() == footerKey)
        {
            _footerNodeList.removeOne(node);
            break;
        }
    }
}
