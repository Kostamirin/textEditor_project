#include "tabs_button.h"


Tabs_button::Tabs_button(const QString &m_title)
    : QObject(nullptr)
    , title(m_title)
{

}

Tabs_button::~Tabs_button()
{

}

//класс для отдельных вкладок
// тут будет хранится текст состояние файла (сохранен, не сохранен)
//