TEX=pdflatex
BIB=bibtex

FLG=-shell-escape

figs = $(wildcard figs/*.pdf)

#make the pdf
all:  NCSpp.pdf

#run pdflatex and bibtex (runs only if you update something in pdfs or in the tex file)
NCSpp.pdf:  NCSpp.tex $(figs) refs.bib
	$(TEX) $(FLG) $< 
	$(TEX) $(FLG) $< 
	$(BIB) NCSpp || true
	$(TEX) $(FLG) $< 
	$(TEX) $(FLG) $< 

	#remove all output except the pdf file
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.toc 
	rm -f *.blg 
	rm -rf _minted-NCSpp
clean:
	rm -f NCSpp.pdf 
	rm -f NCSpp.pdf
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.blg 
	rm -f *.toc 
	rm -rf _minted-NCSpp
	
