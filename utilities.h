#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <QStringList>

namespace Utilities {
    // Simple RLE encoding for level data
    QString encodeRLE(const QString& data) {
        QString result;
        int count = 1;
        char current = data[0].toLatin1();
        
        for (int i = 1; i < data.length(); i++) {
            if (data[i].toLatin1() == current && count < 9) {
                count++;
            } else {
                result += QString::number(count) + current;
                current = data[i].toLatin1();
                count = 1;
            }
        }
        result += QString::number(count) + current;
        return result;
    }

    // Simple encryption (XOR with a key)
    QString encrypt(const QString& data, const QString& key) {
        QString result;
        for (int i = 0; i < data.length(); i++) {
            result += QChar(data[i].toLatin1() ^ key[i % key.length()].toLatin1());
        }
        return result;
    }
}

#endif // UTILITIES_H
