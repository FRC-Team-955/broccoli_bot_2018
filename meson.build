project('broccolibot_vision_2018', 'cpp', default_options : ['cpp_std=c++11'])

sources = [
    'src/bgrd_frame_source.cpp',
    'src/declarative_broccoli_locator.cpp',
    'src/declarative_broccoli_locator_visuals.cpp',
    'src/folder_bgrdfs.cpp',
    'src/frame_io.cpp',
    'src/main.cpp',
    'src/motion_server_connection.cpp',
    'src/realsense_bgrdfs.cpp',
    'src/wo_socket.cpp',
]

include_dir = include_directories('include')

dependencies = [
    dependency('opencv'),
    dependency('realsense2'),
    dependency('boost', modules : ['system']),
    dependency('threads')
]

executable(
    'broccolibot_vision_2018', 
    sources, 
    dependencies : dependencies,
    include_directories : include_dir)
