Name:           px5deditor
Version:        0.1.0
Release:        0%{?dist}
Summary:        A library and editor for the Korg Pandora PX5D on Linux
License:        GPL3
Group:          Audio/Multimedia
URL:            https://github.com/soulfish/px5deditor/
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildArch:      i386 x86_64

BuildRequires:  kernel-devel >= 3 cmake make qt-devel >= 4 alsa-lib-devel
Requires:       kernel >= 3 

%description
This Korg Pandora PX5D Editor for Linux is a simple, no-eye-candy preset editor
which allows to view all parameters of a preset on the PX5D and edit them in real time.

%prep
%setup -q 

%build
mkdir build
pushd build
%cmake ..
make

%install
rm -rf $RPM_BUILD_ROOT
pushd build
make install DESTDIR=$RPM_BUILD_ROOT

rm -f $RPM_BUILD_ROOT/%{_libdir}/forked-daapd/forked-daapd-sqlext.la

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_bindir}/*
%{_mandir}/man1/*
%{_prefix}/share/*

%changelog
* Sat Jun 22 2013 George Machitidze <giomac@gmail.com> - 0.1.0-0
- Initial release
