TARGET := kEachSymInSubstring.cpp
EXECUTABLE := algo

all: $(EXECUTABLE)
	@./$(EXECUTABLE) < input

$(EXECUTABLE): $(TARGET)
	g++ -g -Wall -Wextra -Wpedantic $< -o $@

clean:
	@rm $(EXECUTABLE)
