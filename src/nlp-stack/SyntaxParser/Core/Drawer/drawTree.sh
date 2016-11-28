dot -Tpdf $1 -o tree.pdf
#dot -Kfdp -n -Tpng -o tree.png $1
okular tree.pdf
rm -f tree.pdf
