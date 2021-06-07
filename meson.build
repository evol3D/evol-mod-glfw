project('evol-mod-glfw', 'c',
version : '0.1',
default_options : ['warning_level=3', 'c_std=c11'])

src = subproject('evol').get_variable('evolmod_subdir') + '/meson.build'
dst = meson.current_source_dir() + '/import/'
run_command( 'python3', meson.source_root() + '/subprojects/evol/buildscripts/copy.py', src, dst)
subdir('import')

cmake = import('cmake')

vulkan_dep = dependency('vulkan').partial_dependency(compile_args : true)

glfw_proj = cmake.subproject('glfw')
glfw_dep = glfw_proj.dependency('glfw')

mod_src = [
  'src/mod.c',
  'src/ev_imgl.c',

  'src/input/mod_input.c',
  'src/input/script_api/script_api.c',

  'extern/src/glad/glad.c',
]

mod_incdir = [
  '..',
  'include',
  'extern/include',
]

mod_deps = [
  evmod_deps,

  glfw_dep,
  vulkan_dep,
]

module = shared_module(
  'evmodglfw', mod_src,
  include_directories: mod_incdir,
  dependencies: mod_deps,
)

mod_dep = declare_dependency(
  include_directories: mod_incdir,
)

meson.override_dependency('evmod_glfw', mod_dep)

configure_file(input: 'src/input/script_api/script_api.lua', output: 'script_api.lua', copy:true)
