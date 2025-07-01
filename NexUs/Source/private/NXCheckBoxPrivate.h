#ifndef NXMENUPRIVATE_H
#define NXMENUPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "NXDef.h"
class NXCheckBox;
class NXCheckBoxStyle;
class NXCheckBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXCheckBox)
public:
    explicit NXCheckBoxPrivate(QObject* parent = nullptr);
    ~NXCheckBoxPrivate();

private:
    std::shared_ptr<NXCheckBoxStyle> _checkStyle{ nullptr };
    NXTextType::TextStyle _textStyle{ NXTextType::NoStyle };
};

#endif // NXMENUPRIVATE_H
