TARGET = fs.img
WORK_DIR = fs
FILES = init
# FILES = init urclock bg.bgra lsbg.bgra urclockbg.bgra
# FILES += e.auto_slide e.bio_mid
# FILES += $(notdir $(shell ls $(WORK_DIR)/s.* 2>/dev/null || true))
# FILES += $(notdir $(shell ls $(WORK_DIR)/i.* 2>/dev/null || true))
# FILES += $(notdir $(shell ls $(WORK_DIR)/b* 2>/dev/null || true))
FS_DIR ?= ../fs
ifdef NO_GRAPHIC
	QEMU_ADDITIONAL_ARGS += --nographic
endif
ifdef SMP
	QEMU_ADDITIONAL_ARGS += -smp ${SMP}
endif

$(WORK_DIR)/$(TARGET): $(WORK_DIR) $(addprefix $(WORK_DIR)/, $(FILES))
	cd $(WORK_DIR) && ../tools/create_fs.sh $(FILES)

$(WORK_DIR):
	mkdir -p $(WORK_DIR)

$(WORK_DIR)/init:
	make -C biobin_poc deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true
	# make -C adv_if deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR)/urclock:
	make -C urclock deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR)/e.auto_slide:
	make -C auto_slideshow deploy DEPLOY_DIR=../$(WORK_DIR) RUN_QEMU=true

$(WORK_DIR)/e.bio_mid:
	make -C bio_middle RUN_QEMU=true DEPLOY_DIR=../$(WORK_DIR) deploy

deploy: $(WORK_DIR)/$(TARGET)
	cp $< $(FS_DIR)

run: deploy
	qemu-system-x86_64 -m 4G -enable-kvm \
	-drive if=pflash,format=raw,readonly,file=$(HOME)/OVMF/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=$(HOME)/OVMF/OVMF_VARS.fd \
	-drive dir=$(FS_DIR),driver=vvfat,rw=on \
	$(QEMU_ADDITIONAL_ARGS)

clean:
	make -C biobin_poc clean
	# make -C adv_if clean
	# make -C urclock clean
	# make -C bio_middle clean
	rm -f *~ $(WORK_DIR)/*~ $(WORK_DIR)/init $(WORK_DIR)/urclock	\
		$(WORK_DIR)/e.bio_mid $(WORK_DIR)/e.auto_slide		\
		$(WORK_DIR)/$(TARGET)
	rm -f tools/*~

.PHONY: deploy run clean
