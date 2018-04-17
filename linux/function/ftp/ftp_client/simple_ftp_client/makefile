BIN= client
OBJECTS= ClientFTP.c
CC= gcc
all:	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) -lpthread -DCOULEUR -Wall -march=i686 -O3

debug:	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) -lpthread -DDEBUG -Wall -march=i686

debugc:	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) -lpthread -DDEBUG -DCOULEUR -Wall -march=i686

ncolor:	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) -lpthread -Wall -march=i686 -O3

nopt:	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) -lpthread -DCOULEUR -Wall

optk7:	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN) -lpthread -DCOULEUR -Wall -march=i686 -mtune=athlon -O3	

