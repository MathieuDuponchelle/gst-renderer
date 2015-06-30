#include "gst-renderer-job.h"

#define GET_PRIV(self) (gst_renderer_job_get_instance_private (GST_RENDERER_JOB (self)))

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720
#define DEFAULT_PIXEL_FORMAT "I420"

typedef struct _GstRendererJobPrivate
{
  gchar *uri;
  gchar *location;

  GstRendererContainerFormat container_format;
  gchar *container_preset;

  guint num_video_tracks;
  guint width;
  guint height;
  const gchar *pixel_format;
  gchar *video_preset;
  GstRendererVideoFormat video_format;
  guint num_audio_tracks;
  guint samplerate;
  gchar *audio_preset;
  GstRendererAudioFormat audio_format;
} GstRendererJobPrivate;

struct _GstRendererJob
{
  GObject parent;
};

G_DEFINE_TYPE_WITH_CODE (GstRendererJob, gst_renderer_job, G_TYPE_OBJECT,
    G_ADD_PRIVATE (GstRendererJob)
    )

typedef struct
{
  guint format;
  gchar *format_caps;
} FormatMapping;

#define C_ENUM(v) ((gint) v)

static const FormatMapping video_formats_map[] = {
  {C_ENUM (GST_RENDERER_VIDEO_FORMAT_H264), "video/x-h264"},
  {C_ENUM (GST_RENDERER_VIDEO_FORMAT_THEORA), "video/x-theora"},
  {0, NULL}
};

static const FormatMapping audio_formats_map[] = {
  {C_ENUM (GST_RENDERER_AUDIO_FORMAT_MP3), "audio/mpeg,mpegversion=1,layer=3"},
  {C_ENUM (GST_RENDERER_AUDIO_FORMAT_VORBIS), "audio/x-vorbis"},
  {C_ENUM (GST_RENDERER_AUDIO_FORMAT_AAC), "audio/mpeg, mpegversion=4, base-profile=lc"},
  {0, NULL}
};

static const FormatMapping container_formats_map[] = {
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_OGG), "application/ogg"},
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_MATROSKA), "video/x-matroska"},
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_MP4), "video/quicktime"},
  {C_ENUM (GST_RENDERER_CONTAINER_FORMAT_MP3), "audio/mpeg,mpegversion=1,layer=3"},
  {0, NULL}
};

enum
{
  PROP_0,
  PROP_URI,
  PROP_LOCATION,
  PROP_NUM_VIDEO_TRACKS,
  PROP_WIDTH,
  PROP_HEIGHT,
  PROP_PIXEL_FORMAT,
  PROP_FRAMERATE,
  PROP_VIDEO_PRESET,
  PROP_VIDEO_FORMAT,
  PROP_NUM_AUDIO_TRACKS,
  PROP_SAMPLERATE,
  PROP_AUDIO_PRESET,
  PROP_AUDIO_FORMAT,
  PROP_CONTAINER_FORMAT,
  PROP_CONTAINER_PRESET,
};

