TEX=pdflatex
BIB=bibtex

FLG=-shell-escape -synctex=1

figs = $(wildcard figs/*.pdf)

#make the pdf
all:  NSCpp.pdf

#run pdflatex and bibtex (runs only if you update something in pdfs or in the tex file)
NSCpp.pdf:  NSCpp.tex $(figs) refs.bib
	$(TEX) $(FLG) $< 
	$(TEX) $(FLG) $< 
	$(BIB) NSCpp || true
	$(TEX) $(FLG) $< 
	$(TEX) $(FLG) $< 

	#remove all output except the pdf file
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.toc 
	rm -f *.blg 
clean:
	rm -f NSCpp.pdf 
	rm -f NSCpp.synctex.gz
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.blg 
	rm -f *.toc 
	rm -rf *.run
	
