# Why this is a thing?
Somebody in my school wrote program that almost the same thing.
But it had problem with screen tearing. So I figured out that
I'll fix that by creating better one from scratch. I also used
cross-platform approach rather than windows-only like the other
did.
# Building
## GNU/Linux
Install `allegro5`, either by compilling it yourself, or using
package manager.

Ubuntu: `sudo apt install liballegro5-dev`

Archlinux: `sudo pacman -S allegro`

After installing dependencies, download sources and build program
```
git clone https://github.com/LukaszMoskala/PiMonteCarlo.git
cd PiMonteCarlo
make
./pimontecarlo
```
And, to install it, if you wish, `sudo make install`
## Windows
You have to install `Allegro5` on your own.
See [Allegro5 installation manual](https://www.allegro.cc/manual/5/install/windows.html)

Then, you can try just using `make` to compile, but it may not work, because, well, windows doing windows things.

If that's the case, open `main.cpp` in your IDE, and link `allegro_monolith`, and hope that it'll work.
I have no way to test this.

# Using custom font
To use custom font, place `font.ttf` in program working directory and it'll pick it up automatically.

You can use other font name, just specify `-f customfont.ttf` and you'r good to go.