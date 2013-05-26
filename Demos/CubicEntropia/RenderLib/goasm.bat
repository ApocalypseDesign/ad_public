nvasm -d -h uv1dot3_point.nvv
nvasm -d -h uv1dot3_finalpass.nvv
nvasm -d -h ToonShade_Pass1.nvv
nvasm -d -h ToonShade_PassLights.nvv
nvasm -d -h ToonShade_FinalPass.nvv

nvasm -d uv1dot3_point.nvv
nvasm -d uv1dot3_finalpass.nvv
nvasm -d ToonShade_Pass1.nvv
nvasm -d ToonShade_PassLights.nvv
nvasm -d ToonShade_FinalPass.nvv

@echo off
@xcopy uv1dot3_point.vso c:\3dsmax4\meshes /Y
@xcopy uv1dot3_finalpass.vso c:\3dsmax4\meshes /Y
@xcopy toonshade_pass1.vso c:\3dsmax4\meshes /Y
@xcopy toonshade_passlights.vso c:\3dsmax4\meshes /Y
@xcopy toonshade_finalpass.vso c:\3dsmax4\meshes /Y
@xcopy toongradient.png c:\3dsmax4\meshes /Y

@xcopy uv1dot3_point.vso d:\redshift\demo_sw\psychic\NavigationInterface /Y
@xcopy uv1dot3_finalpass.vso d:\redshift\demo_sw\psychic\NavigationInterface /Y
@xcopy toonshade_pass1.vso d:\redshift\demo_sw\psychic\NavigationInterface /Y
@xcopy toonshade_passlights.vso d:\redshift\demo_sw\psychic\NavigationInterface /Y
@xcopy toonshade_finalpass.vso d:\redshift\demo_sw\psychic\NavigationInterface /Y
@xcopy toongradient.png d:\redshift\demo_sw\psychic\NavigationInterface /Y
