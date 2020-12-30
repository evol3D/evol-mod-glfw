project('evol-mod-glfw', 'c',
version : '0.1',
default_options : ['warning_level=3'])

cmake = import('cmake')

cc = meson.get_compiler('c')

mod_src = [
  'src/mod.c'
]

mod_incdir = [
]

evol_sub = subproject('evol', default_options: ['build_demo=false'])
evolmod_dep = evol_sub.get_variable('evolmod_dep')

glfw_proj = cmake.subproject('glfw')
glfw_dep = glfw_proj.dependency('glfw')

dl_dep = cc.find_library('dl', required: false)

mod_deps = [
  evolmod_dep,
  glfw_dep,
  dl_dep,
]

moduleconfig = get_option('moduleconfig')
configure_file(
  input: moduleconfig,
  command: [
    meson.source_root() + '/subprojects/luajit/src/luajit', 
    meson.source_root() + '/subprojects/evol/buildscripts/bin2cstring.lua', 
    meson.project_source_root() + '/' + moduleconfig
  ],
  output: moduleconfig + '.h',
  capture: true
)

module = shared_library(
  'evmodglfw', mod_src,
  include_directories: mod_incdir,
  dependencies: mod_deps,
)



