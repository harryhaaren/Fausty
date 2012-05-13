#!/usr/bin/python

VERSION='0.0.1'
APPNAME='Fausty'
top = './'
out = './.build'

def options(ctx):
  ctx.tool_options('compiler_cxx')

def init(null):
  pass

def configure(ctx):
  
  ctx.check_tool ('compiler_cxx')
  
  ctx.env.CXXFLAGS = ['-pg','-g','-ldl','-Wall','-std=gnu++0x','-Wextra','-fpermissive']
  
  ctx.check_cfg(package='jack',at_least_version='0.118',args='--cflags --libs',uselib_store='JACK')
  ctx.check_cfg(package='gtkmm-2.4',at_least_version='2.4.0',args='--cflags --libs',uselib_store='GTKMM')
  ctx.check_cfg(package='gtksourceviewmm-2.0',at_least_version='2.0.0',args='--cflags --libs',uselib_store='SOURCEVIEW')

def build(ctx):
  
  sourceList = [ ]
  
  depends = 'JACK GTKMM SOURCEVIEW'
  
  
  lib = ctx.new_task_gen(
    features  = 'cxx cxxstlib',
    source    = sourceList,
    target    = 'faustylib',
    uselib    = depends,
    export_includes = '.')
  
  
  main = ctx.new_task_gen(
    features  = 'cxx cprogram',
    source    = 'fausty.cpp',
    use       = 'faustylib',
    uselib    = depends,
    target    = 'fausty' )
