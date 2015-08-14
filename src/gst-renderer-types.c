#include "gst-renderer-types.h"

#define C_ENUM(v) ((gint) v)

/* Video formats */

static const GEnumValue video_format_values[] = {
  {C_ENUM (GST_RENDERER_VIDEO_FORMAT_H264), "GST_RENDERER_VIDEO_FORMAT_H264", "h264"},
  {C_ENUM (GST_RENDERER_VIDEO_FORMAT_THEORA), "GST_RENDERER_VIDEO_FORMAT_THEORA", "theora"},
  {0, NULL, NULL}
};

static void
register_gst_renderer_video_format_select_result (GType * id)
{
  *id = g_enum_register_static ("GstRendererVideoFormat", video_format_values);
}

const gchar *
gst_renderer_video_format_name (GstRendererVideoFormat type)
{
  guint i;

  for (i = 0; i < G_N_ELEMENTS (video_format_values); i++) {
    if (type == video_format_values[i].value)
      return video_format_values[i].value_nick;
  }

  return "Unknown video format";
}

GType
gst_renderer_video_format_get_type (void)
{
  static GType id;
  static GOnce once = G_ONCE_INIT;

  g_once (&once, (GThreadFunc) register_gst_renderer_video_format_select_result, &id);
  return id;
}

/* Audio formats */

static const GEnumValue audio_format_values[] = {
  {C_ENUM (GST_RENDERER_AUDIO_FORMAT_MP3), "GST_RENDERER_AUDIO_FORMAT_MP3", "mp3"},
  {C_ENUM (GST_RENDERER_AUDIO_FORMAT_VORBIS), "GST_RENDERER_AUDIO_FORMAT_VORBIS", "vorbis"},
  {C_ENUM (GST_RENDERER_AUDIO_FORMAT_AAC), "GST_RENDERER_AUDIO_FORMAT_AAC", "aac"},
  {0, NULL, NULL}
};

static void
register_gst_renderer_audio_format_select_result (GType * id)
{
  *id = g_enum_register_static ("GstRendererAudioFormat", audio_format_values);
}

const gchar *
gst_renderer_audio_format_name (GstRendererVideoFormat type)
{
  guint i;

  for (i = 0; i < G_N_ELEMENTS (audio_format_values); i++) {
    if (type == audio_format_values[i].value)
      return audio_format_values[i].value_nick;
  }

  return "Unknown audio format";
}

GType
gst_renderer_audio_format_get_type (void)
{
  static GType id;
  static GOnce once = G_ONCE_INIT;

  g_once (&once, (GThreadFunc) register_gst_renderer_audio_format_select_result, &id);
  return id;
}

/* Container formats */

static const GEnumValue container_format_values[] = {
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_OGG), "GST_RENDERER_CONTAINER_FORMAT_OGG", "ogg"},
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_MATROSKA), "GST_RENDERER_CONTAINER_FORMAT_MATROSKA", "mkv"},
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_MP3), "GST_RENDERER_CONTAINER_FORMAT_MP3", "mp3"},
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_MP4), "GST_RENDERER_CONTAINER_FORMAT_MP4", "mp4"},
  {0, NULL, NULL}
};

static void
register_gst_renderer_container_format_select_result (GType * id)
{
  *id = g_enum_register_static ("GstRendererContainerFormat", container_format_values);
}

const gchar *
gst_renderer_container_format_name (GstRendererVideoFormat type)
{
  guint i;

  for (i = 0; i < G_N_ELEMENTS (container_format_values); i++) {
    if (type == container_format_values[i].value)
      return container_format_values[i].value_nick;
  }

  return "Unknown container format";
}

GType
gst_renderer_container_format_get_type (void)
{
  static GType id;
  static GOnce once = G_ONCE_INIT;

  g_once (&once, (GThreadFunc) register_gst_renderer_container_format_select_result, &id);
  return id;
}
