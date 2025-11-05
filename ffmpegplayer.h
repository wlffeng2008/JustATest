#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

#include <QThread>
class AVFrame;
class FFMpegPlayer : public QThread
{
    Q_OBJECT
public:
    explicit FFMpegPlayer(QObject *parent = nullptr);
    void setUrl(const QString&url,bool autoPlay=true);
    void play() ;
    void pause();
    void close();

signals:
    void onOpen() ;
    void onClose() ;
    void onHeart();
    void onOutlog(const QString&log);
    void onSize(int width,int height);
    void onImage(const QImage&image);

protected:
    void run() override;
private:
    QString m_strUrl ;
    void doLoop();
    bool m_bClose = false ;
    bool m_bPlay = false ;

    void displayFrame(AVFrame *frame) ;
};

#endif // FFMPEGPLAYER_H
