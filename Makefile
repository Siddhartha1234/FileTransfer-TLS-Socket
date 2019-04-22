LIBS = -lssl -lcrypto
BIN = app
SRCS = *.cpp utils.hpp 
app:
	g++ -o $(BIN) $(SRCS) $(LIBS) 
