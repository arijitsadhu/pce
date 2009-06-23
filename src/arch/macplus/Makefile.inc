# src/arch/macplus/Makefile.inc

rel := src/arch/macplus

DIRS += $(rel)
DIST += $(rel)/Makefile.inc

DIST += $(rel)/pcex/Makefile $(rel)/pcex/pcex.S

# ----------------------------------------------------------------------

PCE_MACPLUS_CFLAGS  := $(CFLAGS_DEFAULT)
PCE_MACPLUS_LDFLAGS := $(LDFLAGS_DEFAULT)
PCE_MACPLUS_LIBS    := $(LIBS)

ifeq "$(PCE_SDL_USE)" "1"
	PCE_MACPLUS_CFLAGS += $(PCE_SDL_CCF)
	PCE_MACPLUS_LIBS += $(PCE_SDL_LDF)
endif

ifeq "$(PCE_X11_USE)" "1"
	PCE_MACPLUS_CFLAGS += $(PCE_X11_CCF)
	PCE_MACPLUS_LIBS += $(PCE_X11_LDF)
endif

ifeq "$(PCE_HOST_WINDOWS)" "1"
	PCE_MACPLUS_LIBS += -mconsole
endif

$(rel)/%.o: $(rel)/%.c
	$(QP)echo "  CC     $@"
	$(QR)$(CC) -c $(PCE_MACPLUS_CFLAGS) -o $@ $<

# ----------------------------------------------------------------------

PCE_MACPLUS_BAS := \
	cmd_68k \
	hook \
	iwm \
	keyboard \
	macplus \
	main \
	mem \
	msg \
	rtc \
	scsi \
	serial \
	sony \
	traps \
	video

PCE_MACPLUS_SRC  := $(foreach f,$(PCE_MACPLUS_BAS),$(rel)/$(f).c)
PCE_MACPLUS_OBJ  := $(foreach f,$(PCE_MACPLUS_BAS),$(rel)/$(f).o)
PCE_MACPLUS_HDR  := $(foreach f,$(PCE_MACPLUS_BAS),$(rel)/$(f).h)
#PCE_MACPLUS_MAN1 := $(rel)/pce-macplus.1
PCE_MACPLUS_ETC  := $(rel)/pce-macplus.cfg
PCE_MACPLUS_BIN  := $(rel)/pce-macplus

PCE_MACPLUS_OBJ_EXT := \
	src/cpu/e68000/e68000.a \
	src/chipset/82xx/e8250.o \
	src/chipset/e6522.o \
	src/chipset/e8530.o \
	src/devices/device.o \
	src/devices/memory.o \
	src/drivers/char/char.a \
	src/lib/brkpt.o \
	src/lib/cmd.o \
	src/lib/console.o \
	src/lib/hexdump.o \
	src/lib/inidsk.o \
	src/lib/iniram.o \
	src/lib/initerm.o \
	src/lib/ihex.o \
	src/lib/srec.o \
	src/lib/load.o \
	src/lib/log.o \
	src/lib/monitor.o \
	src/lib/msg.o \
	src/lib/path.o \
	src/lib/string.o \
	src/lib/sysdep.o \
	src/libini/libini.a \
	$(DEV_BLK_OBJ) \
	$(DRV_TRM_OBJ)

ifeq "$(PCE_BUILD_MACPLUS)" "1"
BIN  += $(PCE_MACPLUS_BIN)
ETC  += $(PCE_MACPLUS_ETC)
#MAN1 += $(PCE_MACPLUS_MAN1)
endif

CLN  += $(PCE_MACPLUS_BIN) $(PCE_MACPLUS_OBJ) $(PCE_MACPLUS_ETC)
DIST += $(PCE_MACPLUS_SRC) $(PCE_MACPLUS_HDR) $(PCE_MACPLUS_MAN1)
DIST += $(rel)/macplus.cfg.in

$(rel)/cmd_68k.o:	$(rel)/cmd_68k.c
$(rel)/hook.o:		$(rel)/hook.c
$(rel)/iwm.o:		$(rel)/iwm.c
$(rel)/keyboard.o:	$(rel)/keyboard.c
$(rel)/macplus.o:	$(rel)/macplus.c
$(rel)/main.o:		$(rel)/main.c
$(rel)/mem.o:		$(rel)/mem.c
$(rel)/msg.o:		$(rel)/msg.c
$(rel)/rtc.o:		$(rel)/rtc.c
$(rel)/scsi.o:		$(rel)/scsi.c
$(rel)/serial.o:	$(rel)/serial.c
$(rel)/sony.o:		$(rel)/sony.c
$(rel)/traps.o:		$(rel)/traps.c
$(rel)/video.o:		$(rel)/video.c

$(rel)/pce-macplus: $(PCE_MACPLUS_OBJ_EXT) $(PCE_MACPLUS_OBJ)
	$(QP)echo "  LD     $@"
	$(QR)$(LD) $(PCE_MACPLUS_LDFLAGS) -o $@ $(PCE_MACPLUS_OBJ) $(PCE_MACPLUS_OBJ_EXT) $(PCE_MACPLUS_LIBS) -lm

$(rel)/pce-macplus.cfg: $(rel)/macplus.cfg.in
	$(QP)echo "  SED    $@"
	$(QR)sed -e "s]PCE_DIR_DATA]$(datadir)]g" < $< > $@