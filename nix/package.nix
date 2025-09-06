{
  stdenv,
  cmake,
  ninja,
  raylib,
  llvmPackages_20,
}: let
  myStdenv = stdenv.override {
    cc = llvmPackages_20.clang;
  };
in
  myStdenv.mkDerivation {
    name = "rpg-raylib";
    version = "0.1.0";
    src = ../.;

    nativeBuildInputs = [
      cmake
      ninja
    ];

    buildInputs = [
      raylib
    ];

    cmakeFlags = [
      "-DCMAKE_BUILD_TYPE=Release"
      "-G Ninja"
    ];
  }
