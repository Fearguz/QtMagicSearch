#include "card.h"

QString Card::toString() const noexcept
{
    QString str;
    str.append(Name);
    str.append('\n');
    str.append('\n');
    str.append(Type);
    str.append('\n');
    str.append(Description);
    return str;
}
