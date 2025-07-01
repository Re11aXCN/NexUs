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
    Qt::ItemFlags flags(const QModelIndex& index) const override;
 
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, const QString& targetExpanderKey, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, int keyPoints, NXIconType::IconName awesome);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, const QString& targetExpanderKey, int keyPoints, NXIconType::IconName awesome);
    QStringList removeNavigationNode(const QString& nodeKey);

    const NXNavigationNode* getRootNode() const;
    NXNavigationNode* getNavigationNode(const QString& nodeKey) const;
    QList<NXNavigationNode*> getRootExpanderNodes() const;
    QList<NXNavigationNode*> getRootExpandedNodes() const;

    virtual Qt::DropActions supportedDropActions() const override;
    virtual Qt::DropActions supportedDragActions() const override;
    virtual QStringList mimeTypes() const override;
    virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
    virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
    virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;

    enum DropIndicatorPosition { OnItem, AboveItem, BelowItem, OnViewport };
    void setDropIndicatorPosition(DropIndicatorPosition position);
    DropIndicatorPosition getDropIndicatorPosition() const;
Q_SIGNALS:
    void mineDataDropped(const QMimeData* data, const QModelIndex& draggedIndex, const QModelIndex& targetIndex);
private:
    QMap<QString, NXNavigationNode*> _nodesMap;
    NXNavigationNode* _rootNode{nullptr};
    DropIndicatorPosition _dropIndicatorPosition{OnViewport};
};

#endif // NXNAVIGATIONMODEL_H
