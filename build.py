import os 

# Run with python3 build.py
COMPILER="g++ -g -std=c++17"
SOURCE="./src/*.cpp"
EXECUTABLE="prog"
ARGUMENTS="-D LINUX"
INCLUDE_DIR="-I ./thirdparty/glm"

compileString = COMPILER + " " + ARGUMENTS + " " + SOURCE + " " + "-o" + " " + EXECUTABLE + " " + INCLUDE_DIR

exit_code = os.system(compileString)
exit(0 if exit_code==0 else 1)