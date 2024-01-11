#!/usr/bin/sh

# for convinience create a link in project root folder: 
# ln -s scripts/build.sh run

. scripts/misc/terminal

clear=clear

NAME=efl2
UNAME=EFL
BD=build

get_os() {
    if [ -f /etc/os-release ]; then
        # freedesktop.org and systemd
        . /etc/os-release
        OS=$NAME
        VER=$VERSION_ID
    elif type lsb_release >/dev/null 2>&1; then
        # linuxbase.org
        OS=$(lsb_release -si)
        VER=$(lsb_release -sr)
    elif [ -f /etc/lsb-release ]; then
        # For some versions of Debian/Ubuntu without lsb_release command
        . /etc/lsb-release
        OS=$DISTRIB_ID
        VER=$DISTRIB_RELEASE
    elif [ -f /etc/debian_version ]; then
        # Older Debian/Ubuntu/etc.
        OS=Debian
        VER=$(cat /etc/debian_version)
    elif [ -f /etc/SuSe-release ]; then
        # Older SuSE/etc.
        OS=SuSe
        ...
    elif [ -f /etc/redhat-release ]; then
        # Older Red Hat, CentOS, etc.
        OS=RedHat
        ...
    else
        # Fall back to uname, e.g. "Linux <version>", also works for BSD, etc.
        OS=$(uname -s)
        VER=$(uname -r)
    fi                                          
}

install_deps_debian()
{
    echo_header "Installing common EFL dependencies on Debian"
    sudo apt-get install -y \
            build-essential check meson ninja-build autoconf automake autopoint \
            binutils ccache dbus-x11 doxygen faenza-icon-theme gettext git imagemagick \
            libbrotli-dev libasound2-dev libavahi-client-dev libblkid-dev libbluetooth-dev \
            libbullet-dev libc6-dev libcogl-dev libcurl4-openssl-dev libdbus-1-dev \
            libfontconfig1-dev libfreetype6-dev libfribidi-dev libgif-dev libgl1-mesa-dev \
            libglib2.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libharfbuzz-dev \
            libibus-1.0-dev libiconv-hook-dev libjpeg-dev \
            luajit libluajit-5.1-dev liblua5.2-dev liblz4-dev libmount-dev libopenjp2-7-dev \
            libpam0g-dev libpcre3-dev libpng-dev libpoppler-cpp-dev libpoppler-dev libpoppler-private-dev \
            libproxy-dev libpulse-dev libraw-dev librsvg2-dev libscim-dev libsndfile1-dev libspectre-dev \
            libssl-dev libsystemd-dev libtiff5-dev libtool libudev-dev libunwind-dev libudisks2-dev \
            libunibreak-dev libwebp-dev libxcb-keysyms1-dev libx11-dev libxcomposite-dev libxcursor-dev \
            libxdamage-dev libxext-dev libxinerama-dev libxkbfile-dev libxrandr-dev libxrender-dev \
            libxss-dev libxtst-dev python3-pip python3-setuptools systemd libxpresent-dev \
            libsdl2-dev cmake libinput-dev libheif-dev librlottie-dev libxkbcommon-x11-dev valgrind \
            libvncserver-dev libexif-dev xwayland libhyphen-dev
}

install_deps_show_header()
{
    echo_header "Installing EFL dependencies for" $OS $VER
}

install_deps()
{
    get_os
    
    if [ $OS = "Ubuntu" ]; then
        install_deps_debian
        if [ $VER = "20.04" ]; then
            install_deps_show_header
            sudo apt-get install -y linux-tools-common libjpeg-turbo8-dev
        elif [ $VER = "22.04" ]; then
            install_deps_show_header
            sudo apt-get install -y linux-tools-common libjpeg-turbo8-dev libavif-dev libhwy-dev libllvm-14-dev
        elif [ $VER = "23.04" ]; then
            install_deps_show_header
        fi
    fi  

    if [ $OS = "Kali GNU/Linux" ]; then
        install_deps_debian
        if [ $VER = "2023.1" ]; then
            install_deps_show_header
            sudo apt-get install -y libavif-dev libhwy-dev
        fi
    fi
}

remove_build_dir()
{
    [ -d ${BD} ] && rm -rf $BD
}

meson_setup_default()
{
    time -p -f "Execution time: %e sec" meson setup . $BD

    # default [3032]: 43-44 sec
    # add -Dembedded-lz4=false [3029]: 27-31 sec
    # add -Dbuild-tests=false [2424]: 26-27 sec
    # add -Dbindings='' [2043]: 22-23 sec
}

meson_setup_dev()
{
    meson setup . $BD \
        -Dcrypto=openssl \
        -Daudio=true \
        -Davahi=false \
        -Dx11=true \
        -Dfb=false \
        -Dsdl=false \
        -Dwl=true \
        -Dbuffer=false \
        -Ddrm=true \
        -Dcocoa=false \
        -Dphysics=true \
        -Deeze=true \
        -Deina-magic-debug=true \
        -Ddebug-threads=false \
        -Dbuild-examples=false \
        -Dbuild-tests=false \
        -Dglib=true \
        -Dg-mainloop=false \
        -Dsystemd=false \
        -Dpulseaudio=true \
        -Dnetwork-backend=none \
        -Dxpresent=false \
        -Dxinput2=true \
        -Dxinput22=true \
        -Dtslib=false \
        -Deject-path=detect \
        -Dmount-path=detect \
        -Dunmount-path=detect \
        -Dharfbuzz=true \
        -Dfribidi=false \
        -Dfontconfig=true \
        -Dedje-sound-and-video=true \
        -Dpixman=false \
        -Dhyphen=false \
        -Dembedded-lz4=false \
        -Dlibmount=true \
        -Dvnc-server=false \
        -Dv4l2=true \
        -Dwl-deprecated=false \
        -Ddrm-deprecated=false \
        -Dnls=true \
        -Dmono-beta=false \
        -Dnative-arch-optimization=true \
        -Ddictionaries-hyphen-dir=/usr/share/hyphen/ \
        -Delementary-base-dir=.elementary \
        -Dinstall-eo-files=false

        # -Devas-modules=shared \
        # -Dxgesture=false \
        # -Delogind=false \
        # -Demotion-loaders-disabler=gstreamer,gstreamer1,libvlc \
        # -Demotion-generic-loaders-disabler=vlc \  
        # -Devas-loaders-disabler=json,xcf,dds,tga,raw \
        # -Decore-imf-loaders-disabler=ibus,scim,xim \
}

