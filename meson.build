project('gst-renderer', 'c')

gobject_dep = dependency('gobject-2.0')
glib_dep = dependency('glib-2.0')
gst_dep = dependency('gstreamer-1.0', version: '>= 1.5')
pbutils_dep = dependency('gstreamer-pbutils-1.0')

inc = include_directories ('src')

subdir('src')
subdir('tools')
