#!/bin/sh

source install.config

install -d $LATEX_CONV_BIN $LATEX_CONV_CONFIG $STYLE_FILES $STYLE_FILES/eps_tabs $STYLE_FILES/eps_tabs_grey

cd src
make cleanall
make || exit 1
make install
cd ..

cp scripts/index_fix scripts/cc_make_ref_pages scripts/cc_ref_wizard $LATEX_CONV_BIN
cp sty/*.sty $STYLE_FILES
cp sty/eps_tabs/*.pdf sty/eps_tabs/*.eps $STYLE_FILES/eps_tabs
cp  sty/eps_tabs_grey/*.eps $STYLE_FILES/eps_tabs_grey # sty/eps_tabs_grey/*.pdf

cd src
make cleanall
cd ..

echo ""
echo "================================================"
echo "Manual_tools successfullly installed. Do not forget to update \$TEXINPUTS:"
echo "export TEXINPUTS=\".:$STYLE_FILES:\$TEXINPUTS\""