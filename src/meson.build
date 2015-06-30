gnome = import ('gnome')

sources = ['gst-renderer-job.c', 'gst-renderer-types.c']
headers = ['gst-renderer-job.h', 'gst-renderer-types.h']

install_headers(headers)

libgstrenderer = shared_library ('gstrenderer',
	   			sources,
				version: '1.0',
	   			install: true,
	   			dependencies: [glib_dep, gobject_dep, gst_dep, pbutils_dep])

if not get_option('disable-introspection')
	girtargets = gnome.generate_gir(libgstrenderer,
	  sources : sources + headers,
	  nsversion : '1.0',
	  namespace : 'GstRenderer',
	  symbol_prefix : 'gst_renderer',
	  identifier_prefix : 'GstRenderer',
	  export_packages : 'gstrenderer',
	  includes : ['GObject-2.0', 'Gst-1.0'],
	  dependencies: [],
	  install : true
	)
endif

pkgconfig_gen(libraries : libgstrenderer,
	      version : '1.0',
	      name : 'libgstrenderer',
	      filebase : 'gstrenderer',
	      subdirs : '.',
	      description : 'A high-level rendering / transcoding interface for GStreamer')
