// *** Apocalypse Design Scripting System v1.0 ***
// Particle systems script auto-created by Turbo's MAX-plugin
// Design is simply all...

  particle "Spray01"
  {  father="NONE";
     texture="psceneflare.jpg";
     emitter_width="5.000000";
     emitter_height="5.000000";
     faded_particles="0.5";
     max_particles="1000";
     start_time="0";
     end_time="500";
     life="200";
     speed="0.0100000";
     variation="0.1000";
     size_attenuation="0.6, 0.002, 0.001, 0.01;
  }

  gravityWSM "Gravity01"
  {
     type="SPHERICAL";
     strength="0.000000";
     decay="0.000000";
  }

  gravityWSM "Gravity02"
  {
     type="SPHERICAL";
     strength="16.000000";
     decay="0.000000";
  }

