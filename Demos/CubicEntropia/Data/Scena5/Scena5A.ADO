// *** Apocalypse Design Scripting System v1.0 ***
// Object script auto-created by Turbo's MAX-plugin
// Design is simply all...

  mesh "Box01" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "Box01" 
  {
    type="SOLID";
    father="NONE";
    material="1 - Default";
    lod="Box01, 0, 99999.99";
    static_vertex="YES";
  }

  mesh "ChamferBox01" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "ChamferBox01" 
  {
    type="SOLID";
    father="NONE";
    material="2 - Default";
    lod="ChamferBox01, 0, 99999.99";
    static_vertex="NO";
  }

