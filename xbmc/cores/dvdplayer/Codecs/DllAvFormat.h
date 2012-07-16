#pragma once
#include "DynamicDll.h"
#include "DllAvCodec.h"

extern "C" {
#ifndef HAVE_MMX
#define HAVE_MMX
#endif
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#ifndef __GNUC__
#pragma warning(disable:4244)
#endif
#include "libavformat/avformat.h"
#include "libavformat/riff.h"
}

#include "settings.h"

typedef int64_t offset_t;

class DllAvFormatInterface
{
public:
  virtual ~DllAvFormatInterface() {}
  virtual void av_register_all_dont_call(void)=0;
  virtual AVInputFormat *av_find_input_format(const char *short_name)=0;
  virtual int url_feof(AVIOContext *s)=0;
  virtual void av_close_input_file(AVFormatContext *s)=0;
  virtual void av_close_input_stream(AVFormatContext *s)=0;
  virtual int av_read_frame(AVFormatContext *s, AVPacket *pkt)=0;
  virtual int av_read_play(AVFormatContext *s)=0;
  virtual int av_read_pause(AVFormatContext *s)=0;
  virtual void av_read_frame_flush(AVFormatContext *s)=0;
  virtual int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp, int flags)=0;
  virtual int avformat_seek_file(AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags)=0;
#if (!defined USE_EXTERNAL_FFMPEG)
  virtual int avformat_find_stream_info_dont_call(AVFormatContext *ic, AVDictionary **options)=0;
#endif
  virtual int avformat_open_input(AVFormatContext **ps, const char *filename, AVInputFormat *fmt, AVDictionary **options)=0;
  virtual int init_put_byte(AVIOContext *s, unsigned char *buffer, int buffer_size, int write_flag, void *opaque, 
                            int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                            int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                            offset_t (*seek)(void *opaque, offset_t offset, int whence))=0;
  virtual AVInputFormat *av_probe_input_format(AVProbeData *pd, int is_opened)=0;
  virtual AVInputFormat *av_probe_input_format2(AVProbeData *pd, int is_opened, int *score_max)=0;
  virtual int av_probe_input_buffer(AVIOContext *pb, AVInputFormat **fmt, const char *filename, void *logctx, unsigned int offset, unsigned int max_probe_size)=0;
  virtual void dump_format(AVFormatContext *ic, int index, const char *url, int is_output)=0;
  virtual int url_fopen(AVIOContext **s, const char *filename, int flags)=0;
  virtual int url_fclose(AVIOContext *s)=0;
  virtual offset_t url_fseek(AVIOContext *s, offset_t offset, int whence)=0;
  virtual int get_buffer(AVIOContext *s, unsigned char *buf, int size)=0;
  virtual int get_partial_buffer(AVIOContext *s, unsigned char *buf, int size)=0;
  virtual AVFormatContext *avformat_alloc_context(void)=0;
  virtual AVStream *avformat_new_stream(AVFormatContext *s, AVCodec *c)=0;
  virtual AVOutputFormat *av_guess_format(const char *short_name, const char *filename, const char *mime_type)=0;
  virtual AVIOContext *av_alloc_put_byte(unsigned char *buffer, int buffer_size, int write_flag, void *opaque,
                                           int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                                           int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                                           offset_t (*seek)(void *opaque, offset_t offset, int whence))=0;
  virtual int avformat_write_header (AVFormatContext *s, AVDictionary **options)=0;
  virtual int av_write_trailer(AVFormatContext *s)=0;
  virtual int av_write_frame  (AVFormatContext *s, AVPacket *pkt)=0;
};

#if (defined USE_EXTERNAL_FFMPEG)

