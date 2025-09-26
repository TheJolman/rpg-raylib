{
  mkShellNoCC,
  cmake,
  pkg-config,
  ninja,
  gdb,
  raylib,
  lldb,
  llvmPackages_20,
  cppcheck,
  doxygen,
  cmake-format,
  cmake-lint,
  glfw,
  json_c
}:
mkShellNoCC {
  packages = [
    cmake
    pkg-config
    ninja
    llvmPackages_20.clang
    gdb
    lldb
    llvmPackages_20.clang-tools
    cppcheck
    doxygen
    cmake-format
    cmake-lint
  ];

  buildInputs = [
    raylib
    glfw
    json_c
  ];

  shellHook = ''
    export CC=clang
    export CMAKE_GENERATOR=Ninja
    export CMAKE_BUILD_TYPE=Debug
  '';
}
