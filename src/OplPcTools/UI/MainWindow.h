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

#ifndef __OPLPCTOOLS_MAINWINDOW__
#define __OPLPCTOOLS_MAINWINDOW__

#include <QMainWindow>
#include <OplPcTools/UI/UIContext.h>

namespace OplPcTools {
namespace UI {

class MainWindow : public QMainWindow, public UIContext
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * _parent = nullptr);
    ~MainWindow() override;
    Core::GameCollection & collection() const override;

public slots:
    void showGameInstaller() override;
    void showIsoRecoverer() override;
    void showGameDetails(const QString & _id) override;

private:
    class MainWindowUI;
    MainWindowUI * mp_ui;
    Core::GameCollection * mp_collection;
};

} // namespace UI
} // namespace OplPcTools

#endif // __OPLPCTOOLS_MAINWINDOW__