meson_setup_dev_all()
{
    # -Db_sanitize='address'
    time -p -f "Execution time: %e sec" \
    meson setup . $BD \
        -Db_lto=false \
        -Dbuildtype=debug \
        -Dwarning_level=2 \
        -Dbuild-tests=true \
        -Davahi=true \
        -Dfb=true \
        -Dsdl=true \
        -Dwl=true \
        -Dbuffer=true \
        -Ddrm=true \
        -Dphysics=true \
        -Dxpresent=true \
        -Devas-loaders-disabler=jxl \
        -Dpixman=true \
        -Dhyphen=true \
        -Dvnc-server=true \
        -Delua=true \
        -Dbindings=lua,cxx \
        -Dembedded-lz4=false \
        -Dbuild-examples=false
}

meson_compile()
{
    time -p -f "Execution time: %e sec" meson compile -C $BD
}

meson_install()
{
    echo_header "Install $UNAME"
    time -p -f "Execution time: %e sec" meson install -C $BD
}

meson_uninstall()
{
    if [ -d ${BD} ]; then 
        sudo ninja -C $BD uninstall
    else
        echo "Nothing to be done! At first build and install it by './run c && ./run i'"
    fi
}

meson_test()
{
    if [ -d ${BD} ]; then 
        meson test -C $BD # be sure that build-tests is true in meson options
    else
        echo "Nothing to be done! Needs to be compiled by './run c'"
    fi
}

tests_show_todo()
{
    if [ "$NAME" = "efl" ]; then
        for f in `find src/tests -name "*.c"`; do
            TODO=`awk '/TODO_TESTS/' $f`
            if ! $TODO; then
                printf "Filename: $f:\n"
                printf "$TODO;\n\n"
            fi
        done
    else
        eprint "ERR" "This command works only for EFL package!\n"
    fi
}

case "$1" in
    d|deps)
        install_deps
        ;;
    s|setup)
        remove_build_dir
        if [ "$2" = "dev" ]; then
            meson_setup_dev
        else
            meson_setup_default
        fi
        ;;
    c|compile)
        remove_build_dir
        if [ "$2" = "dev" ]; then
            meson_setup_dev_all
        else
            meson_setup_default
        fi
        meson_compile
        ;;
    cc|continue)
        eprint "WRN" "Continue $UNAME compilation"
        meson_compile
        ;;
    b|benchmark)
        ninja -C $BD benchmark
        ;;
    dt|dist)
        ninja -C $BD dist
        ;;
    i|install)
        meson_install
        ;;
    u|uninstall)
        meson_uninstall
        ;;
    r|run)
        exec $BD/src/bin/$NAME
        ;;
    cl|clean)
        declare -r -l conf_dir=~/.config/$NAME
        if [ -d $conf_dir ]; then
          rm -rf ~/.config/$NAME
          eprint "MSG" "Config folder $conf_dir removed\n" 
        else
          ninja -C $BD clean
          eprint "MSG" "Build folder '$BD' cleaned\n" 
        fi
        ;;
    t|test)
        meson_test
        ;;
    td|todo)
        tests_show_todo
        ;;
    co|cocci)
        declare -l -r coc=./scripts/coccinelle
        if [ -d ${coc} ]; then
            exec ./scripts/coccinelle/coccicheck.sh
        else
            eprint "ERR" "Coccicheck scripts folder not found!\n"
        fi
        ;;
    cv|coverage)
        remove_build_dir
        meson setup . $BD -Db_coverage=true
        meson_compile
        meson_test
        ninja coverage-text -C $BD
        # ninja coverage-html -C $BD
        ;;
    po|translation)
        ./po/potfiles-update.sh
        meson compile -C $BD $NAME-pot
        meson compile -C $BD $NAME-update-po
        ;;
    do|doc)
        meson compile -C $BD doc
        ;;
    f|format)
        ninja -C $BD clang-format
        ;;
    ts|timestamp)
        find . -exec touch {} \;
        ;;
    *)
        echo "Usage: $0 [cmd]"
        echo "     d|deps         install $UNAME and E dependencies"
        echo "     s|setup        setup meson build system"
        echo "     c|compile      compile $UNAME using meson"
        echo "     b|benchmark    run $UNAME benchmark suite"
        echo "    cc|continue     continue stopped compilation"
        echo "    dt|dist         make $UNAME distribution package"
        echo "     i|install      install $UNAME"
        echo "     u|uninstall    uninstall $UNAME"
        echo "     r|run          run local executable ($BD/src/bin/$NAME)"
        echo "    cl|clear        clean $BD dir"
        echo "     t|test         build and run tests"
        echo "    td|todo         show test's todo list"
        echo "    co|cocci        run coccinelle analysis"
        echo "    cv|coverage     build coverage report"
        echo "    po|translation  build (update) localization files"
        echo "    do|doc          build doc (in $BD dir)"
        echo "     f|format       format all code with clang-format"
        echo "    ts|timestemp    update access time of each file"
        ;;
esac