WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS =
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEBUG =  $(INC) -I/usr/include/mongo -Isrc/base -Isrc/handler
CFLAGS_DEBUG =  $(CFLAGS) -g -Wall
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB) -lgloox -lpthread -lmongoclient -llua
LDFLAGS_DEBUG =  $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/zabbix_xmpp

INC_RELEASE =  $(INC) -I/usr/include/mongo/ -Isrc/base -Isrc/handler
CFLAGS_RELEASE =  $(CFLAGS) -O2
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB) -lgloox -lpthread -lmongoclient -llua
LDFLAGS_RELEASE =  $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/zabbix_xmpp

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/handler/Execute.o $(OBJDIR_DEBUG)/src/handler/Show.o $(OBJDIR_DEBUG)/src/handler/Report.o $(OBJDIR_DEBUG)/src/handler/Learn.o $(OBJDIR_DEBUG)/src/handler/Janitor.o $(OBJDIR_DEBUG)/src/handler/Help.o $(OBJDIR_DEBUG)/src/handler/Forget.o $(OBJDIR_DEBUG)/src/base/Bot.o $(OBJDIR_DEBUG)/src/handler/Config.o $(OBJDIR_DEBUG)/src/handler/Commands.o $(OBJDIR_DEBUG)/src/handler/CommandHandler.o $(OBJDIR_DEBUG)/src/base/main.o $(OBJDIR_DEBUG)/src/base/Server.o $(OBJDIR_DEBUG)/src/base/Master.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/handler/Execute.o $(OBJDIR_RELEASE)/src/handler/Show.o $(OBJDIR_RELEASE)/src/handler/Report.o $(OBJDIR_RELEASE)/src/handler/Learn.o $(OBJDIR_RELEASE)/src/handler/Janitor.o $(OBJDIR_RELEASE)/src/handler/Help.o $(OBJDIR_RELEASE)/src/handler/Forget.o $(OBJDIR_RELEASE)/src/base/Bot.o $(OBJDIR_RELEASE)/src/handler/Config.o $(OBJDIR_RELEASE)/src/handler/Commands.o $(OBJDIR_RELEASE)/src/handler/CommandHandler.o $(OBJDIR_RELEASE)/src/base/main.o $(OBJDIR_RELEASE)/src/base/Server.o $(OBJDIR_RELEASE)/src/base/Master.o

all: debug release

release: release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src/handler || mkdir -p $(OBJDIR_DEBUG)/src/handler
	test -d $(OBJDIR_DEBUG)/src/base || mkdir -p $(OBJDIR_DEBUG)/src/base

after_debug: 

debug: before_debug out_debug after_debug

out_debug: $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LDFLAGS_DEBUG) $(LIBDIR_DEBUG) $(OBJ_DEBUG) $(LIB_DEBUG) -o $(OUT_DEBUG)

$(OBJDIR_DEBUG)/src/handler/Execute.o: src/handler/Execute.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Execute.cpp -o $(OBJDIR_DEBUG)/src/handler/Execute.o

$(OBJDIR_DEBUG)/src/handler/Show.o: src/handler/Show.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Show.cpp -o $(OBJDIR_DEBUG)/src/handler/Show.o

$(OBJDIR_DEBUG)/src/handler/Report.o: src/handler/Report.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Report.cpp -o $(OBJDIR_DEBUG)/src/handler/Report.o

$(OBJDIR_DEBUG)/src/handler/Learn.o: src/handler/Learn.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Learn.cpp -o $(OBJDIR_DEBUG)/src/handler/Learn.o

$(OBJDIR_DEBUG)/src/handler/Janitor.o: src/handler/Janitor.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Janitor.cpp -o $(OBJDIR_DEBUG)/src/handler/Janitor.o

$(OBJDIR_DEBUG)/src/handler/Help.o: src/handler/Help.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Help.cpp -o $(OBJDIR_DEBUG)/src/handler/Help.o

$(OBJDIR_DEBUG)/src/handler/Forget.o: src/handler/Forget.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Forget.cpp -o $(OBJDIR_DEBUG)/src/handler/Forget.o

$(OBJDIR_DEBUG)/src/base/Bot.o: src/base/Bot.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/base/Bot.cpp -o $(OBJDIR_DEBUG)/src/base/Bot.o

