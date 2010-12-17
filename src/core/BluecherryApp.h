#ifndef BLUECHERRYAPP_H
#define BLUECHERRYAPP_H

#include <QObject>
#include <QList>
#include <QIcon>

class DVRServer;
class QNetworkAccessManager;
class MainWindow;
class QNetworkReply;
class QSslError;
class QSslConfiguration;

class BluecherryApp : public QObject
{
    Q_OBJECT

public:
    QNetworkAccessManager * const nam;
    MainWindow *mainWindow;
    QIcon appIcon;

    explicit BluecherryApp();

    QList<DVRServer*> servers() const { return m_servers; }
    DVRServer *addNewServer(const QString &name);
    DVRServer *findServerID(int id);
    bool serverExists(DVRServer *server) { return m_servers.contains(server); }

    /* Temporarily pause live feeds to free up bandwidth for other intensive transfers
     * (particularly event video buffering). The live feed can be paused with pauseLive(),
     * and released releaseLive(). Upon release, the queryLivePaused() signal will be emitted,
     * and anything still requiring live feeds to be paused should call pauseLive() again. */
    bool livePaused() const { return m_livePaused; }

public slots:
    void pauseLive();
    void releaseLive();

signals:
    void serverAdded(DVRServer *server);
    void serverRemoved(DVRServer *server);

    void sslConfirmRequired(DVRServer *server, const QList<QSslError> &errors, const QSslConfiguration &config);

    void queryLivePaused();
    void livePausedChanged(bool paused);

private slots:
    void onServerRemoved(DVRServer *server);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    QList<DVRServer*> m_servers;
    int m_maxServerId;
    bool m_livePaused, m_inPauseQuery;

    void loadServers();
};

extern BluecherryApp *bcApp;

#endif // BLUECHERRYAPP_H
