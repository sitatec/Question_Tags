#pragma once

#include <QtNetwork>
#include <QtWidgets>

class Update: public QWidget
{
    Q_OBJECT
public:
    Update( QWidget* parrent = nullptr );
    void checkForUpdate();

signals:
    void newVersionAvailable();
    void softwareUpToDate();

public slots:
    void updateProgessBar( qint64, qint64 );
    void suggestUpdate();
    void updateApp( QNetworkReply* );
    void readData( QNetworkReply* );
    void softwareUpToDateSlot();

private:
    QNetworkAccessManager *_networkManager;
    QVector <QString>      _latestVersionFeatures;
    QUrl                   _urlOfLatestVersionInfos;
    QVersionNumber         _latestVersion,
                           _currentVersion;
    QDialog                _downloadProgress;
    QProgressBar           _progressBar;
    QLabel                 _downloding;
    QStringList            _latestVersionInfos;
    QString                _urlOfLatestVersion;

    void storeLatestVersionInfos();
    void getLatestVersionInfos();
    void downloadLatestVersion();
    void compareVersions();
    void suggestRestart();
    void lunchUpdater();
    void showProgress();
};

