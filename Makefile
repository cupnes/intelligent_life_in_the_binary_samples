fs/fs.img: fs.img
	cp $< $@

fs.img: init
	tools/create_fs.sh $+

init:
	make -C biobin_poc deploy DEPLOY_DIR=../ RUN_QEMU=true

run: fs/fs.img
	qemu-system-x86_64 -m 4G -enable-kvm \
	-drive if=pflash,format=raw,readonly,file=$(HOME)/OVMF/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=$(HOME)/OVMF/OVMF_VARS.fd \
	-drive dir=fs,driver=vvfat,rw=on

clean:
	make -C biobin_poc clean
	rm -f *~ *.img init fs/*~ fs/*.img  tools/*~

.PHONY: run clean
