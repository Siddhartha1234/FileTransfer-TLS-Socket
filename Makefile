LIBS = -lssl -lcrypto
BIN = app
SRCS = *.cpp 
app:
	g++ -o $(BIN) $(SRCS) $(LIBS) 

clean:
	rm app
