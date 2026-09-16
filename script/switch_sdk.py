import argparse
import pathlib
import shutil
import sys

# Repository root directory.
ROOT = pathlib.Path(__file__).resolve().parent.parent

# Maps profile files to their destination files in the workspace.
#
# For Example:
# sdk_profiles/gecko_4_5/lib_iec60730.slcp.patch
# -> simplicity_sdk/lib_iec60730.slcp
#
# The *.patch files are profile-specific file snapshots, not git patches.
TARGETS = {
    "iec60730.slce.patch":
        ROOT / "iec60730.slce",

    "iec60730_demo.slcp.patch":
        ROOT / "sample/iec60730_demo/iec60730_demo.slcp",

    "iec60730_demo_cpp.slcp.patch":
        ROOT / "sample/iec60730_demo_cpp/iec60730_demo_cpp.slcp",

    "lib_iec60730.slcp.patch":
        ROOT / "simplicity_sdk/lib_iec60730.slcp",

    "lib_iec60730_secure.slcp.patch":
        ROOT / "simplicity_sdk/lib_iec60730_secure.slcp",

    "lib_iec60730_coding_convention.slcp.patch":
        ROOT / "simplicity_sdk/lib_iec60730_coding_convention.slcp",

    # Sample application
    "main.c.patch":
        ROOT / "sample/iec60730_demo/main.c",

    # Sample C++ application
    "main_cpp.cpp.patch":
        ROOT / "sample/iec60730_demo_cpp/main.cpp",

    # Unit test entry point
    "ut_main.c.patch":
        ROOT / "test/unit_test/src/main.c",

    # Integration test entry point
    "it_main.c.patch":
        ROOT / "test/integration_test/src/main.c",

    # Simplicity SDK entry point (if SDK-specific)
    "sdk_main.c.patch":
        ROOT / "simplicity_sdk/main.c",
}


def main():

    # Read the requested SDK profile.
    parser = argparse.ArgumentParser()
    parser.add_argument("profile")
    args = parser.parse_args()

    # Locate the selected SDK profile directory.
    profile_dir = ROOT / "sdk_profiles" / args.profile

    if not profile_dir.exists():
        sys.exit(f"Profile not found: {args.profile}")

    # Copy SDK-specific files from the selected profile into the workspace.
    for src in sorted(profile_dir.glob("*.patch")):
        if src.name not in TARGETS:
            print(f"[SKIP] {src.name}")
            continue

        dst = TARGETS[src.name]

        shutil.copy2(src, dst)

        print(f"[UPDATED] {dst.relative_to(ROOT)}")

    print(f"\nApplied SDK profile: {args.profile}")


if __name__ == "__main__":
    main()