#!/usr/bin/make
###
# Makefile
# https://qiita.com/rimria/items/c3f9455b53d77916e6a1
# gtest
# https://next49.hatenadiary.jp/entry/20120821/p1
###

ifeq ($(OS),Windows_NT)
	# MinGW
	# ECHO = :

	# general
	ECHO = echo
else
	ECHO = echo
endif

###
# コンパイラ設定
###
ifeq ($(OS),Windows_NT)
	# cygwin
	# CXX = /bin/g++.exe

	# general
	CXX = g++
	CXXSTD = -std=gnu++20
else
	CXX = /usr/bin/g++
	CXXSTD = -std=c++20
endif
CXXFLAGS   = $(CXXSTD) -Werror -Wall -Wextra -Wfloat-equal -Winit-self -Wno-missing-field-initializers -Wuninitialized
LDFLAGS    =
LIBS       = -lpthread
INCLUDES   = -I./incl
RCXXFLAGS  = $(CXXFLAGS) -O3
RLDFLAGS   = $(LDFLAGS)
RLIBS      = $(LIBS)
RINCLUDES  = $(INCLUDES)
DCXXFLAGS  = $(CXXFLAGS) -O0 -g
DLDFLAGS   = $(LDFLAGS)
DLIBS      = $(LIBS)
DINCLUDES  = $(INCLUDES)
TCXXFLAGS  = $(CXXFLAGS)
TLDFLAGS   = $(LDFLAGS)
TLIBS      = $(LIBS) /usr/local/lib/libgtest.a /usr/local/lib/libgtest_main.a
TINCLUDES  = $(INCLUDES)

###
# 実行ファイル名
###
RTARGET   = explainnn
DTARGET   = debug_explainnn
TTARGET   = test_explainnn

###
# ディレクトリ指定
###
RTARGETDIR = ./bin/release
DTARGETDIR = ./bin/debug
TTARGETDIR = ./bin/test
ROBJECTDIR = ./obj/release
DOBJECTDIR = ./obj/debug
TOBJECTDIR = ./obj/test
TLOGDIR    = ./test_log

###
# ソースコードディレクトリ指定
###
SOURCEDIR     = ./src
TSOURCEDIR    = ./test_src

###
# テスト用除外ソース指定
###
TARGETMAINSRC = main.cpp

###
# 他設定
###
.DEFAULT_GOAL := run

