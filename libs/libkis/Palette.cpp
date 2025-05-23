/*
 *  SPDX-FileCopyrightText: 2017 Wolthera van Hövell tot Westerflier <griffinvalley@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "Palette.h"
#include <KoColorSet.h>
#include <KisSwatch.h>
#include <KisSwatchGroup.h>
#include <ManagedColor.h>
#include <KisPaletteModel.h>

struct Palette::Private {
    KoColorSetSP palette {0};
};

Palette::Palette(Resource *resource, QObject *parent)
       : QObject(parent)
       , d(new Private()) {
    d->palette = resource->resource().dynamicCast<KoColorSet>();
}

Palette::~Palette()
{
    delete d;
}

bool Palette::operator==(const Palette &other) const
{
    return (d->palette == other.d->palette);
}

bool Palette::operator!=(const Palette &other) const
{
    return !(operator==(other));
}


int Palette::numberOfEntries() const
{
    if (!d->palette) return 0;
    return d->palette->colorCount();
}

int Palette::columnCount()
{
    if (!d->palette) return 0;
    return d->palette->columnCount();
}

void Palette::setColumnCount(int columns)
{
    if (d->palette && columns > 0)
        d->palette->setColumnCount(columns);
}

int Palette::rowCount()
{
    if (!d->palette) return 0;
    return d->palette->rowCount();
}

int Palette::rowCountGroup(QString name)
{
    if (!d->palette) return 0;
    KisSwatchGroupSP group = d->palette->getGroup(name);
    if (!group) return 0;
    return group->rowCount();
}

void Palette::setRowCountGroup(int rows, QString name)
{
    if (!d->palette) return;
    KisSwatchGroupSP group = d->palette->getGroup(name);
    if (!group) return;
    if (rows > 0)
        group->setRowCount(rows);
}

QString Palette::comment()
{
    if (!d->palette) return "";
    return d->palette->comment();
}

void Palette::setComment(QString comment)
{
    if (!d->palette) return;
    return d->palette->setComment(comment);
}

QStringList Palette::groupNames() const
{
    if (!d->palette) return QStringList();
    return d->palette->swatchGroupNames();
}

void Palette::addGroup(QString name)
{
    if (!d->palette) return;
    d->palette->addGroup(name);
}

void Palette::removeGroup(QString name, bool keepColors)
{
    if (!d->palette) return;
    return d->palette->removeGroup(name, keepColors);
}

int Palette::colorsCountTotal()
{
    if (!d->palette) return 0;
    return d->palette->colorCount();
}

int Palette::colorsCountGroup(QString name)
{
    if (!d->palette) return 0;
    KisSwatchGroupSP group = d->palette->getGroup(name);
    if (!group) return 0;
    return group->colorCount();
}

int Palette::slotCount()
{
    if (!d->palette) return 0;
    return d->palette->slotCount();
}

int Palette::slotCountGroup(QString name)
{
    if (!d->palette) return 0;
    KisSwatchGroupSP group = d->palette->getGroup(name);
    if (!group) return 0;
    return group->slotCount();
}

Swatch *Palette::colorSetEntryByIndex(int index)
{
    warnScript << "DEPRECATED Palette.colorSetEntryByIndex() - use Palette.entryByIndex() instead";
    return entryByIndex(index);
}

Swatch *Palette::entryByIndex(int index)
{
    if (!d->palette || columnCount() == 0) {
        return new Swatch();
    }
    int col = index % columnCount();
    int row = (index - col) / columnCount();
    return new Swatch(d->palette->getColorGlobal(col, row));
}

Swatch *Palette::colorSetEntryFromGroup(int index, const QString &groupName)
{
    warnScript << "DEPRECATED Palette.colorSetEntryFromGroup() - use Palette.entryByIndexFromGroup() instead";
    return entryByIndexFromGroup(index, groupName);
}

Swatch *Palette::entryByIndexFromGroup(int index, const QString &groupName)
{
    if (!d->palette || columnCount() == 0) {
        return new Swatch();
    }
    int col = index % columnCount();
    return new Swatch(d->palette->getSwatchFromGroup(col, (index - col) / columnCount(), groupName));
}

void Palette::addEntry(Swatch entry, QString groupName)
{
    d->palette->addSwatch(entry.kisSwatch(), groupName);
}

void Palette::removeEntry(int index)
{
    int col = index % columnCount();
    int tmp = index;
    int row = (index - col) / columnCount();
    KisSwatchGroupSP groupFoundIn;
    Q_FOREACH(const QString &name, groupNames()) {
        KisSwatchGroupSP g = d->palette->getGroup(name);
        tmp -= g->rowCount() * columnCount();
        if (tmp < 0) {
            groupFoundIn = g;
            break;
        }
        row -= g->rowCount();
    }
    if (!groupFoundIn) { return; }
    d->palette->removeSwatch(col, row, groupFoundIn);
}
void Palette::removeEntryFromGroup(int index, const QString &groupName)
{
    if (!d->palette) return;
    KisSwatchGroupSP group = d->palette->getGroup(groupName);
    if (!group) return;
    int col = index % columnCount();
    int row = (index - col) / columnCount();
    d->palette->removeSwatch(col, row, group);
}

void Palette::changeGroupName(QString oldGroupName, QString newGroupName)
{
    warnScript << "DEPRECATED Palette.changeGroupName() - use Palette.renameGroup() instead";
    return renameGroup(oldGroupName, newGroupName);
}

void Palette::renameGroup(QString oldGroupName, QString newGroupName)
{
    d->palette->changeGroupName(oldGroupName, newGroupName);
}

void Palette::moveGroup(const QString &groupName, const QString &groupNameInsertBefore)
{
    return d->palette->moveGroup(groupName, groupNameInsertBefore);
}

bool Palette::save()
{
    return false;
}

KoColorSetSP Palette::colorSet()
{
    return d->palette;
}
