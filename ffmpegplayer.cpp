#include "ffmpegplayer.h"
#include <QDebug>

#ifdef __cplusplus
extern "C" {
#endif

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libavdevice/avdevice.h>
#include <libavutil/timestamp.h>
#include <libswresample/swresample.h>
#include <libavformat/avio.h>

#ifdef __cplusplus
}
#endif

FFMpegPlayer::FFMpegPlayer(QObject *parent)
    : QThread{parent}
{
    // avdevice_register_all();
    // avformat_network_init();

    start();
}

void FFMpegPlayer::setUrl(const QString&url,bool autoPlay)
{
    m_strUrl = url ;
    m_bPlay = autoPlay ;
}

void FFMpegPlayer::play()
{
    m_bPlay = true ;
}

void FFMpegPlayer::pause()
{
    m_bPlay = false ;
}

void FFMpegPlayer::close()
{
    m_bPlay = false ;
    m_bClose = true ;
}

void FFMpegPlayer::run()
{
    doLoop();
}

void FFMpegPlayer::doLoop()
{
    while(!m_bClose)
    {
        if(m_strUrl.isEmpty() || !m_bPlay)
        {
            QThread::usleep(10) ;
            continue ;
        }

        // AVFormatContext *input_ctx = nullptr ;
        // AVFormatContext *output_ctx = nullptr ;
        // char output_file[256]={0};
        // strcpy(output_file,"/home/1234.mp4") ;
        // //avformat_alloc_output_context2(&output_ctx,nullptr,"mp4",output_file);

        // AVCodecContext *codec_ctx_v = nullptr ;
        // AVCodecContext *codec_ctx_a = nullptr ;
        // AVFrame *frame_v = av_frame_alloc();
        // AVFrame *frame_a = av_frame_alloc();

        // AVPacket *packet = av_packet_alloc();
        qDebug() << "" ;


    }
}
