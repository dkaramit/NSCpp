TEX=pdflatex
BIB=bibtex


#make the pdf
all:  response.pdf

#run pdflatex and bibtex (runs only if you update something in pdfs or in the tex file)
response.pdf:  response.tex 
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
	rm -f response.pdf 
	rm -f response.synctex.gz
	rm -f *.aux 
	rm -f *.log 
	rm -f *.out 
	rm -f *.bbl 
	rm -f *.blg 
	rm -f *.toc 