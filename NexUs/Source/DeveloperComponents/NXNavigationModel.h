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
    NXNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, NXIconType::IconName awesome);
    NXNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, NXIconType::IconName awesome);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, NXIconType::IconName awesome);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, NXIconType::IconName awesome);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, int keyPoints, NXIconType::IconName awesome);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, int keyPoints, NXIconType::IconName awesome);
    QStringList removeNavigationNode(QString nodeKey);

    NXNavigationNode* getNavigationNode(QString nodeKey) const;
    QList<NXNavigationNode*> getRootExpanderNodes() const;
    QList<NXNavigationNode*> getRootExpandedNodes() const;

private:
    QMap<QString, NXNavigationNode*> _nodesMap;
    NXNavigationNode* _rootNode{nullptr};
};

#endif // NXNAVIGATIONMODEL_H
