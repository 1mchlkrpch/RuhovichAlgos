TARGET := dHeap.cpp
EXECUTABLE := algo

all: $(EXECUTABLE)
	@./$(EXECUTABLE)

$(EXECUTABLE): $(TARGET)
	g++ -g -Wall -Wextra -Wpedantic $< -o $@

clean:
	@rm $(EXECUTABLE)
