# cdb Makefile

SRCDIR = src
INCDIR = inc
OBJDIR = obj
BINDIR = bin
DOCDIR = doc

DOX = doxygen
DOXYFILE = Doxyfile

CC = g++

DISTCC = distcc
DISTCC_HOSTS=127.0.0.1:9999

CFLAGS = -Wall -g -std=c++17
INCLUDES = -I$(INCDIR) \
           -IDPP/include \

LIB = -LDPP/library \
      -pthread \
      -ldpp \
      -lmosquitto

FILES = main.cpp cdb_io.cpp cdb_logger.cpp cdb_discord_bot.cpp cdb_configurator.cpp \
        cdb_mqtt_handler.cpp
SRC = $(addprefix $(SRCDIR)/,$(FILES))
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o, $(SRC))
BIN = $(BINDIR)/cdb

.PHONY: all dist clean dox

all: $(BIN)

dist: CC = $(DISTCC)
dist: LIB += -lstdc++
dist: | all

$(BIN): $(OBJ) | $(BINDIR)
	$(CC) -o $(BIN) $(OBJ) $(LIB)

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(DOCDIR):
	@mkdir -p $(DOCDIR)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)

dox:  | $(DOCDIR)
	@rm -rf $(DOCDIR)
	$(DOX) $(DOXYFILE)
