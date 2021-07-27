#make the examples
all: Cosmo/Cosmo.run NSC/NSC.run

Cosmo/Cosmo.run:
	cd Cosmo && $(MAKE)

NSC/NSC.run:
	cd NSC && $(MAKE)

clean:
	rm -f Cosmo/*.run
	rm -f NSC/*.run
