#ifndef NXFRAMEWORK_NXWIDGETTOOLS_DEVELOPERCOMPONENTS_NXDRAWERHEADER_H_
#define NXFRAMEWORK_NXWIDGETTOOLS_DEVELOPERCOMPONENTS_NXDRAWERHEADER_H_

#include "NXDef.h"
#include <QVBoxLayout>
#include <QWidget>
class NXDrawerHeader : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, BorderRadius)
    Q_PRIVATE_CREATE(bool, IsExpand)
    Q_PROPERTY_CREATE(qreal, ExpandIconRotate)
public:
    explicit NXDrawerHeader(QWidget* parent = nullptr);
    ~NXDrawerHeader() override;

    void setHeaderWidget(QWidget* widget);
    void doExpandOrCollpaseAnimation();
Q_SIGNALS:
    Q_SIGNAL void drawerHeaderClicked(bool isExpand);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    NXThemeType::ThemeMode _themeMode;
    bool _isPressed{false};
    QWidget* _headerWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
};

#endif //NXFRAMEWORK_NXWIDGETTOOLS_DEVELOPERCOMPONENTS_NXDRAWERHEADER_H_
