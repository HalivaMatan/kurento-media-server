#include <kms-core.h>
#include <rtmp/kms-rtmp-sender.h>
#include <rtmp/kms-rtmp-session.h>
#include "internal/kms-utils.h"

#define KMS_RTMP_SENDER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), KMS_TYPE_RTMP_SENDER, KmsRtmpSenderPriv))

#define LOCK(obj) (g_mutex_lock(KMS_RTMP_SENDER(obj)->priv->mutex))
#define UNLOCK(obj) (g_mutex_unlock(KMS_RTMP_SENDER(obj)->priv->mutex))

#define VIDEO_CAPS "video/x-flash-video;"
#define AUDIO_CAPS "audio/mpeg,mpegversion=1,layer=3,rate=44100;"

struct _KmsRtmpSenderPriv {
	GMutex *mutex;

	KmsRtmpSession *neg_spec;
	gboolean offerer;

	GstElement *flvmux;
	gchar *url;
	GstElement *audio_queue;
	GstElement *video_queue;
	GstElement *audio_src;
	GstElement *video_src;
};

enum {
	PROP_0,

	PROP_NEG_SPEC,
	PROP_OFFERER,
};

G_DEFINE_TYPE(KmsRtmpSender, kms_rtmp_sender, KMS_TYPE_MEDIA_HANDLER_SINK)

static void
dispose_neg_spec(KmsRtmpSender *self) {
	if (self->priv->neg_spec != NULL) {
		g_object_unref(self->priv->neg_spec);
		self->priv->neg_spec = NULL;
	}
}

static void
dispose_flvmux(KmsRtmpSender *self) {
	if (self->priv->flvmux != NULL) {
		g_object_unref(self->priv->flvmux);
		self->priv->flvmux = NULL;
	}
}

static void
dispose_audio_queue(KmsRtmpSender *self) {
	if (self->priv->audio_queue != NULL) {
		g_object_unref(self->priv->audio_queue);
		self->priv->audio_queue = NULL;
	}
}

static void
dispose_video_queue(KmsRtmpSender *self) {
	if (self->priv->video_queue != NULL) {
		g_object_unref(self->priv->video_queue);
		self->priv->video_queue = NULL;
	}
}

static void
dispose_audio_src(KmsRtmpSender *self) {
	if (self->priv->audio_src != NULL) {
		g_object_unref(self->priv->audio_src);
		self->priv->audio_src = NULL;
	}
}

static void
dispose_video_src(KmsRtmpSender *self) {
	if (self->priv->video_src != NULL) {
		g_object_unref(self->priv->video_src);
		self->priv->video_src = NULL;
	}
}

static void
unlinked(GstPad *pad, GstPad *peer, KmsRtmpSender *self) {
	gst_ghost_pad_set_target(GST_GHOST_PAD(pad), NULL);
}

static void
configure_rtmpsink(KmsRtmpSender *self, GstElement *rtmpsink) {
	g_object_set(rtmpsink, "sync", FALSE, NULL);
	g_object_set(rtmpsink, "enable-last-buffer", FALSE, NULL);
	g_object_set(rtmpsink, "blocksize", 10, NULL);
	g_object_set(rtmpsink, "location", self->priv->url, NULL);
}
static void
found_audio(GstElement *tf, guint prob, GstCaps *caps, KmsRtmpSender *self) {
	GstElement *queue, *testsrc;
	GstPad *sink, *src, *peer;

	LOCK(self);
	queue = gst_object_ref(self->priv->audio_queue);
	testsrc = gst_object_ref(self->priv->audio_src);
	UNLOCK(self);

	if (queue == NULL)
		goto end;

	sink = gst_element_get_static_pad(queue, "sink");
	if (gst_pad_is_linked(sink)) {
		peer = gst_pad_get_peer(sink);
		gst_pad_unlink(peer, sink);
		gst_object_unref(peer);
	}

	src = gst_element_get_static_pad(tf, "src");
	g_print("pad link: %d\n", gst_pad_link(src, sink));
	gst_object_unref(src);
	gst_object_unref(sink);

	gst_object_unref(queue);

	if (testsrc != NULL) {
		gst_element_set_state(testsrc, GST_STATE_PAUSED);
	}

end:
	if (testsrc != NULL)
		gst_object_unref(testsrc);
}

