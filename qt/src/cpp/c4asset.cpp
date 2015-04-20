#include "c4asset.h"
#include <QCryptographicHash>
#include <QFile>
#include <QMimeDatabase>

#include <QDebug>

namespace c4
{

Asset::Asset(QObject *parent) : QObject(parent) {}

QString Asset::address()
{
  return _address;
}

QString Asset::c4id()
{
  return _c4id;
}

QString Asset::type()
{
  return _type;
}

void Asset::setAddress(const QString& address)
{
  if(_address != address)
  {
    _address = address;
    emit addressChanged();
  }
}

void Asset::setC4id(const QString& c4id)
{
  if(_c4id != c4id)
  {
    _c4id = c4id;
    emit c4idChanged();
  }
}

void Asset::setType(const QString& type)
{
  if(_type != type)
  {
    _type = type;
    emit typeChanged();
  }
}

void Asset::id(const QUrl& fileUrl)
{
  QCryptographicHash hasher(QCryptographicHash::Sha512);

  QFile file(fileUrl.toLocalFile());
  file.open(QIODevice::ReadOnly);
  hasher.addData(&file);
  QMimeDatabase mdb;

  setAddress(QString(hasher.result().toBase64()));
  setC4id("c412345");  // TODO: add base58 and check digit encoding/decoding.
  setType(mdb.mimeTypeForUrl(fileUrl).name());
}


}

