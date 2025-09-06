{
  stdenv,
  cmake,
  ninja,
  raylib,
}:
stdenv.mkDerivation {
  name = "rpg-raylib";
  version = "0.1.0";
  src = ../.;

  nativeBuildInputs = [
    clang
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
