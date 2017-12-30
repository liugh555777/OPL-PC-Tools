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

#include <cstring>
#include <QTemporaryFile>
#include <OplPcTools/Core/File.h>
#include <OplPcTools/Core/UlConfigGameStorage.h>

#define MT_CD  0x12
#define MT_DVD 0x14
#define UL_CONFIG_FILENAME "ul.cfg"

using namespace OplPcTools::Core;

namespace {

const QString g_image_prefix("ul.");

struct RawConfigRecord
{
    explicit RawConfigRecord(const Game & _game);
    char name[g_max_game_name_length];
    char image[g_max_game_id_length];
    quint8 parts;
    quint8 media;
    quint8 pad[15];
};

RawConfigRecord::RawConfigRecord(const Game & _game)
{
    memset(this, 0, sizeof(RawConfigRecord));
    QByteArray name_bytes = _game.title().toUtf8();
    QByteArray image_bytes = _game.id().toLatin1();
    memcpy(this->image, g_image_prefix.toLatin1().constData(), g_image_prefix.size());
    memcpy(&this->image[g_image_prefix.size()], image_bytes.constData(), image_bytes.size());
    memcpy(this->name , name_bytes.constData(), name_bytes.size());
    this->media = _game.mediaType() == MediaType::DVD ? MT_DVD : MT_CD;
    this->parts = _game.partCount();
    this->pad[4] = 0x08; // To be like USBA
}

size_t findRecordOffset(QFile & _file, const QString & _id, RawConfigRecord * _result = nullptr)
{
    const QByteArray image_qbytes = (g_image_prefix + _id).toLatin1();
    const char * image_bytes = image_qbytes.constData();
    char record_bytes[sizeof(RawConfigRecord)];
    for(size_t offset = 0; ;)
    {
        quint64 read = _file.read(record_bytes, sizeof(record_bytes));
        if(read < sizeof(record_bytes))
            break;
        RawConfigRecord * record = reinterpret_cast<RawConfigRecord *>(record_bytes);
        if(strncmp(image_bytes, record->image, image_qbytes.length()) == 0)
        {
            if(_result)
                memcpy(_result, &record, sizeof(RawConfigRecord));
            return offset;
        }
        offset += sizeof(RawConfigRecord);
    }
    return ~0;
}

} // namespace

UlConfigGameStorage::UlConfigGameStorage(QObject * _parent /*= nullptr*/) :
    GameStorage(_parent)
{
}

UlConfigGameStorage::~UlConfigGameStorage()
{
}

GameInstallationType UlConfigGameStorage::installationType() const
{
    return GameInstallationType::UlConfig;
}

bool UlConfigGameStorage::load(const QDir & _directory)
{
    clear();
    m_config_filepath = _directory.absoluteFilePath(UL_CONFIG_FILENAME);
    QFile file(m_config_filepath);
    openFile(file, QIODevice::ReadWrite);
    const size_t record_size = sizeof(RawConfigRecord);
    char * buffer = new char[record_size];
    for(;;)
    {
        size_t read_bytes = file.read(buffer, record_size);
        if(read_bytes < record_size)
            break;
        RawConfigRecord * raw_record = reinterpret_cast<RawConfigRecord *>(buffer);
        Game * game = createGame(QString::fromLatin1(&raw_record->image[g_image_prefix.size()],
            strlen(raw_record->image) - g_image_prefix.size()));
        if(raw_record->name[g_max_game_name_length - 1] == '\0')
            game->setTitle(QString::fromUtf8(raw_record->name, strlen(raw_record->name)));
        else
            game->setTitle(QString::fromUtf8(raw_record->name, g_max_game_name_length));
        game->setPartCount(raw_record->parts);
        switch(raw_record->media)
        {
        case MT_CD:
            game->setMediaType(MediaType::CD);
            break;
        case MT_DVD:
            game->setMediaType(MediaType::DVD);
            break;
        default:
            game->setMediaType(MediaType::Unknown);
            break;
        }
    }
    delete [] buffer;
}

bool UlConfigGameStorage::renameGame(const QString & _id, const QString & _title)
{
    // TODO: exception
    Game * game = findNonConstGame(_id);
    return game && renameGame(*game, _title);
}

bool UlConfigGameStorage::renameGame(const int _index, const QString & _title)
{
    // TODO: exception
    return renameGame(*gameAt(_index), _title);
}

bool UlConfigGameStorage::renameGame(Game & _game, const QString & _title)
{
    if(renameGameInConfig(_game.id(), _title))
    {
        _game.setTitle(_title);
        return true;
    }
    return false;
}

bool UlConfigGameStorage::renameGameInConfig(const QString & _id, const QString & _title)
{
    // TODO: update ul.conf
    return true;
}

bool UlConfigGameStorage::registerGame(const Game & _game)
{
    // TODO: register game
    return true;
}
