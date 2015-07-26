#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QStringList>

class Card
{
public:
    Card() = default;

    int Id;
    int SetNumber;
    QString Name;
    QString Description;
    QStringList Colors;
    QString ManaCost;
    QString Type;
    QString SubType;
    int Power;
    int Toughness;

    QString asString()
    {
        QString ret;
        ret += Name + "\n\n";
        ret += Type + "\n";
        ret += Description;
        return ret;
    }
};

Q_DECLARE_METATYPE(Card *)

#endif // CARD_H

