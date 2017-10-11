BUILD_TARGET=ntp

localhost: app.localhost

ar7: app.ar7

wp7: app.wp7

ar86: app.ar86

wp85: app.wp85

app.%:
	mkapp ntp.adef -t $* -v

clean:
	rm -rf _build_* *.update

install:
	app install $(BUILD_TARGET).wp85.update 192.168.2.2
start:
	app start $(BUILD_TARGET) 192.168.2.2
stop:	
	app stop $(BUILD_TARGET) 192.168.2.2
	
