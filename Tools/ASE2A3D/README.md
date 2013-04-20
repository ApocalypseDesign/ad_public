ASE2A3D
=======

The first 3D modeling application that we used for our demos was 3D Studio R4 (under DOS), and our first 3D engines used to read directly its "3DS" file format.

When we started using 3D Studio Max (I think it was version R2 or 2.5) we wanted to add support for features that were not present in the 3DS format. For example if I remember well, each vertex of a mesh could have only one UV pair in a 3DS, while in Max you could have different UVs for each triangle sharing the vertex.

3D Studio Max native files did not provide the raw geometry data, but we noticed that Max could export such data to a textual format named "ASE" (ASCII Export).

ASE2A3D was our attempt to convert the data in these "ASE" files to a more compact binary format "A3D" that we specifically designed for our 3D engine.

This conversion utility was developed with Borland Delphi (probably version 4.0), as I (h3r3) already had some experience with this enviroment.

After some time working on this tool we realized that ASE files did not contain all the data that was present in the original 3D Studio Max scene, then we changed our plans and decided to implement a 3DS Max plugin so that we could export anything directly to our format (including animation).

I don't remember if we actually used ASE2A3D to make a demo, probably not.

The source code has been lost.