static void
audio_linked(GstPad *pad, GstPad *peer, KmsRtmpSender *self) {
	GstElement *typefind, *queue;
	GstPad *sink;

	typefind = gst_element_factory_make("typefind", NULL);
	queue = kms_utils_create_queue(NULL);
	if (typefind == NULL || queue == NULL) {
		g_warn_if_reached();

		if (typefind != NULL)
			g_object_unref(typefind);

		if (queue != NULL)
			g_object_unref(queue);

		return;
	}

	gst_element_set_state(typefind, GST_STATE_PLAYING);
	gst_element_set_state(queue, GST_STATE_PLAYING);
	gst_bin_add_many(GST_BIN(self), typefind, queue, NULL);

	g_object_connect(typefind, "signal::have-type", found_audio, self, NULL);

	gst_element_link(queue, typefind);
	sink = gst_element_get_static_pad(queue, "sink");
	gst_ghost_pad_set_target(GST_GHOST_PAD(pad), sink);

	/* TODO: Add callback to remove when unlinked */
	g_print("TODO: Add callback to remove when unlinked\n");

	g_object_unref(sink);
}

static void
found_video(GstElement *tf, guint prob, GstCaps *caps, KmsRtmpSender *self) {
	GstElement *flvmux, *rtmpsink, *queue;
	GstPad *sink, *src;

	flvmux = self->priv->flvmux;
	rtmpsink = gst_bin_get_by_name(GST_BIN(self), "rtmpsink");
	queue = gst_bin_get_by_name(GST_BIN(self), "queue");

	if (flvmux == NULL || rtmpsink == NULL || queue == NULL) {
		if (rtmpsink != NULL)
			g_object_unref(rtmpsink);

		if (queue != NULL)
			g_object_unref(queue);

		return;
	}

	/* TODO: search on templates to request a pad */
	gst_element_set_state(flvmux, GST_STATE_READY);

	gst_bin_remove_many(GST_BIN(self), rtmpsink, queue, NULL);
	gst_element_set_state(rtmpsink, GST_STATE_NULL);
	gst_element_set_state(queue, GST_STATE_NULL);
	gst_object_unref(rtmpsink);
	gst_object_unref(queue);

	sink = gst_element_get_request_pad(flvmux, "video");
	src = gst_element_get_static_pad(tf, "src");

	queue = kms_utils_create_queue("queue");
	rtmpsink = gst_element_factory_make("rtmpsink", "rtmpsink");
	configure_rtmpsink(self, rtmpsink);
	gst_bin_add_many(GST_BIN(self), rtmpsink, queue, NULL);
	gst_element_link_many(flvmux, queue, rtmpsink, NULL);
	gst_element_set_state(queue, GST_STATE_PLAYING);
	gst_element_set_state(rtmpsink, GST_STATE_PLAYING);

	gst_element_set_state(flvmux, GST_STATE_PLAYING);

	if (src == NULL || sink == NULL) {
		g_warn_if_reached();

		if (src != NULL)
			g_object_unref(src);

		if (sink != NULL) {
			gst_element_release_request_pad(flvmux, sink);
			gst_object_unref(sink);
		}

		return;
	}
	gst_pad_link(src, sink);

	g_object_unref(src);
	g_object_unref(sink);
}

static void
video_linked(GstPad *pad, GstPad *peer, KmsRtmpSender *self) {
	GstElement *typefind, *queue;
	GstPad *sink;

	typefind = gst_element_factory_make("typefind", NULL);
	queue = kms_utils_create_queue(NULL);

	if (typefind == NULL || queue == NULL) {
		g_warn_if_reached();

		if (typefind != NULL)
			g_object_unref(typefind);

		if (queue != NULL)
			g_object_unref(queue);

		return;
	}

	gst_element_set_state(typefind, GST_STATE_PLAYING);
	gst_element_set_state(queue, GST_STATE_PLAYING);
	gst_bin_add_many(GST_BIN(self), typefind, queue, NULL);

	g_object_connect(typefind, "signal::have-type", found_video, self, NULL);

	gst_element_link(queue, typefind);
	sink = gst_element_get_static_pad(queue, "sink");
	gst_ghost_pad_set_target(GST_GHOST_PAD(pad), sink);

	/* TODO: Add callback to remove when unlinked */
	g_print("TODO: Add callback to remove when unlinked\n");

	g_object_unref(sink);
}

