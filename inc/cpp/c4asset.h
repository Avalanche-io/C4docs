#ifndef C4ASSET_H
#define C4ASSET_H

#include <QObject>
#include <QString>
#include <QUrl>

namespace c4
{
  class Asset : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString c4id READ c4id WRITE setC4id NOTIFY c4idChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)

    QString _address;
    QString _c4id;
    QString _type;

  public:

    explicit Asset(QObject *parent = 0);

    Q_INVOKABLE void id(const QUrl& fileUrl);

    QString address();
    QString c4id();
    QString type();

  public slots:

    void setAddress(const QString& address);
    void setC4id(const QString& c4id);
    void setType(const QString& type);

  signals:

    void addressChanged();
    void c4idChanged();
    void typeChanged();

  };
}

#endif // C4ASSET_H
