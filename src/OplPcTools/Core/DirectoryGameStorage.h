/***********************************************************************************************
 *                                                                                             *
 * This file is part of the OPL PC Tools project, the graphical PC tools for Open PS2 Loader.  *
 *                                                                                             *
 * OPL PC Tools is free software: you can redistribute it and/or modify it under the terms of  *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * OPL PC Tools is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;  *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __OPLPCTOOLS_DIRECTORYGAMESTORAGE__
#define __OPLPCTOOLS_DIRECTORYGAMESTORAGE__

#include <QVector>
#include <OplPcTools/Core/GameStorage.h>

namespace OplPcTools {
namespace Core {

class DirectoryGameStorage final : public GameStorage
{
    Q_OBJECT

public:
    explicit DirectoryGameStorage(QObject * _parent = nullptr);
    GameInstallationType installationType() const override;
    bool load(const QDir & _directory) override;
    bool renameGame(const QString & _id, const QString & _title) override;
    bool renameGame(const int _index, const QString & _title) override;
    bool registerGame(const Game & _game) override;

public:
    static const QString cd_directory;
    static const QString dvd_directory;

private:
    void load(QDir _base_directory, MediaType _media_type);
    bool renameGame(Game & _game, const QString & _title);
    bool renameGameFiles(const QString & _id, const QString & _title);
};

} // namespace Core
} // namespace OplPcTools

#endif // __OPLPCTOOLS_DIRECTORYGAMESTORAGE__
