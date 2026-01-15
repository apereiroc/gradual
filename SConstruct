import os
import subprocess

# Add command-line option for release mode
AddOption(
    "--release",
    action="store_true",
    help="Build in release mode with optimizations",
    default=False,
)

# Determine build mode
release_mode = GetOption("release")
build_mode = "release" if release_mode else "debug"
build_dir = f"target/{build_mode}"

# Set optimization flags based on build mode
if release_mode:
    opt_flags = ["-O2"]
    print("Building in RELEASE mode (-O2)")
else:
    opt_flags = ["-O0", "-g"]
    print("Building in DEBUG mode (-O0 -g)")

# Create the build environment
env = Environment(
    # CXX="clang++",
    CXXFLAGS=["-std=c++20", "-Wall", "-Wextra", "-pedantic"] + opt_flags,
    CPPPATH=["include"],
    ENV=os.environ,
)
env.Tool("compilation_db")
compdb = env.CompilationDatabase()

# Parse pkg-config for fmt library
env.ParseConfig("pkg-config --cflags --libs fmt")

# Parse pkg-config for catch2
env_test = env.Clone()
env_test.ParseConfig("pkg-config --cflags --libs catch2-with-main")

# Auto-discover and build all example executables
example_sources = Glob("examples/*.cc")

example_progs = []
for src in example_sources:
    target_name = f"{build_dir}/examples/{src.name[:-3]}"  # Remove .cc extension
    prog = env.Program(target=target_name, source=src)
    example_progs.append(prog)

# Auto-discover and build all test executables
test_sources = Glob("tests/*.cc")

test_progs = []
for src in test_sources:
    target_name = f"{build_dir}/tests/{src.name[:-3]}"  # Remove .cc extension
    prog = env_test.Program(target=target_name, source=src)
    test_progs.append(prog)

# Default target (build examples + emit compile_commands.json)
Default(example_progs + [compdb])


# Test target - build and run all tests
def run_tests(target, source, env):
    """Run all test executables"""
    build_mode = "release" if GetOption("release") else "debug"
    test_dir = f"target/{build_mode}/tests"
    
    # Get all test executables dynamically
    import glob
    test_binaries = glob.glob(f"{test_dir}/*")
    
    for test_path in sorted(test_binaries):
        test_name = os.path.basename(test_path)
        print(f"\n{'='*80}")
        print(f"Running {test_name}...")
        print("=" * 80)
        result = subprocess.run([test_path], shell=False)
        if result.returncode != 0:
            return result.returncode
    return 0

# Create test alias that depends on all test executables
test_runner = env.Command("run_tests", test_progs, run_tests)
env.Alias("test", test_runner)
env.AlwaysBuild(test_runner)

# Clean target - remove entire target directory
env.Clean(".", ["target", "compile_commands.json"])
