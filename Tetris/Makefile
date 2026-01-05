PROGRAM := Tetris
# --- OS 判定して実行ファイル名切り替え---
ifeq ($(OS), Windows_NT)
	TARGET := tetris.exe
	LINK 	:= 
	INSTDIR := 
else
	TARGET := tetris
	LINK := -lcurses
	INSTDIR := $(PROGRAM).app/Contents/Resources/C/
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

install: $(TARGET)
	@if [ ! -e $(INSTDIR) ] ; then echo "mkdir $(INSTDIR)" ; mkdir $(INSTDIR) ; fi
	cp -p $(TARGET) $(INSTDIR)

clean:
	@for each in `ls $(TARGET) $(OBJ) 2> /dev/null` ; do echo "rm -f $${each}" ; rm -f $${each} ; done
	@if [ -e $(INSTDIR) ] ; then echo "rm -f -r $(INSTDIR)" ; rm -f -r $(INSTDIR) ; fi


.PHONY: all test clean
