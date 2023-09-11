// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZKSTAKE_QT_ZKSTAKEADDRESSVALIDATOR_H
#define ZKSTAKE_QT_ZKSTAKEADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ZKstakeAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ZKstakeAddressEntryValidator(QObject *parent, bool allowZAddresses = false);

    State validate(QString &input, int &pos) const;
private:
    bool _allowZAddresses;
};

/** ZKstake address widget validator, checks for a valid zkstake address.
 */
class ZKstakeAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ZKstakeAddressCheckValidator(QObject *parent, bool allowZAddresses = false);

    State validate(QString &input, int &pos) const;
private:
    bool _allowZAddresses;
};

#endif // ZKSTAKE_QT_ZKSTAKEADDRESSVALIDATOR_H
