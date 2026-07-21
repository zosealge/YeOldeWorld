CC = g++
INC = assets.o camera.o data.o edittools.o gui.o maptools.o menu.o networkclient.o networkhost.o player.o
FLG = -Wall -Wextra -Wno-unused-parameter
RAY = src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
NET = src/libenet.a


debug:
	clear
	$(CC) -g -c src/inc/cpp/assets.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/camera.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/data.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/edittools.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/gui.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/maptools.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/menu.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/networkclient.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/networkhost.cpp $(FLG)
	$(CC) -g -c src/inc/cpp/player.cpp $(FLG)

install:
	clear
	$(CC) -o olde src/game.cpp $(INC) $(RAY) $(NET) $(FLG)

install_all:
	clear
	$(CC) -c src/inc/cpp/assets.cpp $(FLG)
	$(CC) -c src/inc/cpp/camera.cpp $(FLG)
	$(CC) -c src/inc/cpp/data.cpp $(FLG)
	$(CC) -c src/inc/cpp/edittools.cpp $(FLG)
	$(CC) -c src/inc/cpp/gui.cpp $(FLG)
	$(CC) -c src/inc/cpp/maptools.cpp $(FLG)
	$(CC) -c src/inc/cpp/menu.cpp $(FLG)
	$(CC) -c src/inc/cpp/networkclient.cpp $(FLG)
	$(CC) -c src/inc/cpp/networkhost.cpp $(FLG)
	$(CC) -c src/inc/cpp/player.cpp $(FLG)
	$(CC) -o olde src/game.cpp $(INC) $(RAY) $(NET) $(FLG)
	$(CC) -o editor src/edit.cpp $(INC) $(RAY) $(NET) $(FLG)
	$(CC) -o server src/host.cpp $(INC) $(RAY) $(NET) $(FLG)
	rm *.o


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
