// *** Apocalypse Design Scripting System v1.0 ***
// Object script auto-created by Turbo's MAX-plugin
// Design is simply all...

  mesh "Sphere01" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 0, 0";
  }

  object "Sphere01" 
  {
    type="SOLID";
    father="NONE";
    material="1 - Default";
    lod="Sphere01, 0, 99999.99";
    static_vertex="YES";
  }

  mesh "Plane01" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 0, 0";
  }

  object "Plane01" 
  {
    type="SOLID";
    father="NONE";
    material="2 - Default";
    lod="Plane01, 0, 99999.99";
    static_vertex="YES";
  }

