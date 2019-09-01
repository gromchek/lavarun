## Lava Run

Depending on the mode you choose, you either have to get the longest or shortest time with your maze.  
[Game based on this WC3 map]( https://www.youtube.com/watch?v=A500BgTf8ss )  

### Control
Use spell: Hover tile with cursor and press the hotkey (only for QWER).  
Menu navigation: arrows, Enter, ESC  

Q/E - place or remove lava from tile.  
E - Curse of Weakness. Slows runner moving speed to 50% of normal speed.  
R - Scroll of Speed. Increase runner moving speed to 150% of normal speed.

Other spells (only for 'Free mode'):  
F - start round.  
G - clear map.  
H - gen map.  
J - show runner route.  

### Build

```
mkdir build & cd build
cmake ..
make
```

[Assets dir]( https://www.dropbox.com/s/0dgz3giuc0bh9fw/base.zip?dl=1 )  
Just put `base` directory to `build` directory  


If `cmake` can't find SFML install dir  
use ```cmake -DSFMLDIR="path/to/sfml" ..```


### Dependencies
SFML 2.4.0


### License
MIT