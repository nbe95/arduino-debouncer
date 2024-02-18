tests:
	set -e; \
	for i in test-*/Makefile; do \
		echo '==== Making:' $$(dirname $$i); \
		find ../ -regex '.+\.\(cpp\|h\)' -exec cp "{}" "$$(dirname $$i)" \; ; \
		$(MAKE) -C $$(dirname $$i); \
	done

runtests:
	set -e; \
	for i in test-*/Makefile; do \
		echo '==== Running:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i) run; \
	done

clean:
	set -e; \
	for i in test-*/Makefile; do \
		echo '==== Cleaning:' $$(dirname $$i); \
		find -regex '.+\.\(cpp\|h\)' -delete; \
		$(MAKE) -C $$(dirname $$i) clean; \
	done
