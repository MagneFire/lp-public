# GNU Make project makefile autogenerated by Premake
ifndef config
  config=release
endif

ifndef verbose
  SILENT = @
endif

CC = gcc
CXX = g++
AR = ar

ifndef RESCOMP
  ifdef WINDRES
    RESCOMP = $(WINDRES)
  else
    RESCOMP = windres
  endif
endif

ifeq ($(config),release)
  OBJDIR     = ../../../Output/Targets/Generic/Release/obj/quake2-game
  TARGETDIR  = ../../../Output/Targets/Generic/Release/bin/baseq2
  TARGET     = $(TARGETDIR)/game.so
  DEFINES   += -D__CREATOR_CI20__ -DARCH=\"i386\" -DOSTYPE=\"Linux\" -DNOUNCRYPT -DZIP
  INCLUDES  += -I../../../../../Engine/External/include -I../../../Sources
  ALL_CPPFLAGS  += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS    += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -ffast-math -Wall -Wextra -O2 -fPIC -std=c99 -Wno-unused-function -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-switch -Wno-missing-field-initializers -fPIC -fvisibility=hidden
  ALL_CXXFLAGS  += $(CXXFLAGS) $(ALL_CFLAGS)
  ALL_RESFLAGS  += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS   += $(LDFLAGS) -L../../../Output/Targets/Generic/Release/lib -s -shared
  LDDEPS    +=
  LIBS      += $(LDDEPS)
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

ifeq ($(config),debug)
  OBJDIR     = ../../../Output/Targets/Generic/Debug/obj/quake2-game
  TARGETDIR  = ../../../Output/Targets/Generic/Debug/bin/baseq2
  TARGET     = $(TARGETDIR)/game.so
  DEFINES   += -D__CREATOR_CI20__ -DARCH=\"i386\" -DOSTYPE=\"Linux\" -DNOUNCRYPT -DZIP
  INCLUDES  += -I../../../../../Engine/External/include -I../../../Sources
  ALL_CPPFLAGS  += $(CPPFLAGS) -MMD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS    += $(CFLAGS) $(ALL_CPPFLAGS) $(ARCH) -ffast-math -Wall -Wextra -g -fPIC -std=c99 -Wno-unused-function -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-switch -Wno-missing-field-initializers -fPIC -fvisibility=hidden
  ALL_CXXFLAGS  += $(CXXFLAGS) $(ALL_CFLAGS)
  ALL_RESFLAGS  += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  ALL_LDFLAGS   += $(LDFLAGS) -L../../../Output/Targets/Generic/Debug/lib -shared
  LDDEPS    +=
  LIBS      += $(LDDEPS)
  LINKCMD    = $(CC) -o $(TARGET) $(OBJECTS) $(RESOURCES) $(ARCH) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
endif

OBJECTS := \
	$(OBJDIR)/flash.o \
	$(OBJDIR)/rand.o \
	$(OBJDIR)/shared.o \
	$(OBJDIR)/g_ai.o \
	$(OBJDIR)/g_chase.o \
	$(OBJDIR)/g_cmds.o \
	$(OBJDIR)/g_combat.o \
	$(OBJDIR)/g_func.o \
	$(OBJDIR)/g_items.o \
	$(OBJDIR)/g_main.o \
	$(OBJDIR)/g_misc.o \
	$(OBJDIR)/g_monster.o \
	$(OBJDIR)/g_phys.o \
	$(OBJDIR)/g_spawn.o \
	$(OBJDIR)/g_svcmds.o \
	$(OBJDIR)/g_target.o \
	$(OBJDIR)/g_trigger.o \
	$(OBJDIR)/g_turret.o \
	$(OBJDIR)/g_utils.o \
	$(OBJDIR)/g_weapon.o \
	$(OBJDIR)/berserker.o \
	$(OBJDIR)/boss2.o \
	$(OBJDIR)/boss3.o \
	$(OBJDIR)/boss31.o \
	$(OBJDIR)/boss32.o \
	$(OBJDIR)/brain.o \
	$(OBJDIR)/chick.o \
	$(OBJDIR)/flipper.o \
	$(OBJDIR)/float.o \
	$(OBJDIR)/flyer.o \
	$(OBJDIR)/gladiator.o \
	$(OBJDIR)/gunner.o \
	$(OBJDIR)/hover.o \
	$(OBJDIR)/infantry.o \
	$(OBJDIR)/insane.o \
	$(OBJDIR)/medic.o \
	$(OBJDIR)/move.o \
	$(OBJDIR)/mutant.o \
	$(OBJDIR)/parasite.o \
	$(OBJDIR)/soldier.o \
	$(OBJDIR)/supertank.o \
	$(OBJDIR)/tank.o \
	$(OBJDIR)/client.o \
	$(OBJDIR)/hud.o \
	$(OBJDIR)/trail.o \
	$(OBJDIR)/view.o \
	$(OBJDIR)/weapon.o \
	$(OBJDIR)/savegame.o \

