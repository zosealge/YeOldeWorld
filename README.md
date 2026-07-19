# YeOldeWorld

<<<<<<< HEAD
Welcome to the YeOldeWorld game
=======
Welcome to the YeOldeWorld game/prototype
>>>>>>> refs/remotes/origin/main

inspired by oldschool nes rpg - 8bit graphics, 16x16 tilesets, simple controls.

this is multiplayer only rpg, one could say real-time roguelike, but please don't - it's not.

<<<<<<< HEAD
Olde has:
        - handcrafted maps - that can be big as 4096x4096 tiles
        - dedicated host using ENet - 4 (more in the future) players PVP/E
        - oldschool nes vibe (simpler the better)
=======
# Olde has:

- handcrafted maps - that can be big as 4096x4096 tiles loaded on host and networked to players
- dedicated host using ENet - 4 (more in the future) players
- oldschool nes vibe (simpler the better)
  
>>>>>>> refs/remotes/origin/main

network library is ENet https://github.com/zpl-c/enet
graphics library is Raylib https://github.com/raysan5/raylib

graphics assets are https://opengameart.org/content/simple-broad-purpose-tileset

<<<<<<< HEAD
- provide the enet and raylib libraries into '/src'
=======
# Compilation

- provide the enet and raylib static libraries into '/src'
     - libraylib.a compiled from latest raylib version
     - libenet.a compiled from zpl-c/enet
     - raylib.h to /src/inc
     - enet.h to /src/inc
       
>>>>>>> refs/remotes/origin/main
- make
- make install (for game installation)
- make install_editor
- make install_server
- make clean

Windows compilation is not available at the moment, Olde is developed under Fedora Linux
