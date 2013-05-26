// *** Apocalypse Design Scripting System v1.0 ***
// Object script auto-created by Turbo's MAX-plugin
// Design is simply all...

  object "ROOT" 
  {
    type="DUMMY";
    father="NONE";
    material="NONE";
  }

  object "BODY01TRNL" 
  {
    type="DUMMY";
    father="ROOT";
    material="NONE";
  }

  object "BODY01L" 
  {
    type="DUMMY";
    father="BODY01TRNL";
    material="NONE";
  }

  mesh "object" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object" 
  {
    type="SOLID";
    father="BODY01L";
    material="Material";
    lod="object, 0, 99999.99";
    static_vertex="YES";
  }

  object "object1" 
  {
    type="DUMMY";
    father="BODY01TRNL";
    material="NONE";
  }

  object "BACK01TRNL" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "BACK01L" 
  {
    type="DUMMY";
    father="BACK01TRNL";
    material="NONE";
  }

  mesh "object2" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object2" 
  {
    type="SOLID";
    father="BACK01L";
    material="Material";
    lod="object2, 0, 99999.99";
    static_vertex="YES";
  }

  object "BUST01TRNL" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "BUST01L" 
  {
    type="DUMMY";
    father="BUST01TRNL";
    material="NONE";
  }

  mesh "object3" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object3" 
  {
    type="SOLID";
    father="BUST01L";
    material="Material";
    lod="object3, 0, 99999.99";
    static_vertex="YES";
  }

  object "BUST01TRNR" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "BUST01R" 
  {
    type="DUMMY";
    father="BUST01TRNR";
    material="NONE";
  }

  mesh "object4" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object4" 
  {
    type="SOLID";
    father="BUST01R";
    material="Material";
    lod="object4, 0, 99999.99";
    static_vertex="YES";
  }

  object "WAIST01TRN" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "WAIST01L" 
  {
    type="DUMMY";
    father="WAIST01TRN";
    material="NONE";
  }

  mesh "object5" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object5" 
  {
    type="SOLID";
    father="WAIST01L";
    material="Material";
    lod="object5, 0, 99999.99";
    static_vertex="YES";
  }

  object "WAIST01TR1" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "WAIST01R" 
  {
    type="DUMMY";
    father="WAIST01TR1";
    material="NONE";
  }

  mesh "object6" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object6" 
  {
    type="SOLID";
    father="WAIST01R";
    material="Material";
    lod="object6, 0, 99999.99";
    static_vertex="YES";
  }

  object "HIP01TRNL" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "HIP01L" 
  {
    type="DUMMY";
    father="HIP01TRNL";
    material="NONE";
  }

  mesh "object7" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object7" 
  {
    type="SOLID";
    father="HIP01L";
    material="Material";
    lod="object7, 0, 99999.99";
    static_vertex="YES";
  }

  object "HIP01TRNR" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "HIP01R" 
  {
    type="DUMMY";
    father="HIP01TRNR";
    material="NONE";
  }

  mesh "object8" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object8" 
  {
    type="SOLID";
    father="HIP01R";
    material="Material";
    lod="object8, 0, 99999.99";
    static_vertex="YES";
  }

  object "TAIL01TRNL" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "TAIL01L" 
  {
    type="DUMMY";
    father="TAIL01TRNL";
    material="NONE";
  }

  mesh "object9" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object9" 
  {
    type="SOLID";
    father="TAIL01L";
    material="Material";
    lod="object9, 0, 99999.99";
    static_vertex="YES";
  }

  object "TAIL01TRNR" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "TAIL01R" 
  {
    type="DUMMY";
    father="TAIL01TRNR";
    material="NONE";
  }

  mesh "object10" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object10" 
  {
    type="SOLID";
    father="TAIL01R";
    material="Material";
    lod="object10, 0, 99999.99";
    static_vertex="YES";
  }

  object "EYE01TRNL" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "EYE01L" 
  {
    type="DUMMY";
    father="EYE01TRNL";
    material="NONE";
  }

  mesh "object11" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object11" 
  {
    type="SOLID";
    father="EYE01L";
    material="Material";
    lod="object11, 0, 99999.99";
    static_vertex="YES";
  }

  object "EYE01TRNR" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "EYE01R" 
  {
    type="DUMMY";
    father="EYE01TRNR";
    material="NONE";
  }

  mesh "object12" 
  {
    normalize="OFF";
    boundvolumes_depths="0, 1, 0";
  }

  object "object12" 
  {
    type="SOLID";
    father="EYE01R";
    material="Material";
    lod="object12, 0, 99999.99";
    static_vertex="YES";
  }

  object "NOSE01TRNL" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "NOSE01L" 
  {
    type="DUMMY";
    father="NOSE01TRNL";
    material="NONE";
  }

  object "NOSE01TRNR" 
  {
    type="DUMMY";
    father="object1";
    material="NONE";
  }

  object "NOSE01R" 
  {
    type="DUMMY";
    father="NOSE01TRNR";
    material="NONE";
  }

