CC := g++
PROGRAM := bsc
IDIR :=  lib/fonts \
lib/imgui-docking/backends\
lib/imgui-docking\
lib/gl3w\
lib/stb\
lib/ImGuiColorTextEdit\
src/misc\
src/engine\
src/engine/objects\
src/engine/database\
src\

SRC := $(wildcard lib/imgui-docking/*.cpp)\
 lib/imgui-docking/backends/imgui_impl_opengl3.cpp\
 lib/imgui-docking/backends/imgui_impl_glfw.cpp\
 lib/gl3w/GL/gl3w.cpp\
 lib/ImGuiColorTextEdit/TextEditor.cpp\
 $(wildcard src/misc/*.cpp)\
 $(wildcard src/engine/*.cpp)\
 $(wildcard src/engine/objects/*.cpp)\
 $(wildcard src/engine/database/*.cpp)\
 src/main.cpp
OUT := build
VIBKUP := $(IDIR)

INCLUDE := $(addprefix -I,$(IDIR)) -DBT_USE_DOUBLE_PRECISION -DIMGUI_IMPL_OPENGL_LOADER_GL3W -I/usr/local/include/bullet -I/usr/local/include 
HEADER := $(wildcard $(addsuffix /*.h,$(IDIR)))
OBJ := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))
OUT_OBJ := $(addprefix $(OUT)/,$(OBJ))
CFLAGS := -lm -ljansson -ldl -lopenal -lalut -lassimp -lGL -lglfw -lLinearMath -lBulletCollision -lBulletDynamics -lBulletSoftBody -L/usr/local/lib 

$(PROGRAM): $(OUT_OBJ)
	@echo DONE!
	@$(CC) -o $@ $(OUT_OBJ) $(CFLAGS) $(INCLUDE)

$(OUT)/%.o: %.cpp
	@echo "compiling $<"
	@$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $<

vibackup:
	@ls $(VIBKUP)
test:
	@echo $(HEAD)
clean:
	@rm $(OUT_OBJ)
echo:
	@echo $(INCLUDE)
