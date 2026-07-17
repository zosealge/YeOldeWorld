CC = g++
INC = assets.o camera.o data.o edittools.o gui.o maptools.o menu.o networkclient.o networkhost.o player.o
FLG = -Wall -Wextra -Wno-unused-parameter
RAY = src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
NET = src/libenet.a


main:
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

install:
	clear
	$(CC) -o olde src/game.cpp $(INC) $(RAY) $(NET) $(FLG)

install_editor:
	clear
	$(CC) -o editor src/edit.cpp $(INC) $(RAY) $(NET) $(FLG)

install_host:
	clear
	$(CC) -o server src/host.cpp $(INC) $(RAY) $(NET) $(FLG)

clean:
	clear
	rm *.o
