#ifndef _GST_RENDERER_JOB
#define _GST_RENDERER_JOB

#include <gst/gst.h>
#include <glib-object.h>

#include "gst-renderer-types.h"

G_BEGIN_DECLS

#define GST_TYPE_RENDERER_JOB (gst_renderer_job_get_type ())

G_DECLARE_FINAL_TYPE(GstRendererJob, gst_renderer_job, GST, RENDERER_JOB, GObject)

GstRendererJob *gst_renderer_job_new_simple (const gchar *uri, const gchar *location);
GstRendererJob *gst_renderer_job_new_full (const gchar *uri,
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
    GstRendererAudioFormat aformat);

GstRendererJob *gst_renderer_job_new_youtube_full_hd (const gchar *uri, const gchar *location);
GstRendererJob *gst_renderer_job_new_editing_proxy (const gchar *uri, const gchar *location);

gboolean gst_renderer_job_render_sync (GstRendererJob *self);

G_END_DECLS

#endif
