PDFTEX = pdflatex

DOT = dot
#BIBTEX = bibtex

#TEX_BIBS = int.bib
#TEX_BBL := $(patsubst %.bib, %.bbl, $(TEX_BIBS))

# idea from http://ctan.unsw.edu.au/help/uk-tex-faq/Makefile
%.pdf: %.aux
	while ($(PDFTEX) $(patsubst %.aux, %.tex, $<) && \
		grep -q "Rerun to get cross" $(patsubst %.aux, %.log, $<)) do true ; \
	done

%.png: %.dia
	dia -t png -e $@ $<

%.aux: %.tex
	$(PDFTEX) $<

%.bbl: %.bib
	$(BIBTEX) $*

%.eps: %.dot
	dot -Teps $< > $@

.PHONY: all
all: report.pdf

report.pdf: report.tex Makefile

.PHONY: clean
clean:
	rm *.aux *.log *.out *.bbl *.blg *.pdf

.PHONY: distclean
distclean: clean
	rm -rf report.pdf

