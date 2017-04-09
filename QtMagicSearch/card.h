#ifndef CARD_H
#define CARD_H

#include <QMetaType>
#include <QString>
#include <QStringList>

struct Card
{
    int32_t Id;
    int32_t SetNumber;
    QString Name;
    QString Description;
    QStringList Colors;
    QString ManaCost;
    QString Type;
    QString SubType;
    int8_t Power;
    int8_t Toughness;

    QString toString() const noexcept;
};

Q_DECLARE_METATYPE(Card*)

#endif // CARD_H

