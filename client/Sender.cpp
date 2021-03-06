#include "Sender.hh"

#include <QSharedPointer>
#include <QVector>
#include <QtTest/QTest>

#include "Debug.hh"

Sender::Sender(const QString &host, quint16 port, QObject *parent)
    : QObject(parent),
      m_host(host),
      m_port(port),
      m_socket(new BaseSocket(BaseSocket::BROADCAST, this))
{
    init();
}

Sender::Sender(QObject *parent)
    : QObject(parent),
      m_host(""),
      m_port(0),
      m_socket(new BaseSocket(BaseSocket::BROADCAST, this))
{
    init();
}

void Sender::init()
{
    DEBUG("Sender::Sender()", false);

    connect(m_socket,   SIGNAL(message(qint32, const QVariant&)),
            this,       SLOT(messageReceived(qint32, const QVariant&)));
    connect(m_socket,   SIGNAL(error(QAbstractSocket::SocketError)),
            this,       SLOT(socketError(QAbstractSocket::SocketError)));
}

Sender::~Sender()
{
    DEBUG("Sender::~Sender()", true);
}

bool Sender::start()
{
    if (m_host == "" || m_port == 0)
    {
        DEBUG("Host and port must be set", true);
        return false;
    }

    if ( ! m_socket->isValid())
    {
        m_socket->connectToHost(m_host, m_port);
        if ( ! m_socket->isOpen())
        {
            DEBUG("Sender::start() : Unable to connect to" << m_host << m_port << " :" << m_socket->errorString(), true);
            return false;
        }
        else
        {
            DEBUG("Sender::start() : Connected to " << m_host << ":" << m_port, true);
//            m_timer.start(1000);
            return true;
        }
    }

    return false;
}

bool Sender::start(const QString &host, quint16 port)
{
    m_host = host;
    m_port = port;

    return start();
}

void Sender::stop()
{
    if (m_socket->isOpen())
    {
        DEBUG("Sender::stop() : Stop connection", true);

        m_socket->resetSocket();
        m_socket->abort();
    }
    else
    {
        DEBUG("Sender::stop() : Error, not connected", true);
    }
}

void Sender::send(const QVariant & data)
{
    DEBUG("Sender::send() : Send" << data, false);

    m_socket->send(BaseSocket::BROADCAST, data);
}

const BaseSocket *Sender::socket() const
{
    return m_socket;
}

void Sender::hostname(const QString & host)
{
    m_host = host;
}

void Sender::port(quint16 port)
{
    m_port = port;
}

void Sender::messageReceived(qint32 socketFd, const QVariant & msg)
{
    DEBUG("Sender::receiveMessage() :" << msg, true);

    emit transfertMessage(socketFd, msg);
}

void Sender::socketError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            break;

        case QAbstractSocket::HostNotFoundError:
            break;

        case QAbstractSocket::ConnectionRefusedError:
            break;

        default:
            break;
    }
}
