static void
_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstRendererJobPrivate *priv = GET_PRIV (object);

  switch (property_id) {
    case PROP_URI:
      priv->uri = g_strdup (g_value_get_string (value));
      break;
    case PROP_LOCATION:
      priv->location = g_strdup (g_value_get_string (value));
      break;
    case PROP_CONTAINER_FORMAT:
      priv->container_format = g_value_get_enum (value);
      break;
    case PROP_CONTAINER_PRESET:
      priv->container_preset = g_strdup (g_value_get_string (value));
      break;
    case PROP_NUM_VIDEO_TRACKS:
      priv->num_video_tracks = g_value_get_uint (value);
      break;
    case PROP_WIDTH:
      priv->width = g_value_get_uint (value);
      if (priv->width == 0)
        priv->width = DEFAULT_WIDTH;
      break;
    case PROP_HEIGHT:
      priv->height = g_value_get_uint (value);
      if (priv->height == 0)
        priv->height = DEFAULT_HEIGHT;
      break;
    case PROP_PIXEL_FORMAT:
      priv->pixel_format = g_strdup (g_value_get_string (value));
      if (priv->pixel_format == NULL)
        priv->pixel_format = g_strdup (DEFAULT_PIXEL_FORMAT);
      break;
    case PROP_VIDEO_PRESET:
      priv->video_preset = g_strdup (g_value_get_string (value));
      break;
    case PROP_VIDEO_FORMAT:
      priv->video_format = g_value_get_enum (value);
      break;
    case PROP_NUM_AUDIO_TRACKS:
      priv->num_audio_tracks = g_value_get_uint (value);
      break;
    case PROP_SAMPLERATE:
      priv->samplerate = g_value_get_uint (value);
      break;
    case PROP_AUDIO_PRESET:
      priv->audio_preset = g_strdup (g_value_get_string (value));
      break;
    case PROP_AUDIO_FORMAT:
      priv->audio_format = g_value_get_enum (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstRendererJobPrivate *priv = GET_PRIV (object);

  switch (property_id) {
    case PROP_URI:
      g_value_set_string (value, priv->uri);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
gst_renderer_job_class_init (GstRendererJobClass *klass)
{
  GObjectClass *g_object_class = G_OBJECT_CLASS (klass);

  g_object_class->set_property = _set_property;
  g_object_class->get_property = _get_property;

  g_object_class_install_property (g_object_class, PROP_URI,
      g_param_spec_string ("uri", "URI", "uri of the resource to render", NULL,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_LOCATION,
      g_param_spec_string ("location", "Location", "filename to render to", NULL,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_NUM_VIDEO_TRACKS,
      g_param_spec_uint ("num-video-tracks", "Number of video tracks",
        "Number of video tracks", 0, G_MAXUINT, 0,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_WIDTH,
      g_param_spec_uint ("width", "Width of the video tracks",
        "Width of the video tracks", 0, G_MAXUINT, DEFAULT_WIDTH,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_HEIGHT,
      g_param_spec_uint ("height", "Height of the video tracks",
        "Height of the video tracks", 0, G_MAXUINT, DEFAULT_HEIGHT,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_PIXEL_FORMAT,
      g_param_spec_string ("pixel-format", "Pixel format",
        "The pixel format to encode in", DEFAULT_PIXEL_FORMAT,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_VIDEO_PRESET,
      g_param_spec_string ("video-preset", "Video Preset",
        "GstPreset used by the video tracks encoders", NULL,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_VIDEO_FORMAT,
      g_param_spec_enum ("video-format", "Video Format",
        "The #GstRendererVideoFormat to use", GST_TYPE_RENDERER_VIDEO_FORMAT,
        GST_RENDERER_VIDEO_FORMAT_THEORA,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_NUM_AUDIO_TRACKS,
      g_param_spec_uint ("num-audio-tracks", "Number of audio tracks",
        "Number of audio tracks to render", 0, G_MAXUINT, 0,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_SAMPLERATE,
      g_param_spec_uint ("samplerate", "Sample rate of the audio",
        "Sample rate of the audio tracks", 0, G_MAXUINT, 44100,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_AUDIO_PRESET,
      g_param_spec_string ("audio-preset", "Audio Preset",
        "GstPreset used by the audio tracks encoders", NULL,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_AUDIO_FORMAT,
      g_param_spec_enum ("audio-format", "Audio Format",
        "The #GstRendererAudioFormat to use", GST_TYPE_RENDERER_AUDIO_FORMAT,
        GST_RENDERER_AUDIO_FORMAT_VORBIS,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_CONTAINER_FORMAT,
      g_param_spec_enum ("container-format", "Container Format",
        "The #GstRendererContainerFormat to use", GST_TYPE_RENDERER_CONTAINER_FORMAT,
        GST_RENDERER_CONTAINER_FORMAT_OGG,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (g_object_class, PROP_CONTAINER_PRESET,
      g_param_spec_string ("container-preset", "Container Preset",
        "GstPreset used by the muxer", NULL,
          G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static const gchar *
get_format_caps (const FormatMapping format_map[], guint format)
{
  FormatMapping mapping = *format_map;

  while (mapping.format_caps) {
    if (mapping.format == format)
      return mapping.format_caps;
    mapping = *format_map++;
  }

  return NULL;
}

static GString *
_build_pipeline_string (GstRendererJob *self)
{
  GString *str = g_string_new (NULL);
  GstRendererJobPrivate *priv = GET_PRIV (self);

  g_string_append_printf (str, "uridecodebin uri=%s ? ", priv->uri);
  g_string_append_printf (str, "encodebin profile-string=\"");
  g_string_append_printf (str, "%s", get_format_caps(container_formats_map, priv->container_format));
  if (priv->container_preset)
    g_string_append_printf (str, "+%s:", priv->container_preset);
  else
    g_string_append (str, ":");
  g_string_append_printf (str, "video/x-raw,width=%d,height=%d,format=%s->",
      priv->width, priv->height, priv->pixel_format);
  g_string_append_printf (str, "%s", get_format_caps(video_formats_map, priv->video_format));
  if (priv->video_preset)
    g_string_append_printf (str, "+%s", priv->video_preset);
  if (priv->num_video_tracks)
    g_string_append_printf (str, "|%d:", priv->num_video_tracks);
  else
    g_string_append (str, ":");
  g_string_append_printf (str, "audio/x-raw,rate=%d->", priv->samplerate);
  g_string_append_printf (str, "%s", get_format_caps(audio_formats_map, priv->audio_format));
  if (priv->audio_preset)
    g_string_append_printf (str, "+%s", priv->audio_preset);
  if (priv->num_audio_tracks)
    g_string_append_printf (str, "|%d", priv->num_audio_tracks);
  g_string_append_printf (str, "\" ! filesink location=%s", priv->location);

  GST_ERROR ("created pipeline string : [%s]", str->str);
  return str;
}

static void
play_pipeline (GstElement *pipeline)
{
  GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  while (1) {
    GstMessage *msg = gst_bus_timed_pop (bus, GST_CLOCK_TIME_NONE);
    if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_EOS) {
      GST_ERROR ("we're done here :D");
      break;
    } else if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
      GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "yopopop");
      GST_ERROR ("we're done here :/");
      break;
    }
  }
}

gboolean
gst_renderer_job_render_sync (GstRendererJob *self)
{
  GString *pipeline_string;
  GError *error = NULL;
  GstElement *pipeline;
  GstRendererJobPrivate *priv = GET_PRIV (self);

  if (!priv->uri || !priv->location) {
    GST_ERROR_OBJECT (self, "uri and location must both be specified");
    return FALSE;
  }

  pipeline_string = _build_pipeline_string (self);
  pipeline = gst_parse_launch (pipeline_string->str, &error);

  g_string_free (pipeline_string, TRUE); 

  if (error)
    return FALSE;

  play_pipeline (pipeline);

  return TRUE;
}

static void
gst_renderer_job_init (GstRendererJob *self)
{
}

GstRendererJob *
gst_renderer_job_new_simple (const gchar *uri, const gchar *location)
{
  return g_object_new (GST_TYPE_RENDERER_JOB, "uri", uri, "location", location, NULL);
}

GstRendererJob *
gst_renderer_job_new_full (const gchar *uri,
    const gchar *location,
    GstRendererContainerFormat cformat,
    const gchar *container_preset,
    guint num_video_tracks,
    guint width,
    guint height,
    const gchar *pixel_format,
    const gchar *video_preset,
    GstRendererVideoFormat vformat,
    guint num_audio_tracks,
    guint samplerate,
    const gchar *audio_preset,
    GstRendererAudioFormat aformat)
{
  return g_object_new (GST_TYPE_RENDERER_JOB,
      "uri", uri,
      "location", location,
      "container-format", cformat,
      "container-preset", container_preset,
      "num-video-tracks", num_video_tracks,
      "width", width,
      "height", height,
      "pixel-format", pixel_format,
      "video-preset", video_preset,
      "video-format", vformat,
      "num-audio-tracks", num_audio_tracks,
      "samplerate", samplerate,
      "audio-preset", audio_preset,
      "audio-format", aformat,
      NULL);
}

GstRendererJob *
gst_renderer_job_new_youtube_full_hd (const gchar *uri, const gchar *location)
{
  return gst_renderer_job_new_full (uri,
      location,
      GST_RENDERER_CONTAINER_FORMAT_MP4,
      "Profile YouTube",
      1,
      1920,
      1080,
      "I420",
      "Profile YouTube",
      GST_RENDERER_VIDEO_FORMAT_H264,
      1,
      48000,
      NULL,
      GST_RENDERER_AUDIO_FORMAT_AAC);
}

GstRendererJob *
gst_renderer_job_new_editing_proxy (const gchar *uri, const gchar *location)
{
  return gst_renderer_job_new_full (uri,
      location,
      GST_RENDERER_CONTAINER_FORMAT_MATROSKA,
      NULL,
      0,
      640,
      360,
      "I420",
      "Quality Low",
      GST_RENDERER_VIDEO_FORMAT_H264,
      0,
      44100,
      NULL,
      GST_RENDERER_AUDIO_FORMAT_AAC);
}
