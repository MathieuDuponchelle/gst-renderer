#ifndef _GST_RENDERER_TYPES
#define _GST_RENDERER_TYPES

#include <glib-object.h>

G_BEGIN_DECLS

#define GST_TYPE_RENDERER_VIDEO_FORMAT (gst_renderer_video_format_get_type ())
GType gst_renderer_video_format_get_type (void);

typedef enum
{
  GST_RENDERER_VIDEO_FORMAT_H264,
  GST_RENDERER_VIDEO_FORMAT_THEORA,
} GstRendererVideoFormat;

#define GST_TYPE_RENDERER_AUDIO_FORMAT (gst_renderer_audio_format_get_type ())
GType gst_renderer_audio_format_get_type (void);

typedef enum
{
  GST_RENDERER_AUDIO_FORMAT_MP3,
  GST_RENDERER_AUDIO_FORMAT_VORBIS,
  GST_RENDERER_AUDIO_FORMAT_AAC,
} GstRendererAudioFormat;

#define GST_TYPE_RENDERER_CONTAINER_FORMAT (gst_renderer_container_format_get_type ())
GType gst_renderer_container_format_get_type (void);

typedef enum
{
  GST_RENDERER_CONTAINER_FORMAT_OGG,
  GST_RENDERER_CONTAINER_FORMAT_MATROSKA,
  GST_RENDERER_CONTAINER_FORMAT_MP3,
  GST_RENDERER_CONTAINER_FORMAT_MP4,
} GstRendererContainerFormat;

G_END_DECLS

#endif
