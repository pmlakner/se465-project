
pipair:
	cd pi/partA && $(MAKE) clean && $(MAKE) && cp out ../../ && cp pipair ../../

clean:
	rm out; rm pipair; true

