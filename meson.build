project('vec', 'c')

vec_inc = include_directories('.')
vec_lib = static_library('vec', 'vec.c', include_directories : vec_inc)
vec_dep = declare_dependency(include_directories : vec_inc, link_with: vec_lib)