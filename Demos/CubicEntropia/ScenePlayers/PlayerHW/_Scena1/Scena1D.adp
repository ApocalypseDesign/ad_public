// *** Apocalypse Design Scripting System v1.0 ***
// Particle systems script auto-created by Turbo's MAX-plugin
// Design is simply all...

  particle "Spray01"
  {
     father="NONE";
     texture="psceneflare.jpg";
     emitter_width="2.000000";
     emitter_height="2.000000";
     faded_particles="0.3";
     max_particles="2000";
     start_time="0";
     end_time="100";
     life="50";
     speed="1.000000";
     variation="5.000000";
     size_attenuation="0.4, 0.002, 0.001, 0.03";
  }

  gravityWSM "Gravity02"
  {
     type="SPHERICAL";
     strength="20.000000";
     decay="0.000000";
  }

  gravityWSM "Gravity04"
  {
     type="SPHERICAL";
     strength="10.000000";
     decay="0.000000";
  }

