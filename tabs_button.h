#ifndef TABS_BUTTON_H
#define TABS_BUTTON_H

#include <QTextEdit>
#include <QString>

class Tabs_button : QObject
{

public:
    explicit Tabs_button(const QString &m_title);
    ~Tabs_button();


    QTextEdit text;
    const QString title;

private:

};

#endif // TABS_BUTTON_H