// Use direct mapping
class DllAvFormat : public DllDynamic, DllAvFormatInterface
{
public:
  virtual ~DllAvFormat() {}
  virtual void av_register_all() 
  { 
    CSingleLock lock(DllAvCodec::m_critSection);
    ::av_register_all();
  } 
  virtual void av_register_all_dont_call() { *(int* )0x0 = 0; } 
  virtual AVInputFormat *av_find_input_format(const char *short_name) { return ::av_find_input_format(short_name); }
  virtual int url_feof(AVIOContext *s) { return ::url_feof(s); }
  virtual void av_close_input_file(AVFormatContext *s) { ::av_close_input_file(s); }
  virtual void av_close_input_stream(AVFormatContext *s) { ::av_close_input_stream(s); }
  virtual int av_read_frame(AVFormatContext *s, AVPacket *pkt) { return ::av_read_frame(s, pkt); }
  virtual void av_read_frame_flush(AVFormatContext *s) { return ::av_read_frame_flush(s); }
  virtual int av_read_play(AVFormatContext *s) { return ::av_read_play(s); }
  virtual int av_read_pause(AVFormatContext *s) { return ::av_read_pause(s); }
  virtual int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp, int flags) { return ::av_seek_frame(s, stream_index, timestamp, flags); }
  virtual int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options)
  {
    CSingleLock lock(DllAvCodec::m_critSection);
    return ::avformat_find_stream_info(ic, options);
  }
  virtual int avformat_open_input(AVFormatContext **ps, const char *filename, AVInputFormat *fmt, AVDictionary **options)
  { return ::avformat_open_input(ps, filename, fmt, options); }
  virtual int init_put_byte(AVIOContext *s, unsigned char *buffer, int buffer_size, int write_flag, void *opaque, 
                            int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                            int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                            offset_t (*seek)(void *opaque, offset_t offset, int whence)) { return ::init_put_byte(s, buffer, buffer_size, write_flag, opaque, read_packet, write_packet, seek); }
  virtual AVInputFormat *av_probe_input_format(AVProbeData *pd, int is_opened) {return ::av_probe_input_format(pd, is_opened); }
  virtual AVInputFormat *av_probe_input_format2(AVProbeData *pd, int is_opened, int *score_max) {*score_max = 100; return ::av_probe_input_format(pd, is_opened); } // Use av_probe_input_format, this is not exported by ffmpeg's headers
  virtual int av_probe_input_buffer(AVIOContext *pb, AVInputFormat **fmt, const char *filename, void *logctx, unsigned int offset, unsigned int max_probe_size) { return ::av_probe_input_buffer(pb, fmt, filename, logctx, offset, max_probe_size); }
  virtual void dump_format(AVFormatContext *ic, int index, const char *url, int is_output) { ::dump_format(ic, index, url, is_output); }
  virtual int url_fopen(AVIOContext **s, const char *filename, int flags) { return ::url_fopen(s, filename, flags); }
  virtual int url_fclose(AVIOContext *s) { return ::url_fclose(s); }
  virtual offset_t url_fseek(AVIOContext *s, offset_t offset, int whence) { return ::url_fseek(s, offset, whence); }
  virtual int get_buffer(AVIOContext *s, unsigned char *buf, int size) { return ::get_buffer(s, buf, size); }
  virtual int get_partial_buffer(AVIOContext *s, unsigned char *buf, int size) { return ::get_partial_buffer(s, buf, size); }
  virtual AVFormatContext *avformat_alloc_context() { return ::avformat_alloc_context(); }
  virtual AVStream *avformat_new_stream(AVFormatContext *s, AVCodec *c) { return ::avformat_new_stream(s, c); }
  virtual AVOutputFormat *av_guess_format(const char *short_name, const char *filename, const char *mime_type) { return ::av_guess_format(short_name, filename, mime_type); }
  virtual AVIOContext *av_alloc_put_byte(unsigned char *buffer, int buffer_size, int write_flag, void *opaque,
                                           int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                                           int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                                           offset_t (*seek)(void *opaque, offset_t offset, int whence)) { return ::av_alloc_put_byte(buffer, buffer_size, write_flag, opaque, read_packet, write_packet, seek) }
  virtual int avformat_write_header (AVFormatContext *s, AVDictionary **options) { return ::avformat_write_header (s, options); }
  virtual int av_write_trailer(AVFormatContext *s) { return ::av_write_trailer(s); }
  virtual int av_write_frame  (AVFormatContext *s, AVPacket *pkt) { return ::av_write_frame(s, pkt); }

  // DLL faking.
  virtual bool ResolveExports() { return true; }
  virtual bool Load() {
    CLog::Log(LOGDEBUG, "DllAvFormat: Using libavformat system library");
    return true;
  }
  virtual void Unload() {}
};

#else

class DllAvFormat : public DllDynamic, DllAvFormatInterface
{
public:
  DllAvFormat() : DllDynamic( g_settings.GetFFmpegDllFolder() + "avformat-53.dll") {}

  LOAD_SYMBOLS()

