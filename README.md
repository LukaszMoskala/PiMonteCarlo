# Why this is a thing?
Somebody in my school wrote program that almost the same thing.
But it had problem with screen tearing. So I figured out that
I'll fix that by creating better one from scratch. I also used
cross-platform approach rather than windows-only like the other
did.
# Installing dependencies on GNU/Linux
Install `allegro5`, either by compilling it yourself, or using
package manager.
## Ubuntu
`sudo apt install liballegro5-dev`
## Archlinux
`sudo pacman -S allegro`
# Installing dependencies on Windows
See [Allegro5 installation manual](https://www.allegro.cc/manual/5/install/windows.html)
# Compilation on GNU/Linux
```
git clone https://github.com/LukaszMoskala/PiMonteCarlo.git
cd PiMonteCarlo
make
sudo make install
```
# Compillation on Windows
Well, probably you should download `main.cpp` and compile it in your IDE.

You probably want to link `-lallegro_monolith` and copy `allegro_monolith_version.dll`
to folder with program