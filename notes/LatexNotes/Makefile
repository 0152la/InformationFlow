notes.pdf: notes.tex notes.bib
	pdflatex notes.tex
	bibtex notes
	pdflatex notes.tex
	pdflatex notes.tex

clean:
	rm -f notes.aux notes.bbl notes.pdf notes.blg notes.log notes.out notes.fdb_latexmk notes.fls notes.synctex.gz texput.log
