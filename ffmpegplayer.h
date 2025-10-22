#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

#include <QThread>

class FFMpegPlayer : public QThread
{
    Q_OBJECT
public:
    explicit FFMpegPlayer(QObject *parent = nullptr);
    void setUrl(const QString&url,bool autoPlay=true);
    void play() ;
    void pause();
    void close();
protected:
    void run() override;
private:
    QString m_strUrl ;
    void doLoop();
    bool m_bClose = false ;
    bool m_bPlay = false ;
};

#endif // FFMPEGPLAYER_H
