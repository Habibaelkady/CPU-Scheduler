CC = g++

TARGET = main

all: $(TARGET)

$(TARGET):
	@$(CC) $(TARGET).cpp -g -o lab4
	