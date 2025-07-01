#ifndef NXSUGGESTBOX_H
#define NXSUGGESTBOX_H

#include <QVariantMap>
#include <QWidget>

#include "NXDef.h"

class NXSuggestBoxPrivate;
class NX_EXPORT NXSuggestBox : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXSuggestBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit NXSuggestBox(QWidget* parent = nullptr);
    ~NXSuggestBox();
    void setPlaceholderText(const QString& placeholderText);

    QString addSuggestion(const QString& suggestText, const QVariantMap& suggestData = {});
    QString addSuggestion(NXIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData = {});
    void removeSuggestion(const QString& suggestKey);
    void removeSuggestion(int index);
Q_SIGNALS:
    Q_SIGNAL void suggestionClicked(const QString& suggestText, QVariantMap suggestData);
};

#endif // NXSUGGESTBOX_H
