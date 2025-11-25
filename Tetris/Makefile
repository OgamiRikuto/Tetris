# --- OS 判定して実行ファイル名切り替え---
ifeq ($(OS), Windows_NT)
	TARGET := tetris.exe
	LINK 	:= 
else
	TARGET := tetris
	LINK := -lcurses
endif

CC := gcc
CCFLAGS := -Wall -Iincludes -g

SRCDIR := src
SRC :=	$(shell find $(SRCDIR) -name '*.c')

OBJ := $(patsubst %.c, %.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)	
	@$(CC) $(OBJ) -o $@ $(LINK)

%.o: %.c
	@$(CC) $(CCFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	@rm -f $(OBJ) $(TARGET)

.PHONY: all test clean