  DEFINE_METHOD0(void, av_register_all_dont_call)
  DEFINE_METHOD1(AVInputFormat*, av_find_input_format, (const char *p1))
  DEFINE_METHOD1(int, url_feof, (AVIOContext *p1))
  DEFINE_METHOD1(void, av_close_input_file, (AVFormatContext *p1))
  DEFINE_METHOD1(void, av_close_input_stream, (AVFormatContext *p1))
  DEFINE_METHOD1(int, av_read_play, (AVFormatContext *p1))
  DEFINE_METHOD1(int, av_read_pause, (AVFormatContext *p1))
  DEFINE_METHOD1(void, av_read_frame_flush, (AVFormatContext *p1))
  DEFINE_FUNC_ALIGNED2(int, __cdecl, av_read_frame, AVFormatContext *, AVPacket *)
  DEFINE_FUNC_ALIGNED4(int, __cdecl, av_seek_frame, AVFormatContext*, int, int64_t, int)
  DEFINE_FUNC_ALIGNED6(int, __cdecl, avformat_seek_file, AVFormatContext*, int, int64_t, int64_t, int64_t, int)
  DEFINE_FUNC_ALIGNED2(int, __cdecl, avformat_find_stream_info_dont_call, AVFormatContext*, AVDictionary **)
  DEFINE_FUNC_ALIGNED4(int, __cdecl, avformat_open_input, AVFormatContext **, const char *, AVInputFormat *, AVDictionary **)
  DEFINE_FUNC_ALIGNED2(AVInputFormat*, __cdecl, av_probe_input_format, AVProbeData*, int)
  DEFINE_FUNC_ALIGNED3(AVInputFormat*, __cdecl, av_probe_input_format2, AVProbeData*, int, int*)
  DEFINE_FUNC_ALIGNED6(int, __cdecl, av_probe_input_buffer, AVIOContext *, AVInputFormat **, const char *, void *, unsigned int, unsigned int)
  DEFINE_FUNC_ALIGNED3(int, __cdecl, get_buffer, AVIOContext*, unsigned char *, int)
  DEFINE_FUNC_ALIGNED3(int, __cdecl, get_partial_buffer, AVIOContext*, unsigned char *, int)
  DEFINE_METHOD1(void, url_set_interrupt_cb, (URLInterruptCB *p1))
  DEFINE_METHOD8(int, init_put_byte, (AVIOContext *p1, unsigned char *p2, int p3, int p4, void *p5, 
                  int (*p6)(void *opaque, uint8_t *buf, int buf_size),
                  int (*p7)(void *opaque, uint8_t *buf, int buf_size),
                  offset_t (*p8)(void *opaque, offset_t offset, int whence)))
  DEFINE_METHOD4(void, dump_format, (AVFormatContext *p1, int p2, const char *p3, int p4))
  DEFINE_METHOD3(int, url_fopen, (AVIOContext **p1, const char *p2, int p3))
  DEFINE_METHOD1(int, url_fclose, (AVIOContext *p1))
  DEFINE_METHOD3(offset_t, url_fseek, (AVIOContext *p1, offset_t p2, int p3))
  DEFINE_METHOD0(AVFormatContext *, avformat_alloc_context)
  DEFINE_METHOD2(AVStream *, avformat_new_stream, (AVFormatContext *p1, AVCodec *p2))
  DEFINE_METHOD3(AVOutputFormat *, av_guess_format, (const char *p1, const char *p2, const char *p3))
  DEFINE_METHOD7(AVIOContext *, av_alloc_put_byte, (unsigned char *p1, int p2, int p3, void *p4,
                  int(*p5)(void *opaque, uint8_t *buf, int buf_size),
                  int(*p6)(void *opaque, uint8_t *buf, int buf_size),
                  offset_t(*p7)(void *opaque, offset_t offset, int whence)))
  DEFINE_METHOD2(int, avformat_write_header , (AVFormatContext *p1, AVDictionary **p2))
  DEFINE_METHOD1(int, av_write_trailer, (AVFormatContext *p1))
  DEFINE_METHOD2(int, av_write_frame  , (AVFormatContext *p1, AVPacket *p2))
  BEGIN_METHOD_RESOLVE()
    RESOLVE_METHOD_RENAME(av_register_all, av_register_all_dont_call)
    RESOLVE_METHOD(av_find_input_format)
    RESOLVE_METHOD(url_feof)
    RESOLVE_METHOD(av_close_input_file)
    RESOLVE_METHOD(av_close_input_stream)
    RESOLVE_METHOD(av_read_frame)
    RESOLVE_METHOD(av_read_play)
    RESOLVE_METHOD(av_read_pause)
    RESOLVE_METHOD(av_read_frame_flush)
    RESOLVE_METHOD(av_seek_frame)
    RESOLVE_METHOD(avformat_seek_file)
    RESOLVE_METHOD_RENAME(avformat_find_stream_info, avformat_find_stream_info_dont_call)
    RESOLVE_METHOD(avformat_open_input)
    RESOLVE_METHOD(init_put_byte)
    RESOLVE_METHOD(av_probe_input_format)
    RESOLVE_METHOD(av_probe_input_format2)
    RESOLVE_METHOD(av_probe_input_buffer)
    RESOLVE_METHOD(dump_format)
    RESOLVE_METHOD(url_fopen)
    RESOLVE_METHOD(url_fclose)
    RESOLVE_METHOD(url_fseek)
    RESOLVE_METHOD(get_buffer)
    RESOLVE_METHOD(get_partial_buffer)
    RESOLVE_METHOD(avformat_alloc_context)
    RESOLVE_METHOD(avformat_new_stream)
    RESOLVE_METHOD(av_guess_format)
    RESOLVE_METHOD(av_alloc_put_byte)
    RESOLVE_METHOD(avformat_write_header)
    RESOLVE_METHOD(av_write_trailer)
    RESOLVE_METHOD(av_write_frame)
  END_METHOD_RESOLVE()

  /* dependencies of libavformat */
  DllAvCodec m_dllAvCodec;
  // DllAvUtil loaded implicitely by m_dllAvCodec

public:
  void av_register_all()
  {
    CSingleLock lock(DllAvCodec::m_critSection);
    av_register_all_dont_call();
  }
  int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options)
  {
    CSingleLock lock(DllAvCodec::m_critSection);
    return avformat_find_stream_info_dont_call(ic, options);
  }
  virtual bool Load()
  {
    if (!m_dllAvCodec.Load())
      return false;
    return DllDynamic::Load();
  }
};

#endif
