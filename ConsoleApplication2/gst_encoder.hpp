#pragma once
#ifndef  GST_ENCODER_HPP
#define	 GST_ENCODER_HPP

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>

class GstEncoder
{
public:
	GstEncoder();
	~GstEncoder();

private:
	GstElement* pipeline;
	GstAppSink* appsink;
	GstElement* gstsrc;
};


#endif