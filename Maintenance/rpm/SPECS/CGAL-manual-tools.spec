%{!?cgal_manual_revision: %define cgal_manual_revision 36165}

Name:           CGAL-manual-tools
Version:        %{cgal_manual_revision}
Release:        1%{?dist}
Summary:        CC Manual Style and LaTeX Converter for HTML

Group:          Development/Tools
License:        Non-distributable
URL:            http://www.cgal.org/Members/Manual_tools/
Source0:        Manual_tools-%{version}.tar.gz
Source1:        Manual-%{version}.tar.gz
Patch0:         CGAL_manual_tools-config.patch
Patch1:         CGAL_manual_tools-rpm.patch
Patch2:         CGAL_manual_tools-perl.patch
Patch3:         CGAL_manual_tools-cgal_manual.patch
Patch4:         CGAL_manual_tools-cc_extract_html.patch
Patch5:         CGAL_manual_tools-latex_to_html.patch
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: bison flex
BuildRequires:  /usr/bin/kpsewhich
Requires:       tetex-latex tetex-dvips 
Requires:       ghostscript >= 6.0
Requires(post):   /usr/bin/texhash
Requires(postun): /usr/bin/texhash

%description
Specification and Manual Writing Tools for C++ Reference Manuals

%prep
%setup -q -n Manual_tools -a 1
%patch0 -p0
%patch1 -p0
%patch2 -p0
%patch3 -p0
%patch4 -p0
%patch5 -p0

%build
source install.config
make -C src LATEX_CONV_INPUTS=$LATEX_CONV_INPUTS \
            CXXFLAGS="${CXXFLAGS:-%optflags}" || exit 1

%install
rm -rf $RPM_BUILD_ROOT
./install.sh
[ -d $RPM_BUILD_ROOT/usr/share/texmf/tex/latex/CGAL ] || mkdir -p $RPM_BUILD_ROOT/usr/share/texmf/tex/latex/CGAL
cp -r doc_tex/Manual $RPM_BUILD_ROOT/usr/share/texmf/tex/latex/CGAL
cp doc_tex/ipe.sty $RPM_BUILD_ROOT/usr/share/texmf/tex/latex/CGAL
[ -d $RPM_BUILD_ROOT/usr/share/texmf/bibtex/bib/CGAL ] || mkdir -p $RPM_BUILD_ROOT/usr/share/texmf/bibtex/bib/CGAL/Manual
mv $RPM_BUILD_ROOT/usr/share/texmf/tex/latex/CGAL/Manual/*.bib $RPM_BUILD_ROOT/usr/share/texmf/bibtex/bib/CGAL/Manual
cp developer_scripts/cgal_manual developer_scripts/bibmerge $RPM_BUILD_ROOT/usr/bin/ 

%post
texhash > /dev/null 2>&1 || :

%postun
texhash > /dev/null 2>&1 || :

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc doc_ps/*
/usr/bin/*
/usr/share/texmf/tex/latex/CGAL
/usr/share/texmf/bibtex/bib/CGAL
%dir /usr/share/CGAL/
/usr/share/CGAL/latex_conv_config

%changelog
* Sat Feb 10 2007 Laurent Rineau <laurent.rineau__fedora_extras@normalesup.org> - 36165-1
- Updated to revision 36165.

* Tue Jul  4 2006 Laurent Rineau <laurent.rineau__fedora_extras@normalesup.org> - 32190-1
- Updated to revision 32190.
- Added an optionnal macro %%cgal_manual_revision, that can be used at compile time, to define another revision. For example:
  rpmbuild --define "%%cgal_manual_revision 32195"

* Thu May 11 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 31112-1
- Update to revision 31112.
- Remove BuildRequires: sed.
- Change License:, to remove a rpmlint warning.

* Wed Apr 26 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30764-1
- Updated to revision 30764.

* Mon Apr 19 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30346-1
- Updated to revision 30675.

* Thu Apr 13 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30269-2
- Fix the prep part: no need for two setup macros.

* Wed Apr 12 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30269-1
- Updated to revision 30269.
- Fixed the warnings (included twice, etc.) of rpmbuild.
- Passed rpmlint.
- Updated patch5 to new revision.
- Changed my email address.

* Tue Apr 11 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30224
- Updated to revision 30224.

* Fri Apr  7 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30090
- Updated to revision 30090.

* Thu Apr  6 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30080
- Updated to revision 30080.

* Wed Apr  5 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 30025
- Updated to revision 30025.
- Source0 and Source1 now have %%version in their names.

* Tue Apr  4 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 29933
- Updated to revision 29933.

* Mon Apr  3 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 29922
- Updated to revision 29922.

* Fri Mar 31 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 29890
- Updated to 29890.
- New patch, for latex_to_html
- Updated the CGAL_manual_tools-config.patch, to add
  $LATEX_CONV_CONFIG/html to $LATEX_CONV_INPUTS
- New patch, for cc_extract_html.C. Will up submitted upstream soon.
- Added ipe.sty to files
- Move bib files to the bibtex installation.

* Thu Mar 30 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 29841
- Updated to rev. 29841. No longer need for patch2
- Specify the two scripts of developer_scripts/ that need to be installed,
to avoid things like cgal_manual.orig!

* Tue Mar  7 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 29111
- Updated to Manual_tools rev 29111., Manual rev 29112.
- Manual_tools are now pulled from SVN too.
- Some patches are now included in SVN.

* Fri Mar  3 2006 Laurent Rineau <laurent.rineau__fc_extra@normalesup.org> - 3.12
- Added cgal_manual and dependancies
- Added a call to texhash

