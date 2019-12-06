TARGET = init
CFLAGS = -Wall -Wextra -nostdinc -nostdlib -fno-builtin -fno-common	\
	 -Iinclude -fPIE
LDFLAGS = -Map app.map -s -x -T apps.ld -pie
OBJS = main.o lives/incrementer.o cell.o protein.o compound.o lib.o
DEPLOY_DIR ?= ../fs
ifdef RUN_QEMU
	CFLAGS += -DRUN_QEMU
endif
ifdef NO_GRAPHIC
	QEMU_ADDITIONAL_ARGS += --nographic
endif
ifdef SMP
	QEMU_ADDITIONAL_ARGS += -smp ${SMP}
endif

$(TARGET): $(OBJS)
	ld $(LDFLAGS) -o $@ $+

.o: %.c
	gcc $(CFLAGS) -c -o $@ $<
%.o: %.s
	gcc $(CFLAGS) -c -o $@ $<

deploy: $(TARGET)
	cp $(TARGET) $(DEPLOY_DIR)

unit_deploy: $(TARGET)
	cp $< init
	../tools/create_fs.sh init
	cp fs.img $(DEPLOY_DIR)

run: unit_deploy
	qemu-system-x86_64 -m 4G -enable-kvm \
	-drive if=pflash,format=raw,readonly,file=$(HOME)/OVMF/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=$(HOME)/OVMF/OVMF_VARS.fd \
	-drive dir=$(shell echo $(DEPLOY_DIR)),driver=vvfat,rw=on \
	$(QEMU_ADDITIONAL_ARGS)

clean:
	rm -f *~ *.o *.map *.img $(TARGET) init lives/*~ lives/*.o include/*~

.PHONY: clean deploy unit_deploy run
