#include <gst/gst.h>
#include "gst-renderer.h"

#define TEST_URI "file:///home/meh/Music/taliban.mp4"

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

static void
render (GstElement *element)
{
  GstPad *src, *sink;
  GstElement *pipeline;
  GstElement *filesink = gst_element_factory_make ("filesink", NULL);
  GstElement *encodebin =
      gst_element_factory_make ("encodebin", NULL);
  g_object_set (filesink, "location", "plop.mkv", NULL);

  src = gst_element_get_static_pad (element, "src");
  sink = gst_element_get_request_pad (encodebin, "audio_%u");
  pipeline = gst_pipeline_new ("renderer");

  gst_bin_add_many (GST_BIN (pipeline), element, encodebin, filesink, NULL);
  gst_pad_link (src, sink);
  gst_element_link (encodebin, filesink);
  play_pipeline (pipeline);
}

static void
test_render_video_simple (void)
{
  GstRendererJob *job;

  //job = gst_renderer_job_new_youtube_full_hd (TEST_URI, "/home/meh/Videos/rendered.mp4");

  job = gst_renderer_job_new_editing_proxy (TEST_URI, "/home/meh/Videos/rendered.mp4");
  gst_renderer_job_render_sync (job);
}

int main (int ac, char **av)
{
  gst_init (NULL, NULL);
  GstPluginFeature *h264_feature;

  h264_feature = GST_PLUGIN_FEATURE (gst_element_factory_find ("x264enc"));
  gst_plugin_feature_set_rank (h264_feature, GST_RANK_MARGINAL);
  test_render_video_simple ();
}
