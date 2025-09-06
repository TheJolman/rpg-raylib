{
  cmake,
  ninja,
  raylib,
  llvmPackages_20,
  git,
}:
llvmPackages_20.stdenv.mkDerivation {
  name = "rpg-raylib";
  version = "0.1.0";
  src = ../.;

  nativeBuildInputs = [
    cmake
    ninja
    git
  ];

  buildInputs = [
    raylib
  ];

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    "-G Ninja"
  ];
}
