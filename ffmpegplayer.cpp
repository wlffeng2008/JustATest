#include "ffmpegplayer.h"
#include <QDebug>
#include <QImage>

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
    avdevice_register_all();
    avformat_network_init();

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

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char BYTE;
#define MAKEWORD(low,high) ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))
#define MAKELONG(low,high) ((LONG)(((WORD)(low)) | ((DWORD)((WORD)(high))) << 16))
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l)>>16)&0xFFFF))
#define LOBYTE(l) ((BYTE)(l))
#define HIBYTE(l) ((BYTE)(((WORD)(l)>>8)&0xFF))


static SwrContext *init_resampler(AVCodecContext *codecCtx,int outSampleRate,int outChannels)
{
    AVSampleFormat in_sample_fmt = codecCtx->sample_fmt;
    AVSampleFormat out_sample_fmt= AV_SAMPLE_FMT_S16;
    int inSampleRate =  codecCtx->sample_rate;
    int inChannels = codecCtx->ch_layout.nb_channels;
    AVChannelLayout in_channel_layout = AV_CHANNEL_LAYOUT_MONO;
    if(inChannels  = 2) in_channel_layout = AV_CHANNEL_LAYOUT_STEREO;
    AVChannelLayout out_channel_layout = AV_CHANNEL_LAYOUT_MONO;
    if(inChannels  = 2) out_channel_layout = AV_CHANNEL_LAYOUT_STEREO;

    static SwrContext *swrCtx = nullptr ;
    if(swrCtx) swr_free(&swrCtx);
    swr_alloc_set_opts2(&swrCtx,
                        &out_channel_layout,out_sample_fmt,outSampleRate,
                        &in_channel_layout,in_sample_fmt,inSampleRate,  0, nullptr
                        );
    if(!swrCtx) return nullptr ;
    swr_init(swrCtx) ;
    return swrCtx;
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

        AVFormatContext *input_ctx = nullptr ;
        AVFormatContext *output_ctx = nullptr ;
        char output_file[256]={0};
        strcpy(output_file,"/home/levi/Desktop/1234.mp4") ;
        //avformat_alloc_output_context2(&output_ctx,nullptr,"mp4",output_file);

        AVCodecContext *codec_ctx_v = nullptr ;
        AVCodecContext *codec_ctx_a = nullptr ;
        AVFrame *frame_v = av_frame_alloc();
        AVFrame *frame_a = av_frame_alloc();

        AVPacket *packet = av_packet_alloc();
        qDebug() << "avformat_init OK!" ;

        int ret = 0;
        int fail= 0;
        while(!m_bClose)
        {
            if(input_ctx) avformat_close_input(&input_ctx);
            input_ctx = avformat_alloc_context();
            AVDictionary *opts = nullptr ;
            av_dict_set(&opts,"stimeout","5000000",0);
            av_dict_set(&opts,"timeout","5000000",0);
            av_dict_set(&opts,"rw_timeout","5000000",0);
            ret = avformat_open_input(&input_ctx,m_strUrl.toStdString().c_str(),nullptr,nullptr);//&opts
            if(ret < 0)
            {
                fail++;
                if(fail >= 3) break;
                QThread::msleep(100);
                continue;
            }
            fail = 0;
            ret = avformat_find_stream_info(input_ctx,nullptr);
            if(ret < 0) continue;

            int video_stream_index = -1;
            int audio_stream_index = -1;
            AVCodecID VCodecId = AV_CODEC_ID_VNULL;
            AVCodecID ACodecId = AV_CODEC_ID_ANULL;
            AVCodecParameters *coderpar_V = nullptr;
            AVCodecParameters *coderpar_A = nullptr;
            for(int i=0; i<input_ctx->nb_streams; i++)
            {
                AVCodecParameters *coderpar = input_ctx->streams[i]->codecpar;
                if(!coderpar) continue;
                if(coderpar->codec_type == AVMEDIA_TYPE_VIDEO)
                {
                    video_stream_index = i;
                    coderpar_V = coderpar;
                    VCodecId = coderpar->codec_id;
                    continue;
                }

                if(coderpar->codec_type == AVMEDIA_TYPE_AUDIO)
                {
                    audio_stream_index = i;
                    coderpar_A = coderpar;
                    ACodecId = coderpar->codec_id;
                    continue;
                }
            }

            if(video_stream_index == -1 && audio_stream_index)
                break;

            if(video_stream_index != -1)
            {
                const AVCodec *codec = avcodec_find_decoder(VCodecId);
                codec_ctx_v = avcodec_alloc_context3(codec) ;
                avcodec_parameters_to_context(codec_ctx_v,coderpar_V);
                avcodec_open2(codec_ctx_v,codec,nullptr);
            }

            if(audio_stream_index != -1)
            {
                const AVCodec *codec = avcodec_find_decoder(ACodecId);
                codec_ctx_a = avcodec_alloc_context3(codec) ;
                avcodec_parameters_to_context(codec_ctx_a,coderpar_A);
                avcodec_open2(codec_ctx_a,codec,nullptr);
            }

            if(output_ctx)
            {
                for(int i=0; i<input_ctx->nb_streams; i++)
                {
                    AVStream *in_stream=input_ctx->streams[i];
                    AVStream *out_stream=avformat_new_stream(output_ctx,nullptr);
                    avcodec_parameters_copy(out_stream->codecpar,in_stream->codecpar);
                    out_stream->codecpar->codec_tag = 0;
                    out_stream->time_base = in_stream->time_base;
                }
                if(!(output_ctx->oformat->flags&AVFMT_NOFILE))
                    avio_open(&output_ctx->pb,output_file,AVIO_FLAG_WRITE);
                avformat_write_header(output_ctx,nullptr);
            }

            qDebug() <<"avformat_open_input:" << m_strUrl << "OK!" ;
            emit onOpen() ;
            emit onOutlog("connted...");

            SwrContext *swrCtx = init_resampler(codec_ctx_a,44100,2);
            int pack_count = 0;

            while(!m_bClose)
            {
                ret = av_read_frame(input_ctx,packet);
                if(ret != 0) break;

                if(packet->size <= 0) continue ;

                if(output_ctx)
                {
                    AVPacket *pkt = av_packet_clone(packet);
                    AVStream *in_stream = input_ctx->streams[pkt->stream_index];
                    AVStream *out_stream= output_ctx->streams[pkt->stream_index];
                    pkt->pts = av_rescale_q_rnd(pkt->pts,in_stream->time_base,out_stream->time_base,
                                                (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
                    pkt->dts = av_rescale_q_rnd(pkt->dts,in_stream->time_base,out_stream->time_base,
                                                (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
                    pkt->duration=av_rescale_q(pkt->duration,in_stream->time_base,out_stream->time_base);
                    pkt->pos = -1;
                    av_interleaved_write_frame(output_ctx,pkt);
                    av_packet_unref(pkt);
                }

                if(pack_count ++ %100){}
                if(packet->stream_index == video_stream_index && video_stream_index != -1)
                {
                    if(avcodec_send_packet(codec_ctx_v,packet) != 0) continue;
                    while (!m_bClose) {
                        if(avcodec_receive_frame(codec_ctx_v,frame_v)<0) break;
                        displayFrame(frame_v) ;
                        //av_usleep(frame_v->duration);
                    }
                }

                if(!swrCtx)continue ;

                if(packet->stream_index == audio_stream_index && audio_stream_index != -1)
                {
                    QByteArray pcmData;
                    if(avcodec_send_packet(codec_ctx_a,packet) != 0) continue;
                    while(!m_bClose)
                    {
                        if(avcodec_receive_frame(codec_ctx_a,frame_a) != 0) break;
                        int out_nb_samples = av_rescale_rnd(swr_get_delay(swrCtx,frame_a->sample_rate)+frame_a->nb_samples,
                                                            44100,frame_a->sample_rate,AV_ROUND_UP);
                        uint8_t *out_buf = nullptr;
                        int out_linesize = 0;
                        av_samples_alloc(&out_buf,&out_linesize,2,out_nb_samples,AV_SAMPLE_FMT_S16,0);
                        if(!out_buf) break;
                        int converted = swr_convert(swrCtx,&out_buf,out_nb_samples, (const uint8_t **)frame_a->data,frame_a->nb_samples);
                        if(converted>0)
                        {
                            int size = av_samples_get_buffer_size(nullptr,2, converted,AV_SAMPLE_FMT_S16,1);
                            int minData=-0x8000;
                            int maxData= 0x7FFF;
                            for(int i=0; i<size; i+=2)
                            {
                                short wdata = MAKEWORD(out_buf[i],out_buf[i+1]);
                                long dwdata = wdata * 2;
                                if(dwdata<minData)
                                    dwdata = minData ;
                                else if(dwdata>maxData)
                                    dwdata = maxData;
                                wdata = LOWORD(dwdata) ;
                                out_buf[i] = LOBYTE(wdata) ;
                                out_buf[i+1] = HIBYTE(wdata) ;
                            }
                            pcmData.append((char*)out_buf,size);
                        }
                    }
                }
            }
        }
        if(input_ctx)
        {
            avformat_close_input(&input_ctx);
            avformat_free_context(input_ctx);
        }

        if(output_ctx)
        {
            av_write_trailer(output_ctx);
            avformat_close_input(&output_ctx);
            avformat_free_context(output_ctx);
        }
        if(frame_a) av_frame_free(&frame_a);
        if(frame_v) av_frame_free(&frame_v);
        if(packet) av_packet_free(&packet);
        if(codec_ctx_a) avcodec_free_context(&codec_ctx_a);
        if(codec_ctx_v) avcodec_free_context(&codec_ctx_v);
        emit onOutlog("disconnted...");

        emit onClose() ;
    }

}

void FFMpegPlayer::displayFrame(AVFrame *frame)
{
    static int width_last = 0;
    static int height_last = 0;
    int width = frame->width;
    int height = frame->height;

    if(width != width_last || height != height_last)
    {
        emit onSize(width,height) ;
    }

    width_last  = frame->width;
    height_last = frame->height;

    SwsContext *sws_ctx = sws_getContext(width,height,(AVPixelFormat)frame->format,
                                        width,height,AV_PIX_FMT_RGB24,SWS_BILINEAR,nullptr,nullptr,nullptr);
    static uint8_t *rgb_data[4]={nullptr};
    static int rgb_size[4]={0} ;
    if(rgb_size[0] == 0) av_image_alloc(rgb_data,rgb_size,width,height,AV_PIX_FMT_RGB24,1);
    if(rgb_size[0] > 0)
    {
        sws_scale(sws_ctx,frame->data,frame->linesize,0,height,rgb_data,rgb_size);
        QImage img(rgb_data[0],width,height,rgb_size[0],QImage::Format_RGB888);

        if(!img.isNull()) emit onImage(img.copy());
    }
    sws_freeContext(sws_ctx);
}
