{
  description = "builds the editor for XEngine";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };

        buildInputs = [
          pkgs.raylib
          pkgs.gcc
        ];

        src = ./0.1;

      in
      {
        packages.default = pkgs.stdenv.mkDerivation { # might wanna make this be split in the diferent versions based on the paths
          pname = "editor";
          version = "0.1";

          inherit src;

          buildInputs = buildInputs;

          buildPhase = ''
            gcc -o editor Tools/editor.c -lraylib
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp editor $out/bin/
          '';
        };
      }
    );
}