$(OBJDIR_DEBUG)/src/handler/Config.o: src/handler/Config.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Config.cpp -o $(OBJDIR_DEBUG)/src/handler/Config.o

$(OBJDIR_DEBUG)/src/handler/Commands.o: src/handler/Commands.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/Commands.cpp -o $(OBJDIR_DEBUG)/src/handler/Commands.o

$(OBJDIR_DEBUG)/src/handler/CommandHandler.o: src/handler/CommandHandler.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/handler/CommandHandler.cpp -o $(OBJDIR_DEBUG)/src/handler/CommandHandler.o

$(OBJDIR_DEBUG)/src/base/main.o: src/base/main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/base/main.cpp -o $(OBJDIR_DEBUG)/src/base/main.o

$(OBJDIR_DEBUG)/src/base/Server.o: src/base/Server.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/base/Server.cpp -o $(OBJDIR_DEBUG)/src/base/Server.o

$(OBJDIR_DEBUG)/src/base/Master.o: src/base/Master.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/base/Master.cpp -o $(OBJDIR_DEBUG)/src/base/Master.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src/handler
	rm -rf $(OBJDIR_DEBUG)/src/base

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/src/handler || mkdir -p $(OBJDIR_RELEASE)/src/handler
	test -d $(OBJDIR_RELEASE)/src/base || mkdir -p $(OBJDIR_RELEASE)/src/base

after_release: 

release: before_release out_release after_release

out_release: $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LDFLAGS_RELEASE) $(LIBDIR_RELEASE) $(OBJ_RELEASE) $(LIB_RELEASE) -o $(OUT_RELEASE)

$(OBJDIR_RELEASE)/src/handler/Execute.o: src/handler/Execute.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Execute.cpp -o $(OBJDIR_RELEASE)/src/handler/Execute.o

$(OBJDIR_RELEASE)/src/handler/Show.o: src/handler/Show.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Show.cpp -o $(OBJDIR_RELEASE)/src/handler/Show.o

$(OBJDIR_RELEASE)/src/handler/Report.o: src/handler/Report.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Report.cpp -o $(OBJDIR_RELEASE)/src/handler/Report.o

$(OBJDIR_RELEASE)/src/handler/Learn.o: src/handler/Learn.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Learn.cpp -o $(OBJDIR_RELEASE)/src/handler/Learn.o

$(OBJDIR_RELEASE)/src/handler/Janitor.o: src/handler/Janitor.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Janitor.cpp -o $(OBJDIR_RELEASE)/src/handler/Janitor.o

$(OBJDIR_RELEASE)/src/handler/Help.o: src/handler/Help.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Help.cpp -o $(OBJDIR_RELEASE)/src/handler/Help.o

$(OBJDIR_RELEASE)/src/handler/Forget.o: src/handler/Forget.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Forget.cpp -o $(OBJDIR_RELEASE)/src/handler/Forget.o

$(OBJDIR_RELEASE)/src/base/Bot.o: src/base/Bot.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/base/Bot.cpp -o $(OBJDIR_RELEASE)/src/base/Bot.o

$(OBJDIR_RELEASE)/src/handler/Config.o: src/handler/Config.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Config.cpp -o $(OBJDIR_RELEASE)/src/handler/Config.o

$(OBJDIR_RELEASE)/src/handler/Commands.o: src/handler/Commands.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/Commands.cpp -o $(OBJDIR_RELEASE)/src/handler/Commands.o

$(OBJDIR_RELEASE)/src/handler/CommandHandler.o: src/handler/CommandHandler.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/handler/CommandHandler.cpp -o $(OBJDIR_RELEASE)/src/handler/CommandHandler.o

$(OBJDIR_RELEASE)/src/base/main.o: src/base/main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/base/main.cpp -o $(OBJDIR_RELEASE)/src/base/main.o

$(OBJDIR_RELEASE)/src/base/Server.o: src/base/Server.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/base/Server.cpp -o $(OBJDIR_RELEASE)/src/base/Server.o

$(OBJDIR_RELEASE)/src/base/Master.o: src/base/Master.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/base/Master.cpp -o $(OBJDIR_RELEASE)/src/base/Master.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/src/handler
	rm -rf $(OBJDIR_RELEASE)/src/base

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

