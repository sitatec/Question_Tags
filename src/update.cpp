#include "include/update.hpp"
#include <QMessageBox>


static const char* TEXT_OF_UPDATE_FAIL       = "An error has been occured when updating software.<br>You can try"
                                               "again by clicking on Update in the left menu of the software";

static const char* URL_OF_UPDATE_INFOS       = "https://custauto.com/question_tags/update.php?password=sljfdijlmq45398"
                                               "jtlz469uzj9RZuF%C3%A7uzSFu%C3%A7fsOIUutsfF9EuZFUS9fjs&secure=sjlfLJSFJI"
                                               "S464Zfdsjsi64ZT4fjslj634Z_rjsdlkf63zrjie&appName=question_tags";

static const char* TEXT_OF_UPDATE_SUGGESTION = "New version available :<ul> <li>Version %1 .</li>"
                                               "<li>Features :<ul>%2 </ul></li></ul><br> Download the new version ?";

static const char* RESTART_SUGGESTION_TEXT   = "Update done. You need restart the software to use new features."
                                               "<br>Restart now ?";




Update::Update( QWidget* parrent ): QWidget( parrent )
{
    _currentVersion = QVersionNumber( VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    _urlOfLatestVersionInfos = QUrl( URL_OF_UPDATE_INFOS );

    connect(this, SIGNAL( newVersionAvailable() ), this, SLOT( suggestUpdate() ));
}



void Update::checkForUpdate()
{
    getLatestVersionInfos();
}



void Update::getLatestVersionInfos()
{
    _networkManager = new QNetworkAccessManager( this );
    connect( _networkManager, SIGNAL( finished( QNetworkReply* ) ),
                        this,   SLOT( readData( QNetworkReply* ) ));
    _networkManager->get( QNetworkRequest( _urlOfLatestVersionInfos ));
}



void Update::readData( QNetworkReply* reply )
{
    if( !reply->error() ){
        QString data = reply->readAll();
        // latest version informations is separated by hyphens(-)
        _latestVersionInfos = data.split("-");
        storeLatestVersionInfos();
    }
    reply->deleteLater();
    compareVersions();
}



void Update::storeLatestVersionInfos()
{
    _latestVersion = QVersionNumber::fromString( _latestVersionInfos[ 0 ] );
    _urlOfLatestVersion = _latestVersionInfos[ 1 ];
    // Features Infos of latest version start at index 2
    for(int i = 2, j = _latestVersionInfos.size(); i < j; i++){
        _latestVersionFeatures.push_back( _latestVersionInfos[ i ] );
    }
}



void Update::compareVersions()
{
    if( _latestVersion > _currentVersion )
        emit newVersionAvailable();
    else
        emit softwareUpToDate();
}



void Update::suggestUpdate()
{
    QString latestVersionFeatures;
    foreach( auto feature, _latestVersionFeatures ){
          latestVersionFeatures.append("<li>"+ feature +"</li>");
    }
    auto choice = QMessageBox::question( this, tr("Update Question Tages"), tr( TEXT_OF_UPDATE_SUGGESTION ).
                                         arg( _latestVersion.toString(), latestVersionFeatures ),
                                         QMessageBox::No | QMessageBox::Yes , QMessageBox::Yes );
    if( choice == QMessageBox::Yes )
        downloadLatestVersion();
}



void Update::downloadLatestVersion()
{
    showProgress();
    _networkManager = new QNetworkAccessManager( this );
    connect( _networkManager, SIGNAL(  finished( QNetworkReply* ) ),
                        this,   SLOT( updateApp( QNetworkReply* ) ));

    QNetworkReply *reply = _networkManager->get( QNetworkRequest( _urlOfLatestVersion ) );
    connect( reply, SIGNAL( downloadProgress( qint64, qint64 )),
              this,   SLOT( updateProgessBar( qint64, qint64 )) );

}



void Update::updateProgessBar( qint64 bytesReceived, qint64 bytesTotal )
{
    if( bytesTotal != -1 )
        _progressBar.setValue( (bytesReceived * 100) / bytesTotal );
}



void Update::showProgress()
{
    QVBoxLayout *layout = new QVBoxLayout;
    _downloding.setText("Downloding...");
    _downloding.setFixedWidth( 200 );

    _progressBar.setRange( 0,100 );
    _progressBar.setFixedWidth( 200 );

    layout->addWidget( &_downloding , 0,  Qt::AlignTop );
    layout->addWidget( &_progressBar, 0, Qt::AlignBottom);

    _downloadProgress.setParent( this );
    _downloadProgress.setLayout( layout );
    _downloadProgress.open();
}



void Update::updateApp( QNetworkReply* reply )
{
    _downloding.setText("Downloading... done.<br>Updating...");

    QFile appToUpdate( qApp->applicationFilePath() );
    if( !reply->error() && appToUpdate.open( QIODevice::WriteOnly )){
       QByteArray compressedData = reply->readAll();
       appToUpdate.write( qUncompress( compressedData ) );
       reply->deleteLater();
       suggestRestart();
    }
    else
        QMessageBox::critical( this, tr("Update Fail"), tr( TEXT_OF_UPDATE_FAIL ) );
    _downloadProgress.close();
}



void Update::suggestRestart()
{
    auto response = QMessageBox::question( this, tr("Restart"), tr( RESTART_SUGGESTION_TEXT ),
                                           QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if( response == QMessageBox::Yes ){
        QProcess::startDetached( qApp->applicationFilePath() );
        QTimer::singleShot( 250, qApp, SLOT( quit() ) );
    }
}



void Update::softwareUpToDateSlot()
{
    QMessageBox::information(this, tr("Update"), tr("Question Tags is already up to date!"));
}
