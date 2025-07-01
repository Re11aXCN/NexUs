#ifndef NXNAVIGATIONMODEL_H
#define NXNAVIGATIONMODEL_H

#include <QAbstractItemModel>
#include <QObject>

#include "NXDef.h"
class NXNavigationNode;
class NXNavigationModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PRIVATE_CREATE(NXNavigationNode*, SelectedNode)
    Q_PRIVATE_CREATE(NXNavigationNode*, SelectedExpandedNode)

public:
    explicit NXNavigationModel(QObject* parent = nullptr);
    ~NXNavigationModel();
    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, const QString& targetExpanderKey, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, int keyPoints, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, const QString& targetExpanderKey, int keyPoints, NXIconType::IconName awesome);
    QStringList removeNavigationNode(const QString& nodeKey);

    NXNavigationNode* getNavigationNode(const QString& nodeKey) const;
    QList<NXNavigationNode*> getRootExpanderNodes() const;
    QList<NXNavigationNode*> getRootExpandedNodes() const;

private:
    QMap<QString, NXNavigationNode*> _nodesMap;
    NXNavigationNode* _rootNode{nullptr};
};

#endif // NXNAVIGATIONMODEL_H