static void
create_fake_audio_sender(KmsRtmpSender *self, GstElement *flvmux) {
	GstElement *src, *enc, *filter, *tee, *queue, *sink;
	GstCaps *caps;

	src = gst_element_factory_make("audiotestsrc", NULL);
	enc = gst_element_factory_make("lame", NULL);
	filter = gst_element_factory_make("capsfilter", NULL);
	queue = kms_utils_create_queue(NULL);
	tee = gst_element_factory_make("tee", NULL);
	sink = gst_element_factory_make("fakesink", NULL);

	if (src == NULL || enc == NULL || filter == NULL || queue == NULL ||
						tee == NULL || sink == NULL) {
		g_warn_if_reached();
		g_printerr("Check if you have lame plugin installed");

		if (src != NULL)
			g_object_unref(src);

		if (filter != NULL)
			g_object_unref(filter);

		if (enc != NULL)
			g_object_unref(enc);

		if (queue != NULL)
			g_object_unref(queue);

		if (tee != NULL)
			g_object_unref(tee);

		if (sink != NULL)
			g_object_unref(sink);

		return;
	}

	/* Configure src for silence */
	g_object_set(src, "wave", 4, NULL);
	g_object_set(src, "is-live", TRUE, NULL);
	g_object_set(sink, "sync", TRUE, NULL);

	g_object_set(enc, "bitrate", 64, NULL);

	caps = gst_caps_from_string(AUDIO_CAPS);
	g_object_set(filter, "caps", caps, NULL);
	gst_caps_unref(caps);

	gst_bin_add_many(GST_BIN(self), src, enc, filter, tee, sink, queue, NULL);
	gst_element_link_many(tee, sink, NULL);
	gst_element_link_many(src, enc, filter, tee, queue, flvmux, NULL);

	kms_utils_release_unlinked_pads(tee);

	gst_element_set_state(enc, GST_STATE_PLAYING);
	gst_element_set_state(filter, GST_STATE_PLAYING);
	gst_element_set_state(queue, GST_STATE_PLAYING);
	gst_element_set_state(tee, GST_STATE_PLAYING);
	gst_element_set_state(sink, GST_STATE_PLAYING);
	gst_element_set_state(src, GST_STATE_PLAYING);

	self->priv->audio_queue = gst_object_ref(queue);
	self->priv->audio_src = gst_object_ref(src);

	KMS_DEBUG_PIPE("Added");
}

static void
create_fake_senders(KmsRtmpSender *self, GstElement *flvmux) {
	create_fake_audio_sender(self, flvmux);
}

static void
create_media_chain(KmsRtmpSender *self) {
	GstElement *rtmpsink, *flvmux, *queue;
	GstPadTemplate *audio_templ, *video_templ;
	GstPad *audio_pad, *video_pad;
	gchar *url;

	url = kms_rtmp_session_get_url(self->priv->neg_spec,
							!self->priv->offerer);

	if (url == NULL) {
		g_warn_if_reached();
		return;
	}

	rtmpsink = gst_element_factory_make("rtmpsink", "rtmpsink");
	flvmux = gst_element_factory_make("flvmux", NULL);
	queue = kms_utils_create_queue("queue");

	if (rtmpsink == NULL || flvmux == NULL || queue == NULL) {
		g_warn_if_reached();

		if (rtmpsink == NULL)
			g_object_unref(rtmpsink);

		if (flvmux != NULL)
			g_object_unref(flvmux);

		if (queue != NULL)
			g_object_unref(queue);

		g_free(url);
		return;
	}

	self->priv->url = url;

	g_object_set(flvmux, "streamable", TRUE, NULL);

	self->priv->flvmux = g_object_ref(flvmux);
	kms_utils_release_unlinked_pads(flvmux);

	gst_element_set_state(flvmux, GST_STATE_PLAYING);
	gst_element_set_state(queue, GST_STATE_PLAYING);

	gst_bin_add_many(GST_BIN(self), rtmpsink, queue, flvmux, NULL);
	gst_element_link_many(flvmux, queue, rtmpsink, NULL);

	configure_rtmpsink(self, rtmpsink);
	gst_element_set_state(rtmpsink, GST_STATE_PLAYING);

	create_fake_senders(self, flvmux);

	/* TODO: Add specific media configurations from rtmp-session */
	audio_templ = gst_pad_template_new("audio_sink",
					GST_PAD_SINK,
					GST_PAD_ALWAYS,
					gst_caps_from_string(AUDIO_CAPS));
	audio_pad = gst_ghost_pad_new_no_target_from_template("audio_sink",
								audio_templ);
	if (audio_pad != NULL) {
		gst_pad_set_active(audio_pad, TRUE);
		g_object_connect(audio_pad, "signal::unlinked", unlinked,
								self, NULL);
		g_object_connect(audio_pad, "signal::linked", audio_linked,
								self, NULL);
		gst_element_add_pad(GST_ELEMENT(self), audio_pad);
	}
	g_object_unref(audio_templ);

	video_templ = gst_pad_template_new("video_sink",
					GST_PAD_SINK,
					GST_PAD_ALWAYS,
					gst_caps_from_string(VIDEO_CAPS));
	video_pad = gst_ghost_pad_new_no_target_from_template("video_sink",
								video_templ);
	if (video_pad != NULL) {
		gst_pad_set_active(video_pad, TRUE);
		g_object_connect(video_pad, "signal::unlinked", unlinked,
								self, NULL);
		g_object_connect(video_pad, "signal::linked", video_linked,
								self, NULL);
		gst_element_add_pad(GST_ELEMENT(self), video_pad);
	}
	g_object_unref(video_templ);
}

