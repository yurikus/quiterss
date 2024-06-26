/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2013-2014  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "qzregexp.h"

QzRegExp::QzRegExp()
    : QRegularExpression(QString(), QRegularExpression::DotMatchesEverythingOption)
    , m_matchedLength(-1)
{
}

QzRegExp::QzRegExp(const QString &pattern, Qt::CaseSensitivity cs)
    : QRegularExpression(pattern, QRegularExpression::DotMatchesEverythingOption)
    , m_matchedLength(-1)
{
    if (cs == Qt::CaseInsensitive) {
        setPatternOptions(patternOptions() | QRegularExpression::CaseInsensitiveOption);
    }
}

const QzRegExp& QzRegExp::operator=(const QzRegExp &re)
{
    QRegularExpression::operator=(re);
    m_capturedTexts = QStringList();
    m_matchedLength = -1;

    return *this;
}

void QzRegExp::setMinimal(bool minimal)
{
    QRegularExpression::PatternOptions opt;

    if (minimal) {
        opt = patternOptions() | QRegularExpression::InvertedGreedinessOption;
    }
    else {
        opt = patternOptions() & ~QRegularExpression::InvertedGreedinessOption;
    }

    setPatternOptions(opt);
}

int QzRegExp::indexIn(const QString &str, int offset) const
{
    QzRegExp* that = const_cast<QzRegExp*>(this);
    QRegularExpressionMatch m = match(str, offset);

    if (!m.hasMatch()) {
        that->m_matchedLength = -1;
        that->m_capturedTexts.clear();
        return -1;
    }

    that->m_matchedLength = m.capturedLength();
    that->m_capturedTexts = m.capturedTexts();
    return m.capturedStart();
}

int QzRegExp::matchedLength() const
{
    return m_matchedLength;
}

QString QzRegExp::cap(int nth) const
{
    if (!(nth >= 0 && m_capturedTexts.count() > nth)) {
        return QString();
    }

    return m_capturedTexts.at(nth);
}

