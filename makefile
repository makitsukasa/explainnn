# http://urin.github.io/posts/2013/simple-makefile-for-clang

# sampleproject
#   |-- makefile
#   |-- bin
#   |   `-- sampleproject  <- 生成される実行ファイル (TARGET)
#   |-- incl
#   |   `-- main.h
#   |-- obj                <- 生成される中間ファイル生成先ディレクトリ (OBJDIR)
#   |   |-- main.d         <- 生成される依存関係ファイル (DEPENDS)
#   |   `-- main.o         <- 生成されるオブジェクトファイル (OBJECTS)
#   `-- src
#       `-- main.cpp

CXX       = g++
CXXFLAGS  = -g -MMD -MP -Wall -Wextra -Winit-self -Wno-missing-field-initializers
ifeq "$(shell getconf LONG_BIT)" "64"
    LDFLAGS   =
else
    LDFLAGS   =
endif
LIBS      =
INCLUDE   = -I./incl
TARGET    = ./bin/$(shell basename `readlink -f .`)
SRCDIR    = ./src
ifeq "$(strip $(SRCDIR))" ""
    SRCDIR    = .
endif
SOURCES   = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR    = ./obj
ifeq "$(strip $(OBJDIR))" ""
    OBJDIR    = .
endif
OBJECTS   = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
DEPENDS   = $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS) $(LIBS)
	@echo \(makefile\) compile $@
	@$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo \(makefile\) compile $<
	-@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

run: $(TARGET)
	@echo \(makefile\) run bin/$(shell basename `readlink -f .`)
	@bin/$(shell basename `readlink -f .`)

clean:
	@echo \(makefile\) clean
	-@rm -f $(OBJECTS) $(DEPENDS) $(TARGET)

-include $(DEPENDS)
