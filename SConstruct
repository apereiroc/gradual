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

# Set optimization flags based on build mode
if release_mode:
    opt_flags = ["-O2"]
    print("Building in RELEASE mode (-O2)")
else:
    opt_flags = ["-O0", "-g"]
    print("Building in DEBUG mode (-O0 -g)")

# Create the build environment
env = Environment(
    CXX="clang++",
    CXXFLAGS=["-std=c++20", "-Wall", "-Wextra", "-pedantic"] + opt_flags,
    ENV=os.environ,
)
env.Tool("compilation_db")
compdb = env.CompilationDatabase()


# Parse pkg-config for fmt library
env.ParseConfig("pkg-config --cflags --libs fmt")

# Parse pkg-config for catch2
env_test = env.Clone()
env_test.ParseConfig("pkg-config --cflags --libs catch2-with-main")

# Build main executable
main = env.Program(target="main", source="main.cc")

# Build test executables
test_dual = env_test.Program(target="test_dual", source="test_dual.cc")

test_vector = env_test.Program(target="test_vector", source="test_vector.cc")

test_gradient = env_test.Program(target="test_gradient", source="test_gradient.cc")

# Default target (also emit compile_commands.json)
Default([main, compdb])


# Test target - build and run all tests
def run_tests(target, source, env):
    """Run all test executables"""
    tests = ["test_dual", "test_vector", "test_gradient"]
    for test in tests:
        print(f"\n{'='*80}")
        print(f"Running {test}...")
        print("=" * 80)
        result = subprocess.run([f"./{test}"], shell=False)
        if result.returncode != 0:
            return result.returncode
    return 0


# Create test alias that depends on test executables
test_targets = [test_dual, test_vector, test_gradient]
test_runner = env.Command("run_tests", test_targets, run_tests)
env.Alias("test", test_runner)
env.AlwaysBuild(test_runner)

# Clean target
env.Clean(
    ".", ["main", "test_dual", "test_vector", "test_gradient", "compile_commands.json"]
)
