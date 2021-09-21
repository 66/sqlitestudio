#include "style.h"
#include "themetuner.h"
#include "common/global.h"
#include "mainwindow.h"
#include <QApplication>
#include <QToolTip>

Style* Style::instance = nullptr;

Style* Style::getInstance()
{
    if (instance == nullptr)
        instance = new Style(QApplication::style());

    return instance;
}

const ExtendedPalette& Style::extendedPalette() const
{
    return extPalette;
}

void Style::setStyle(QStyle *style, const QString &styleName)
{
    setBaseStyle(style);

    QApplication::setPalette(initialPalette); // reset palette, cause styles don't provide
                                              // full palette when changed in runtime (i.e. windowsvista)
    QApplication::setStyle(this);
    QApplication::setPalette(standardPalette());
    THEME_TUNER->tuneTheme(styleName);
    QToolTip::setPalette(standardPalette());

    extPalette.styleChanged(this, styleName);

    MAINWINDOW->getMdiArea()->setBackground(extPalette.mdiAreaBase());
}

QString Style::name() const
{
    return baseStyle()->objectName();
}

bool Style::isDark() const
{
    return standardPalette().text().color().value() >= 80;
}

Style::Style(QStyle *style)
    : QProxyStyle(style)
{
    initialPalette = style->standardPalette();
}
