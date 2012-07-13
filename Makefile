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

INC_DEBUG =  $(INC) -I/usr/include/mongo
CFLAGS_DEBUG =  $(CFLAGS) -g
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB) -lgloox -lpthread -lmongoclient -llua
LDFLAGS_DEBUG =  $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/zabbix_xmpp

INC_RELEASE =  $(INC) -I/usr/include/mongo/
CFLAGS_RELEASE =  $(CFLAGS) -O2
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB) -lgloox -lpthread -lmongoclient -llua
LDFLAGS_RELEASE =  $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/zabbix_xmpp

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/main.o $(OBJDIR_DEBUG)/src/Show.o $(OBJDIR_DEBUG)/src/Server.o $(OBJDIR_DEBUG)/src/Report.o $(OBJDIR_DEBUG)/src/Master.o $(OBJDIR_DEBUG)/src/Learn.o $(OBJDIR_DEBUG)/src/Janitor.o $(OBJDIR_DEBUG)/src/Bot.o $(OBJDIR_DEBUG)/src/Help.o $(OBJDIR_DEBUG)/src/Forget.o $(OBJDIR_DEBUG)/src/Execute.o $(OBJDIR_DEBUG)/src/Config.o $(OBJDIR_DEBUG)/src/Commands.o $(OBJDIR_DEBUG)/src/CommandHandler.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/main.o $(OBJDIR_RELEASE)/src/Show.o $(OBJDIR_RELEASE)/src/Server.o $(OBJDIR_RELEASE)/src/Report.o $(OBJDIR_RELEASE)/src/Master.o $(OBJDIR_RELEASE)/src/Learn.o $(OBJDIR_RELEASE)/src/Janitor.o $(OBJDIR_RELEASE)/src/Bot.o $(OBJDIR_RELEASE)/src/Help.o $(OBJDIR_RELEASE)/src/Forget.o $(OBJDIR_RELEASE)/src/Execute.o $(OBJDIR_RELEASE)/src/Config.o $(OBJDIR_RELEASE)/src/Commands.o $(OBJDIR_RELEASE)/src/CommandHandler.o

all: debug release

release: release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 

debug: before_debug out_debug after_debug

out_debug: $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LDFLAGS_DEBUG) $(LIBDIR_DEBUG) $(OBJ_DEBUG) $(LIB_DEBUG) -o $(OUT_DEBUG)

$(OBJDIR_DEBUG)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/main.cpp -o $(OBJDIR_DEBUG)/src/main.o

$(OBJDIR_DEBUG)/src/Show.o: src/Show.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Show.cpp -o $(OBJDIR_DEBUG)/src/Show.o

$(OBJDIR_DEBUG)/src/Server.o: src/Server.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Server.cpp -o $(OBJDIR_DEBUG)/src/Server.o

$(OBJDIR_DEBUG)/src/Report.o: src/Report.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Report.cpp -o $(OBJDIR_DEBUG)/src/Report.o

$(OBJDIR_DEBUG)/src/Master.o: src/Master.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Master.cpp -o $(OBJDIR_DEBUG)/src/Master.o

$(OBJDIR_DEBUG)/src/Learn.o: src/Learn.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Learn.cpp -o $(OBJDIR_DEBUG)/src/Learn.o

$(OBJDIR_DEBUG)/src/Janitor.o: src/Janitor.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Janitor.cpp -o $(OBJDIR_DEBUG)/src/Janitor.o

$(OBJDIR_DEBUG)/src/Bot.o: src/Bot.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Bot.cpp -o $(OBJDIR_DEBUG)/src/Bot.o

$(OBJDIR_DEBUG)/src/Help.o: src/Help.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Help.cpp -o $(OBJDIR_DEBUG)/src/Help.o

$(OBJDIR_DEBUG)/src/Forget.o: src/Forget.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Forget.cpp -o $(OBJDIR_DEBUG)/src/Forget.o

$(OBJDIR_DEBUG)/src/Execute.o: src/Execute.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Execute.cpp -o $(OBJDIR_DEBUG)/src/Execute.o

$(OBJDIR_DEBUG)/src/Config.o: src/Config.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Config.cpp -o $(OBJDIR_DEBUG)/src/Config.o

$(OBJDIR_DEBUG)/src/Commands.o: src/Commands.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Commands.cpp -o $(OBJDIR_DEBUG)/src/Commands.o

$(OBJDIR_DEBUG)/src/CommandHandler.o: src/CommandHandler.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/CommandHandler.cpp -o $(OBJDIR_DEBUG)/src/CommandHandler.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src

after_release: 

release: before_release out_release after_release

out_release: $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LDFLAGS_RELEASE) $(LIBDIR_RELEASE) $(OBJ_RELEASE) $(LIB_RELEASE) -o $(OUT_RELEASE)

$(OBJDIR_RELEASE)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/main.cpp -o $(OBJDIR_RELEASE)/src/main.o

$(OBJDIR_RELEASE)/src/Show.o: src/Show.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Show.cpp -o $(OBJDIR_RELEASE)/src/Show.o

$(OBJDIR_RELEASE)/src/Server.o: src/Server.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Server.cpp -o $(OBJDIR_RELEASE)/src/Server.o

$(OBJDIR_RELEASE)/src/Report.o: src/Report.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Report.cpp -o $(OBJDIR_RELEASE)/src/Report.o

$(OBJDIR_RELEASE)/src/Master.o: src/Master.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Master.cpp -o $(OBJDIR_RELEASE)/src/Master.o

$(OBJDIR_RELEASE)/src/Learn.o: src/Learn.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Learn.cpp -o $(OBJDIR_RELEASE)/src/Learn.o

$(OBJDIR_RELEASE)/src/Janitor.o: src/Janitor.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Janitor.cpp -o $(OBJDIR_RELEASE)/src/Janitor.o

$(OBJDIR_RELEASE)/src/Bot.o: src/Bot.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Bot.cpp -o $(OBJDIR_RELEASE)/src/Bot.o

$(OBJDIR_RELEASE)/src/Help.o: src/Help.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Help.cpp -o $(OBJDIR_RELEASE)/src/Help.o

$(OBJDIR_RELEASE)/src/Forget.o: src/Forget.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Forget.cpp -o $(OBJDIR_RELEASE)/src/Forget.o

$(OBJDIR_RELEASE)/src/Execute.o: src/Execute.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Execute.cpp -o $(OBJDIR_RELEASE)/src/Execute.o

$(OBJDIR_RELEASE)/src/Config.o: src/Config.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Config.cpp -o $(OBJDIR_RELEASE)/src/Config.o

$(OBJDIR_RELEASE)/src/Commands.o: src/Commands.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Commands.cpp -o $(OBJDIR_RELEASE)/src/Commands.o

$(OBJDIR_RELEASE)/src/CommandHandler.o: src/CommandHandler.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/CommandHandler.cpp -o $(OBJDIR_RELEASE)/src/CommandHandler.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/src

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

