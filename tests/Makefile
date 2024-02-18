tests:
	set -e; \
	for i in test-*/Makefile; do \
		echo '==== Making:' $$(dirname $$i); \
		cp ../debouncer.* $$(dirname $$i); \
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
		rm $$(dirname $$i)/debouncer.* &> /dev/null || true; \
		$(MAKE) -C $$(dirname $$i) clean; \
	done
