readelf -s --dyn-syms $1 | grep UND | grep -v GLIBC
