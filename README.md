# RaycastingEngine

A c++ raycasting engine made with SFML

![Alt text](data/images/screenshot/capture.png?raw=true "Title")

## Current State 
 
A simple raycasting engine including :

- Textured walls 
- Textured floor / ceiling
- Working doors
- Thin walls suprot
- Mouse control for camera
- Dynamic map loading (as json exported from Tiled map editor)

## To-do

#### Raycasting 
- ~~Add textured wall~~
- ~~Add thin walls~~
- ~~Add doors~~
- ~~Add doors animations~~
- ~~Add basic shadows on walls~~
- ~~Add floor / ceiling casting~~
- ~~Add floor / ceiling textures mapping~~
- Add sky texture (for bg illusion)
- Add transparent textures suport 
- Add more type of thin walls suprot (not just in the midle of a tile)
- Add sprites  
- Add enimies 
- Add gun logic with multiple gun types

#### Map
- ~~Change map system (load / storing data)~~
- ~~Make map load system compatible with Tiled maps editor~~

#### Other
- ~~Add mouse control~~
- Improve movement feels

## Controls

#### Move

Move with the `Z Q S D` keys, 8 direction movement
Holding the `shift` key while moving for sprinting

#### Turn view

`left` and `right` arrows to turn the view (mouse currently disabled)

#### Interact

Open doors with the `space` key

## Build

#### Dependencies

###### Ubuntu / Debian

    sudo apt-get install build-essential libsfml-dev

#### Clone and build

    git clone https://github.com/Frost-Phoenix/RaycastingEngine.git
    cd RaycastingEngine
    make compile run