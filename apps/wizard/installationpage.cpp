#include "installationpage.hpp"

#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <QMessageBox>

#include "mainwizard.hpp"
#include "inisettings.hpp"

Wizard::InstallationPage::InstallationPage(MainWizard *wizard) :
    QWizardPage(wizard),
    mWizard(wizard)
{
    setupUi(this);
}

void Wizard::InstallationPage::initializePage()
{
    QString path(field("installation.path").toString());
    QStringList components(field("installation.components").toStringList());

    logTextEdit->append(QString("Installing to %1").arg(path));
    logTextEdit->append(QString("Installing %1.").arg(components.join(", ")));

    installProgressBar->setMinimum(0);

    // Set the progressbar maximum to a multiple of 100
    // That way installing all three components would yield 300%
    // When one component is done the bar will be filled by 33%

    if (field("installation.new").toBool() == true)
    {
        installProgressBar->setMaximum((components.count() * 100));
    }
    else
    {
        if (components.contains("Tribunal") && mWizard->mInstallations[path]->hasTribunal == false)
            installProgressBar->setMaximum(100);

        if (components.contains("Bloodmoon") && mWizard->mInstallations[path]->hasBloodmoon == false)
            installProgressBar->setMaximum(installProgressBar->maximum() + 100);
    }

    installProgressBar->setValue(100);

    if (field("installation.new").toBool() == false)
        setupSettings();
}

void Wizard::InstallationPage::setupSettings()
{
    // Test settings
    IniSettings iniSettings;

    QString path(field("installation.path").toString());
    QFile file(mWizard->mInstallations[path]->iniPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error opening Morrowind configuration file"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setText(QObject::tr("<br><b>Could not open %0 for reading</b><br><br> \
                                   Please make sure you have the right permissions \
                                   and try again.<br>").arg(file.fileName()));
                                   msgBox.exec();
        mWizard->close();
        return;
    }

    QTextStream stream(&file);

    QString language(field("installation.language").toString());

    if (language == QLatin1String("Polish")) {
        stream.setCodec(QTextCodec::codecForName("windows-1250"));
    }
    else if (language == QLatin1String("Russian")) {
        stream.setCodec(QTextCodec::codecForName("windows-1251"));
    }
    else {
        stream.setCodec(QTextCodec::codecForName("windows-1252"));
    }

    iniSettings.readFile(stream);

    qDebug() << iniSettings.value("Game Files/GameFile0");
}

int Wizard::InstallationPage::nextId() const
{
    return MainWizard::Page_Import;
}
