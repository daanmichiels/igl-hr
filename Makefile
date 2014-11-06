#
# Makefile for igl-hr
# 

ifeq ($(OS),Windows_NT)
	include Makefile.windows
else
	uname := $(shell uname -s)
ifeq ($(uname),Linux)
	include Makefile.linux
else
ifeq ($(uname),Darwin)
	include Makefile.osx
else
	$(error Unknown platform.)
endif
endif
endif


