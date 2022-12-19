# cdb Makefile

SRCDIR = src
INCDIR = inc
OBJDIR = obj
BINDIR = bin

CC = g++
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

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ) | $(BINDIR)
	$(CC) -o $(BIN) $(OBJ) $(LIB)

$(OBJ): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)