static void
constructed(GObject *object) {
	KmsRtmpSender *self = KMS_RTMP_SENDER(object);

	G_OBJECT_CLASS(kms_rtmp_sender_parent_class)->constructed(object);

	g_return_if_fail(self->priv->neg_spec != NULL);

	create_media_chain(self);
}

static void
set_property (GObject *object, guint property_id, const GValue *value,
							GParamSpec *pspec) {
	KmsRtmpSender *self = KMS_RTMP_SENDER(object);

	switch (property_id) {
		case PROP_NEG_SPEC:
			LOCK(self);
			dispose_neg_spec(self);
			self->priv->neg_spec = g_value_dup_object(value);
			UNLOCK(self);
			break;
		case PROP_OFFERER:
			LOCK(self);
			self->priv->offerer = g_value_get_boolean(value);
			UNLOCK(self);
			break;
		default:
			/* We don't have any other property... */
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
			break;
	}
}

static void
get_property(GObject *object, guint property_id, GValue *value,
							GParamSpec *pspec) {
	KmsRtmpSender *self = KMS_RTMP_SENDER(object);

	switch (property_id) {
		case PROP_NEG_SPEC:
			LOCK(self);
			g_value_set_object(value, self->priv->neg_spec);
			UNLOCK(self);
			break;
		case PROP_OFFERER:
			LOCK(self);
			g_value_set_boolean(value, self->priv->offerer);
			UNLOCK(self);
			break;
		default:
			/* We don't have any other property... */
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
			break;
	}
}

static void
dispose(GObject *object) {
	KmsRtmpSender *self = KMS_RTMP_SENDER(object);

	LOCK(self);
	dispose_neg_spec(self);
	dispose_flvmux(self);
	dispose_audio_queue(self);
	dispose_video_queue(self);
	dispose_audio_src(self);
	dispose_video_src(self);
	UNLOCK(self);

	G_OBJECT_CLASS(kms_rtmp_sender_parent_class)->dispose(object);
}

static void
finalize(GObject *object) {
	KmsRtmpSender *self = KMS_RTMP_SENDER(object);

	g_mutex_free(self->priv->mutex);
	if (self->priv->url != NULL)
		g_free(self->priv->url);

	G_OBJECT_CLASS(kms_rtmp_sender_parent_class)->finalize(object);
}

static void
kms_rtmp_sender_class_init(KmsRtmpSenderClass *klass) {
	GParamSpec *pspec;
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	g_type_class_add_private(klass, sizeof(KmsRtmpSenderPriv));

	object_class->finalize = finalize;
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	object_class->constructed = constructed;

	/* HACK:
		Don't know why but padtemplates are NULL in child classes,
		this hack takes them from parent class
	*/
	GST_ELEMENT_CLASS(klass)->padtemplates =
		GST_ELEMENT_CLASS(kms_rtmp_sender_parent_class)->padtemplates;
	GST_ELEMENT_CLASS(klass)->numpadtemplates =
		GST_ELEMENT_CLASS(kms_rtmp_sender_parent_class)->numpadtemplates;

	pspec = g_param_spec_object("neg-spec", "Negotiated Session Spec",
					"Negotiated Session Description",
					KMS_TYPE_RTMP_SESSION,
					G_PARAM_CONSTRUCT_ONLY |
					G_PARAM_WRITABLE);

	g_object_class_install_property(object_class, PROP_NEG_SPEC, pspec);

	pspec = g_param_spec_boolean("offerer", "Offerer",
					"If local party is offerer",
					FALSE, G_PARAM_CONSTRUCT_ONLY |
					G_PARAM_WRITABLE);

	g_object_class_install_property(object_class, PROP_OFFERER, pspec);
}

static void
kms_rtmp_sender_init(KmsRtmpSender *self) {
	self->priv = KMS_RTMP_SENDER_GET_PRIVATE(self);

	self->priv->mutex = g_mutex_new();
	self->priv->neg_spec = NULL;
	self->priv->offerer = FALSE;
	self->priv->flvmux = NULL;
	self->priv->url = NULL;
	self->priv->audio_queue = NULL;
	self->priv->video_queue = NULL;
	self->priv->audio_src = NULL;
	self->priv->video_src = NULL;
}