RESOURCES := \

SHELLTYPE := msdos
ifeq (,$(ComSpec)$(COMSPEC))
  SHELLTYPE := posix
endif
ifeq (/bin,$(findstring /bin,$(SHELL)))
  SHELLTYPE := posix
endif

.PHONY: clean prebuild prelink

all: $(TARGETDIR) $(OBJDIR) prebuild prelink $(TARGET)
	@:

$(TARGET): $(GCH) $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking quake2-game
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning quake2-game
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(GCH): $(PCH)
	@echo $(notdir $<)
	$(SILENT) $(CC) -x c-header $(ALL_CFLAGS) -MMD -MP $(DEFINES) $(INCLUDES) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
endif

$(OBJDIR)/flash.o: ../../../Sources/common/shared/flash.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/rand.o: ../../../Sources/common/shared/rand.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/shared.o: ../../../Sources/common/shared/shared.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_ai.o: ../../../Sources/game/g_ai.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_chase.o: ../../../Sources/game/g_chase.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_cmds.o: ../../../Sources/game/g_cmds.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_combat.o: ../../../Sources/game/g_combat.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_func.o: ../../../Sources/game/g_func.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_items.o: ../../../Sources/game/g_items.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_main.o: ../../../Sources/game/g_main.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_misc.o: ../../../Sources/game/g_misc.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_monster.o: ../../../Sources/game/g_monster.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_phys.o: ../../../Sources/game/g_phys.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_spawn.o: ../../../Sources/game/g_spawn.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_svcmds.o: ../../../Sources/game/g_svcmds.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_target.o: ../../../Sources/game/g_target.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_trigger.o: ../../../Sources/game/g_trigger.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_turret.o: ../../../Sources/game/g_turret.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_utils.o: ../../../Sources/game/g_utils.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/g_weapon.o: ../../../Sources/game/g_weapon.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/berserker.o: ../../../Sources/game/monster/berserker/berserker.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/boss2.o: ../../../Sources/game/monster/boss2/boss2.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/boss3.o: ../../../Sources/game/monster/boss3/boss3.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/boss31.o: ../../../Sources/game/monster/boss3/boss31.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/boss32.o: ../../../Sources/game/monster/boss3/boss32.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/brain.o: ../../../Sources/game/monster/brain/brain.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/chick.o: ../../../Sources/game/monster/chick/chick.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/flipper.o: ../../../Sources/game/monster/flipper/flipper.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/float.o: ../../../Sources/game/monster/float/float.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/flyer.o: ../../../Sources/game/monster/flyer/flyer.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/gladiator.o: ../../../Sources/game/monster/gladiator/gladiator.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/gunner.o: ../../../Sources/game/monster/gunner/gunner.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/hover.o: ../../../Sources/game/monster/hover/hover.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/infantry.o: ../../../Sources/game/monster/infantry/infantry.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/insane.o: ../../../Sources/game/monster/insane/insane.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/medic.o: ../../../Sources/game/monster/medic/medic.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/move.o: ../../../Sources/game/monster/misc/move.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/mutant.o: ../../../Sources/game/monster/mutant/mutant.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/parasite.o: ../../../Sources/game/monster/parasite/parasite.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/soldier.o: ../../../Sources/game/monster/soldier/soldier.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/supertank.o: ../../../Sources/game/monster/supertank/supertank.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/tank.o: ../../../Sources/game/monster/tank/tank.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/client.o: ../../../Sources/game/player/client.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/hud.o: ../../../Sources/game/player/hud.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/trail.o: ../../../Sources/game/player/trail.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/view.o: ../../../Sources/game/player/view.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/weapon.o: ../../../Sources/game/player/weapon.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

$(OBJDIR)/savegame.o: ../../../Sources/game/savegame/savegame.c
	@echo $(notdir $<)
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF $(@:%.o=%.d) -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif