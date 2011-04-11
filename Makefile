default: client-install
.PHONY: client client-clean clean client-install docs

client:
	cd Connector_HTTP; $(MAKE)
	cd Connector_RTMP; $(MAKE)
	cd Connector_RAW; $(MAKE)
	cd Buffer; $(MAKE)
client-clean:
	cd Connector_HTTP; $(MAKE) clean
	cd Connector_RTMP; $(MAKE) clean
	cd Connector_RAW; $(MAKE) clean
	cd Buffer; $(MAKE) clean
clean: client-clean
client-install: client-clean client
	cd Connector_RTMP; $(MAKE) install
	cd Connector_HTTP; $(MAKE) install
	cd Connector_RAW; $(MAKE) install
	cd Buffer; $(MAKE) install
docs:
	doxygen ./Doxyfile > /dev/null