###
# 処理部
###
# 1. サブディレクトリを含むディレクトリリストの生成
SRCDIRLIST  := $(shell find $(SOURCEDIR) -type d)
TSRCDIRLIST := $(shell find $(TSOURCEDIR) -type d)
# 2. 全てのcppファイルのリストの生成
SRCLIST     = $(foreach srcdir, $(SRCDIRLIST), $(wildcard $(srcdir)/*.cpp))
TSRCLIST    = $(foreach testsrcdir, $(TSRCDIRLIST), $(wildcard $(testsrcdir)/*.cpp))
# 3. トリミング
CUTSRCLIST  = $(subst $(SOURCEDIR),.,$(SRCLIST))
CUTTSRCLIST = $(subst $(TSOURCEDIR),.,$(TSRCLIST))
# 4. オブジェクトファイル名の決定
ROBJLIST    = $(addprefix $(ROBJECTDIR)/, $(CUTSRCLIST:.cpp=.o))
DOBJLIST    = $(addprefix $(DOBJECTDIR)/, $(CUTSRCLIST:.cpp=.o))
TOBJLIST    = $(addprefix $(TOBJECTDIR)/, $(CUTTSRCLIST:.cpp=.o))
# 5. テスト用にmainを含むファイルの除外
TEMPSRCLIST = $(filter-out %$(TARGETMAINSRC), $(CUTSRCLIST))
TMODULELIST = $(addprefix $(DOBJECTDIR)/, $(TEMPSRCLIST:.cpp=.o))
# 6. ディレクトリ構造のリスト化
ROBJDIRLIST = $(addprefix $(ROBJECTDIR)/, $(SRCDIRLIST))
DOBJDIRLIST = $(addprefix $(DOBJECTDIR)/, $(SRCDIRLIST))
TOBJDIRLIST = $(addprefix $(TOBJECTDIR)/, $(TSRCDIRLIST))

# 7. 各種ビルドターゲット設定
.PHONY: all build run clean debugbuild debugclean testbuild testclean testrun testlog allbuild allclean
all: allclean allbuild

build: $(RTARGET)

run: $(RTARGET)
	@$(ECHO) \(makefile\) run $(RTARGETDIR)/$(RTARGET)
	@$(RTARGETDIR)/$(RTARGET)

clean:
	@$(ECHO) \(makefile\) clean
	@rm -f $(ROBJLIST) $(RTARGETDIR)/$(RTARGET)

debugbuild: $(DTARGET)

debugclean:
	@$(ECHO) \(makefile\) debugclean
	@rm -f $(DOBJLIST) $(DTARGETDIR)/$(DTARGET)

testbuild: $(DTARGET) $(TTARGET)

testclean:
	@$(ECHO) \(makefile\) testclean
	@rm -f $(TOBJLIST) $(DOBJLIST) $(TTARGETDIR)/$(TTARGET)

testrun: testbuild
	@$(ECHO) \(makefile\) testrun
	@chmod +x $(TTARGETDIR)/$(TTARGET)
	@$(TTARGETDIR)/$(TTARGET) -v

testlog: testclean testbuild
	@$(ECHO) \(makefile\) testlog
	@chmod +x $(TTARGETDIR)/$(TTARGET)
	@$(TTARGETDIR)/$(TTARGET) -ojunit
	@if [ ! -e $(TLOGDIR) ]; then mkdir -p $(TLOGDIR); fi
	@mv *.xml $(TLOGDIR)

allbuild: build debugbuild testbuild

allclean: clean debugclean testclean

# 8. ターゲット実行ファイルの生成
$(RTARGET): $(ROBJLIST)
	@$(ECHO) \(makefile\) compile $(RTARGETDIR)/$(RTARGET)
	@if [ ! -e $(RTARGETDIR) ]; then mkdir -p $(RTARGETDIR); fi
	@$(CXX) -o $(RTARGETDIR)/$@ $^ $(RLDFLAGS) $(RLIBS)

$(DTARGET): $(DOBJLIST)
	@$(ECHO) \(makefile\) compile $(DTARGETDIR)/$(DTARGET)
	@if [ ! -e $(DTARGETDIR) ]; then mkdir -p $(DTARGETDIR); fi
	@$(CXX) -o $(DTARGETDIR)/$@ $^ $(DLDFLAGS) $(DLIBS)

$(TTARGET): $(TOBJLIST)
	@$(ECHO) \(makefile\) compile $(TTARGETDIR)/$(TTARGET)
	@if [ ! -e $(TTARGETDIR) ]; then mkdir -p $(TTARGETDIR); fi
	@$(CXX) -o $(TTARGETDIR)/$@ $^ $(TMODULELIST) $(TLDFLAGS) $(TLIBS)

# 9. 中間バイナリの生成
$(ROBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	@$(ECHO) \(makefile\) compile $@
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	@$(CXX) $(RCXXFLAGS) $(RINCLUDES) -o $@ -c $<

$(DOBJECTDIR)/%.o: $(SOURCEDIR)/%.cpp
	@$(ECHO) \(makefile\) compile $@
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	@$(CXX) $(DCXXFLAGS) $(DINCLUDES) -o $@ -c $<

$(TOBJECTDIR)/%.o: $(TSOURCEDIR)/%.cpp
	@$(ECHO) \(makefile\) compile $@
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	@$(CXX) $(TCXXFLAGS) $(TINCLUDES) -o $@ -c $<
