TOOLS
=====

Apocalypse Design tools, in a more or less chronological order:

* **Adesso-alpha5** was a basic text editor we developed in 1999.
    This editor could read materials from a 3DS file converting them into a custom textual format. It could also import binary files into C/C++ header files (we used this for our "0design" 64k intro).
    The name "Adesso" means "now" in Italian but it's also an acronym for *Apocalypse DEsign Scripting System Organizer*.

* **ASE2A3D** was one of our first attempts to use something better than 3DS files in our demos. It's a converter from 3D Studio Max "ASE" (Ascii Export) format to a old verision of our custom "A3D" format. We later understood that reading ASE files was not a very good idea.

* **ADinterface** was an attempt to create a stylish display-mode selection dialog in Delphi, then compile it to a DLL and use this DLL from C++. It worked, but we never actually used it in a demo (we ended up writing these dialogs directly in C++/Win32 API).

* **VAdesso-alpha2** (which stands for "Visual Adesso") was a first attempt to create a WYSIWYG demo construction tool.

* **VAdesso-v0.5.4** is the latest version of our WYSIWYG demo sequencing tool. It was intended only for software demos and it's quite functional, but not yet production-ready.

* **ADplayer** was a simple player for demos created using VAdesso (there was also a ActiveX version of this player in the works, that could be used to run demos inside Internet Explorer).

* **ADsdk** is a C++ SDK for software-rendered effects (with no 3D accelleration). Each effect could be written ad compiled as a separate plugin. These plugins could then be used directly in VAdesso to compose a demo, then the demo-sequencing file could be saved and played in our demo-player ADplayer.

* **inJPEG** is a library used by VAdesso and ADplayer to load JPEG textures.

* **ADrender** was the base for our 3D engine (later replaced by renderLib), this library abstracted 3D rasterization and texture management, so that we could write both software and hardware-accelerated demos using more or less the same code. The first versions used DirectX 6, we then ugraded to version 7 and then to version 8. The version here has been upgraded to DirectX 9.

* **3DFileFormats** provides some documentation (in Italian) about our 3D file formats. These files were exported directly from 3DStudio Max using our exporter plugin.

* **3DSMaxExporter** is a plugin for 3DStudio Max (version 3.0/4.0/5.0). This plugin exports scenes to our proprietary 3D formats.
