#ifndef URLS_H
#define URLS_H

#include <QString>
#include <QStandardPaths>

const QString mtgDbUrl =  "http://api.mtgdb.info";
const QString getAllCardsUrl = mtgDbUrl + "/cards/";
const QString mtgDbSearch = mtgDbUrl + "/search/";
const QString lowResImageUrl = "http://api.mtgdb.info/content/card_images/";
const QString highResImageUrl = "http://api.mtgdb.info/content/hi_res_card_images/";

const QString imageDir = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::TempLocation) + "/images/";

#endif // URLS_H

