# 3rd-Party #

## Libraries ##
> These libraries are fully or partially integrated as submodules, there may be some patches for better integration.<br>
**Note**: Updates are currently performed manually (git submodule update --remote --force).

| Project | License | Release | Version |
| :------ | :-----: | :-----: | :------ |
| [Dear ImGui](https://github.com/ocornut/imgui.git "imgui")                                    | MIT           | 2020-10-31 | commit\{0bdd426b89feb030a4ed0cbe3e2ef1cd75a099ea} |
| [Vulkan/GL/GLES/EGL/GLX/WGL Loader](https://github.com/Dav1dde/glad "glad")                   | MIT           | 2020-10-18 | commit\{b5d260b65b3edbbd9e75f5a6c9924fe0ff4f04dd} |
| [ImViewGizmo](https://github.com/CedricGuillemet/ImGuizmo.git "ImGuizmo")                     | MIT           | 2020-12-06 | commit\{f14713cc0ec12382ef26cff72848ef076380f25a} |
| [OpenGL-Registry](https://github.com/KhronosGroup/OpenGL-Registry.git "OpenGL-Registry")      | MIT?          | 2020-10-28 | commit\{69ceb7f70d4147367ea85f85406d05c1b028c328} |
| [yaml-cpp](https://github.com/jbeder/yaml-cpp.git "yaml-cpp")                                 | MIT           | 2020-07-27 | commit\{98acc5a8874faab28b82c28936f4b400b389f5d6} |
| [Vulkan-Headers](https://github.com/KhronosGroup/Vulkan-Headers.git "Vulkan-Headers")         | Apache-2.0    | 2020-10-19 | commit\{320af06cbdd29848e1d7100d9b8e4e517db1dfd5} |

> **Info**: Glad must be generated here: <https://glad.dav1d.de/>, the version is the latest commit on the github page.


<!-- These are leftovers which where used in the prototype, left them here, cause maybe they will be needed again.
| [EGL-Registry](https://github.com/KhronosGroup/EGL-Registry.git "EGL-Registry")               | MIT?          | 2020-05-05 | commit\{90b78b0662e2f0548cfd1926fb77bf628933541b} |
| [Magic Enum C++](https://github.com/Neargye/magic_enum "magic_enum")                          | MIT           | 2020-05-24 | commit\{b9450675bbbd8779da5088353cf52417c567eb85} |
| [Node Editor in ImGui](https://github.com/thedmd/imgui-node-editor.git "imgui-node-editor")   | MIT           | 2020-04-17 | commit\{2522c65355879b68ad8c39f05634f92cc8d2bf98} |
-->

<!-- These are leftovers which where used in the prototype, left them here, cause maybe they will be needed again.
## Inspiration ##
> This project is inspired by several other projects, therefore they are listet honorarily.<br>
> **Note**: They are not part of this project, neither their complete source, only some parts of the code or structure may be used.

| Project | License | Release | Version |
| :------ | :-----: | :-----: | :------ |
| [CrossWindow](https://github.com/alaingalvan/CrossWindow.git "CrossWindow")                               | MIT        | 2020-01-06 | commit\{90e6f9f0d05151609a5b1f49c80cd7653b1bda3f} |
| [CrossWindow-Graphics](https://github.com/alaingalvan/CrossWindow-Graphics.git "CrossWindow-Graphics")    | MIT        | 2020-01-06 | commit\{d44babefc50dffdc0be047131d729abd0d0e611e} |
| [CrossShader](https://github.com/alaingalvan/CrossShader.git "CrossShader")                               | MIT        | 2020-01-02 | commit\{1653192b580e64d4d463f2155f668ace640a0456} |
| [GLFW](https://github.com/glfw/glfw "glfw")                                                               | Zlib       | 2020-05-22 | commit\{91eebe922de06a3ed4ba6622686c436c7941aa63} |
| [Object-oriented OpenGL](https://github.com/Overv/OOGL.git "OOGL")                                        | BSL-1.0    | 2018-07-31 | commit\{f28b9823a355fece391d30bc74b38d1f5b27d94e} |

## Story
You wonder why I choose exactly them over the thousend available sources out there?

* CrossWindow: Using the idea of a modern all-in-one Window API.
* CrossWindow-Graphics: Using the idea of a modern all-in-one Graphics API.
* CrossShader: Using the idea of a modern all-in-one Shader API.
* GLFW: Using it only for reference if another 3rd-Party library needs a specific OpenGL feature from glfw, that needs to be implemented.
* OOGL: Using the idea of an interesting event system solution and try to improve it.

-->