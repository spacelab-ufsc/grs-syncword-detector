TARGET=syncword_unit_test

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
INC=../
FLAGS=-std=c99 -Wall -I$(INC)

.PHONY: all
all: $(TARGET)

$(TARGET): $(BUILD_DIR)/syncword.o $(BUILD_DIR)/syncword_test.o
	$(CC) $(FLAGS) $(BUILD_DIR)/syncword.o $(BUILD_DIR)/syncword_test.o -o $(BUILD_DIR)/$(TARGET) -lcmocka

$(BUILD_DIR)/syncword.o: ../syncword.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/syncword_test.o: syncword_test.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/*.o
