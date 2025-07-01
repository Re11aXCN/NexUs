#ifndef NXSUGGESTMODEL_H
#define NXSUGGESTMODEL_H

#include <QAbstractListModel>

#include "NXDef.h"
class NXSuggestion;
class NXSuggestModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit NXSuggestModel(QObject* parent = nullptr);
    ~NXSuggestModel();
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    void setSearchSuggestion(QVector<NXSuggestion*> suggestionVector);
    void clearSearchNode();
    NXSuggestion* getSearchSuggestion(int row);

private:
    QVector<NXSuggestion*> _suggestionVector; //缁楋箑鎮庨幖婊呭偍閻ㄥ嫯濡悙?
};

#endif // NXSUGGESTMODEL_H
