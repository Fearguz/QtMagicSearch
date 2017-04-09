#ifndef URLS_H
#define URLS_H

#include <QString>
#include <QStandardPaths>

namespace UrlConstants
{

static constexpr char MtgDbUrl[] =  "http://api.mtgdb.info";
static constexpr char GetAllCardsUrl[] = "http://api.mtgdb.info/cards/";
static constexpr char MtgDbSearch[] = "http://api.mtgdb.info/search/";
static constexpr char LowResImageUrl[] = "http://api.mtgdb.info/content/card_images/";
static constexpr char HighResImageUrl[] = "http://api.mtgdb.info/content/hi_res_card_images/";

}

namespace DirConstants
{

static const QString ImageDir {QStandardPaths::writableLocation(QStandardPaths::StandardLocation::TempLocation) + "/images/"};

}

#endif // URLS_H

