CC = gcc

CFLAGS = -Wall -Wextra

TARGET = address_book

SOURCES = \
	main.c \
	address_book.c \
	interactive_menu.c
	
OBJECTS = $(SOURCES:.c=.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@


%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


-include $(DEPENDS)


clean:
	rm -f $(OBJECTS) $(DEPENDS) $(TARGET)


run: $(TARGET)
	./$(TARGET)
