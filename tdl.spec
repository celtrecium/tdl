Name:           tdl
Version:        1.0.0
Release:        1%{?dist}
Summary:        A library for drawing with text using user-friendly API.

License:        GPLv2+
URL:            https://github.com/celtrecium/tdl
Source0:        tdl.tar.gz

BuildRequires:  gcc cmake

%description

%prep
%autosetup -n %name

%build
%cmake
%cmake_build

%install
%cmake_install

%files
/usr/include/*.h
/usr/include/%{name}/*.h

/usr/lib/debug/usr/lib/*.debug
/usr/lib/debug/usr/lib64/*.debug

/usr/lib/*.so
/usr/lib64/*.so

%changelog
%autochangelog
