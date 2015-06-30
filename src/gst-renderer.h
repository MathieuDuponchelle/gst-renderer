#ifndef _GST_RENDERER
#define _GST_RENDERER

#include <glib-object.h>
#include <gst/gst.h>

#include "gst-renderer-job.h"

/* GstRenderer doesn't deal with GstElements directly, similarly to GstPlayer
 * it will take as input Uniform Resource Identifiers.
 */

/* GstRenderer will do multithreaded batch-rendering */

/* GstRenderer will optionally provide snapshots as GstSample */

/* GstRenderer will not, at least in a first time, perform CPU throttling
 * operations. This would require having seeking support in muxers, to
 * manipulate the rate, and this isn't very well supported for now */

/* GstRenderer will probably expose a "GstRenderingJob" interface or somesuch.
 * This will allow defining output properties in a nice way */

/* GstRenderer will allow rendering multiple tracks with the same mediatype in
 * a container, with the accepted limitation that all these tracks will be
 * encoded in the same way.
 */

/* GstRenderer will offer a ton of helpful methods for creating jobs */

/* GstRendererJob will build its pipeline through parse_launch and the
 * profile-string property of encodebin, this way we will ensure that anything
 * possible with GstRendererJob is possible with gst-launch too */

G_BEGIN_DECLS

#define GST_TYPE_RENDERER (gst_renderer_get_type ())

G_DECLARE_FINAL_TYPE(GstRenderer, gst_renderer, GST, RENDERER, GObject)

gboolean      gst_renderer_add_job  (GstRenderer *renderer, GstRendererJob *job);

GstRenderer * gst_renderer_new      (void);
gboolean      gst_renderer_start    (void);
gboolean      gst_renderer_stop     (gboolean wait_for_unfinished_jobs);
gboolean      gst_renderer_suspend  (void);

G_END_DECLS

#endif
