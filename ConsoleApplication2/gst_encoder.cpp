#include "gst_encoder.hpp"
#include "stdio.h"
#define DUMP_H264

typedef struct _SpiceGstFrame {
	GstBuffer* buffer;
	GstMapInfo info;
} SpiceGstFrame;


static gboolean gst_video_init()
{
	static int success = 0;
	if (!success) {
		GError* err = NULL;
		if (gst_init_check(NULL, NULL, &err)) {
			success = 1;
		}
		else {
			g_warning("Disabling GStreamer video support: %s", err->message);
			g_clear_error(&err);
			success = -1;
		}
	}
	return success > 0;
}

#ifdef DUMP_H264
static void Dump_H264(guint8* outbuf, gint size)
{
	static FILE* g_fp_264 = NULL;
	if (g_fp_264 == NULL)
	{

		char Buf[128];
		sprintf_s(Buf, "D:\\output.h264");
		fopen_s(&g_fp_264, Buf, "wb");

	}
	if (g_fp_264 != NULL) {
		fwrite(outbuf, size, 1, g_fp_264);
	}

}
#endif

static int fps = 0;
static guint64 start_time = 0;

static GstFlowReturn new_sample(GstAppSink* gstappsink, gpointer opaque)
{	
	if (fps == 0) {
		start_time = g_get_monotonic_time() / 1000;
	}
	else {
		if (g_get_monotonic_time() / 1000 - start_time > 1000) {
			printf("usb video send fps: %d", fps);
			fps = 0;
			start_time = g_get_monotonic_time() / 1000;

		}
	}
	GstSample* sample = gst_app_sink_pull_sample(gstappsink);
	if (sample) {
		fps++;
		SpiceGstFrame* frame = g_new0(SpiceGstFrame, 1);
		GstBuffer* gst_buffer = gst_sample_get_buffer(sample);
		frame->buffer = gst_buffer_ref(gst_buffer);
		gst_sample_unref(sample);
		if (gst_buffer_map(gst_buffer, &frame->info, GST_MAP_READ)) {
#ifdef DUMP_H264
			Dump_H264(frame->info.data, frame->info.size);
#endif 		
		}
		else {
			gst_buffer_unref(frame->buffer);
			g_free(frame);
		}
	}

	return GST_FLOW_OK;
}

static GstBusSyncReply handle_pipeline_message(GstBus* bus, GstMessage* msg, gpointer video_encoder)
{

	if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
		GError* err = NULL;
		gchar* debug_info = NULL;
		gst_message_parse_error(msg, &err, &debug_info);
		printf("GStreamer error from element %s: %s",
			GST_OBJECT_NAME(msg->src), err->message);
		if (debug_info) {
			printf("debug details: %s", debug_info);
			g_free(debug_info);
		}
		g_clear_error(&err);
	}
	return GST_BUS_PASS;
}


GstEncoder::GstEncoder():pipeline(NULL),gstsrc(NULL),appsink(NULL)
{	
	printf("start construct!");
	if (!gst_video_init()) {
		return;
	}
	gchar* desc = g_strdup_printf("dx9screencapsrc name=source ! videorate ! video/x-raw,framerate=120/1 ! videoconvert ! openh264enc ! appsink name=sink");
	GError* err = NULL;
	pipeline = gst_parse_launch_full(desc, NULL, GST_PARSE_FLAG_FATAL_ERRORS, &err);
	printf("gstreamer pipeline %s", desc);
	g_free(desc);
	if (!pipeline || err) {
		
		printf("GStreamer error: %s", err->message);
		g_clear_error(&err);
		if (pipeline) {
			gst_object_unref(pipeline);
			pipeline = NULL;
		}
		return;
	}
	//gstsrc = gst_bin_get_by_name(GST_BIN(pipeline), "source");
	GstAppSink* appsink = (GstAppSink*)(gst_bin_get_by_name((GstBin*)pipeline, "sink"));
	GstAppSinkCallbacks appsink_cbs = { NULL, NULL, &new_sample, {NULL} };
	gst_app_sink_set_callbacks(appsink, &appsink_cbs, NULL, NULL);
	GstBus* bus = gst_pipeline_get_bus((GstPipeline*)(pipeline));
	gst_bus_set_sync_handler(bus, handle_pipeline_message, NULL, NULL);
	gst_object_unref(bus);
	if (gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
		printf("GStreamer pipeline create failure!");
	}

}

GstEncoder::~GstEncoder()
{
}