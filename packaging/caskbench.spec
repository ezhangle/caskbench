#
Name:           caskbench
Summary:        Cairo and Skia Benchmark
Version:        0.3.9
Release:        1
Group:          Graphics & UI Framework/Testing
ExcludeArch:    i596
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
License:        GPL
URL:            http://www.x.org/
Source0:        %{name}-%{version}.tar.bz2
Source1001:     packaging/caskbench.manifest

Requires: xorg-x11-server-Xorg
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  rpm-devel
BuildRequires:  popt-devel
BuildRequires:  pkgconfig(fontconfig)
BuildRequires:  pkgconfig(freetype2)
BuildRequires:  pkgconfig(x11)
BuildRequires:  pkgconfig(opengl-es-20)
BuildRequires:  pkgconfig(gles20)
BuildRequires:  pkgconfig(cairo) >= 1.12.0
BuildRequires:  skia

%description
Cairo and Skia benchmark

%prep
%setup -q -n %{name}-%{version}

%build
mkdir build
cd build
export CFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$CFLAGS"

cmake $CMAKE_FLAGS \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DDOC_INSTALL_DIR=%{_docdir} \
    -DLIB=%{_lib} \
    -DCMAKE_VERBOSE_MAKEFILE=TRUE \
    -DCMAKE_BUILD_TYPE=Release \
    ..

make %{?jobs:-j %jobs} VERBOSE=1

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/share/license
cd build
make install DESTDIR=$RPM_BUILD_ROOT
cd ..
cp COPYING %{buildroot}/usr/share/license/%{name}


%clean
rm -rf "$RPM_BUILD_ROOT"

%files
%manifest packaging/caskbench.manifest
%defattr(-,root,root,-)
%{_bindir}/caskbench
/usr/share/license/%{name}

