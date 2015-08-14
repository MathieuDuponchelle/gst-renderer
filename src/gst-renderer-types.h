#ifndef _GST_RENDERER_TYPES
#define _GST_RENDERER_TYPES

#include <glib-object.h>

G_BEGIN_DECLS

#define GST_TYPE_RENDERER_VIDEO_FORMAT (gst_renderer_video_format_get_type ())
GType gst_renderer_video_format_get_type (void);

/**
 * GstRendererVideoFormat:
 * @GST_RENDERER_VIDEO_FORMAT_H264: Encode video to h264.
 * @GST_RENDERER_VIDEO_FORMAT_THEORA: Encode video to theora.
 *
 * Codec used for encoding video.
 *
 */
typedef enum
{
  GST_RENDERER_VIDEO_FORMAT_H264,
  GST_RENDERER_VIDEO_FORMAT_THEORA,
} GstRendererVideoFormat;

#define GST_TYPE_RENDERER_AUDIO_FORMAT (gst_renderer_audio_format_get_type ())
GType gst_renderer_audio_format_get_type (void);

/**
 * GstRendererAudioFormat:
 * @GST_RENDERER_AUDIO_FORMAT_MP3: Encode audio to mp3.
 * @GST_RENDERER_AUDIO_FORMAT_VORBIS: Encode audio to vorbis.
 * @GST_RENDERER_AUDIO_FORMAT_AAC: Encode audio to aac.
 *
 * Codec used for encoding audio.
 *
 */
typedef enum
{
  GST_RENDERER_AUDIO_FORMAT_MP3,
  GST_RENDERER_AUDIO_FORMAT_VORBIS,
  GST_RENDERER_AUDIO_FORMAT_AAC,
} GstRendererAudioFormat;

#define GST_TYPE_RENDERER_CONTAINER_FORMAT (gst_renderer_container_format_get_type ())
GType gst_renderer_container_format_get_type (void);

/**
 * GstRendererContainerFormat:
 * @GST_RENDERER_CONTAINER_FORMAT_OGG: Mux streams in a ogg container.
 * @GST_RENDERER_CONTAINER_FORMAT_MATROSKA: Mux streams in a matroska container.
 * @GST_RENDERER_CONTAINER_FORMAT_MP3: Encode and mux audio as mp3.
 * @GST_RENDERER_CONTAINER_FORMAT_MP4: Mux streams in a mp4 container.
 *
 * Container used for muxing streams.
 *
 */
typedef enum
{
  GST_RENDERER_CONTAINER_FORMAT_OGG,
  GST_RENDERER_CONTAINER_FORMAT_MATROSKA,
  GST_RENDERER_CONTAINER_FORMAT_MP3,
  GST_RENDERER_CONTAINER_FORMAT_MP4,
} GstRendererContainerFormat;

G_END_DECLS

#endif
