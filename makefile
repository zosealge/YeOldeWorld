CC = g++
INC = assets.o camera.o data.o edittools.o gui.o maptools.o menu.o networkclient.o networkhost.o player.o
FLG = -Wall -Wextra -Wno-unused-parameter
RAY = src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
NET = src/libenet.a

all: \
	assets.o \
	camera.o \
	data.o \
	edittools.o \
	gui.o \
	maptools.o \
	menu.o \
	networkclient.o \
	networkhost.o \
	player.o

assets.o: src/inc/cpp/assets.cpp
	$(CC) -c src/inc/cpp/assets.cpp -o assets.o $(FLG)

camera.o: src/inc/cpp/camera.cpp
	$(CC) -c src/inc/cpp/camera.cpp -o camera.o $(FLG)

data.o: src/inc/cpp/data.cpp
	$(CC) -c src/inc/cpp/data.cpp -o data.o $(FLG)

edittools.o: src/inc/cpp/edittools.cpp
	$(CC) -c src/inc/cpp/edittools.cpp -o edittools.o $(FLG)

gui.o: src/inc/cpp/gui.cpp
	$(CC) -c src/inc/cpp/gui.cpp -o gui.o $(FLG)

maptools.o: src/inc/cpp/maptools.cpp
	$(CC) -c src/inc/cpp/maptools.cpp -o maptools.o $(FLG)

menu.o: src/inc/cpp/menu.cpp
	$(CC) -c src/inc/cpp/menu.cpp -o menu.o $(FLG)

networkclient.o: src/inc/cpp/networkclient.cpp
	$(CC) -c src/inc/cpp/networkclient.cpp -o networkclient.o $(FLG)

networkhost.o: src/inc/cpp/networkhost.cpp
	$(CC) -c src/inc/cpp/networkhost.cpp -o networkhost.o $(FLG)

player.o: src/inc/cpp/player.cpp
	$(CC) -c src/inc/cpp/player.cpp -o player.o $(FLG)

install:
	clear
	$(CC) -o olde src/game.cpp $(INC) $(RAY) $(NET) $(FLG)

install_debug:
	clear
	$(CC) -g -o olde src/game.cpp $(INC) $(RAY) $(NET) $(FLG)

install_editor:
	clear
	$(CC) -o editor src/edit.cpp $(INC) $(RAY) $(NET) $(FLG)

install_server:
	clear
	$(CC) -o server src/host.cpp $(INC) $(RAY) $(NET) $(FLG)

uninstall:
	clear
	rm olde
	rm server
	rm editor

clean:
	clear
	rm *.o
