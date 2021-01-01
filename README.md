# voxel
A piece of art - 3D printed cubes, semitransparent, arranged in pattern and LED light

48 cubes, back light LEDs, slowly changing their base color, fading in and out, giving a nice, non distracting back ground mood setting for a room

![running (2)](images\running (2).jpg)

cycling through the color wheel every 15min (or so)

![running](images\running.jpg)

## Material

### cube

Transparent filament (Prusament PETG Clear)

### backplane

wood (3mm)

### LEDs

WS2813 (50 LEDs, single LEDs with a diffusor cap)

### µController

Arduino Nano
Prototyping PCB, USB port, USB cable

### parameter case

Optional housing for the Arduino and and some old mechanical fader for parameter adjusting

## Assembly

hot glue (lots of)

solder

## Code

FastLED library for Arduino example code, modified to support parameter settings via analog faders on analog input (potentiometer)

## Backplate

generated with Processing to reflect a [Pythagorean tiling](https://en.wikipedia.org/wiki/Pythagorean_tiling)

laser cutter to cut from wood. The cubes have an edge length of 31.5mm, the squares are cut to be 31.7mm (just check for laser cutter file if dpi was set correctly)

![backplane](hardware\backplane\backplane.png)

### Cubes

generated with Fusion360, interesting part is the 3D Printing part:

A did a lot of experimenting for the correct look, since they are semitransparent, be careful about the surface structure

* fill pattern, top and bottom fill should be "Hilbert Curve", other infills generate ugly stripes and even worse: depending on the rotation on the print plate stripes with different angles. See first image, look closely to the surface to see the differences, there are some "beta cubes" in the assembly
* If available, use a textured sheet on your printer, this gives a better "frosted glass" look
* 3 cubes fit on a Prusa Mini if you rotated them as shown here

![three on a prusa mini](hardware\cube\three on a prusa mini.png)

Once printed, just fold them. They should snap together and are held in place by the back plate (snug fit)

### Assembly

Use lots of hot glue

![assembly](images\assembly.jpg)

![back side](images\back side.jpg)