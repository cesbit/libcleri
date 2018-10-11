# Install dependencies
```
sudo apt-get install libpcre2-8-0
```

# Running tests
```
make --directory ./Release test
```

# Build
```
make --directory ./Release clean
make --directory ./Release
```

# Install
```
# You might want to uninstall first...
sudo make --directory ./Release uninstall
sudo make --directory ./Release install
```

# Building deb package
Make sure the required tools are installed
```
sudo apt-get install devscripts lintian
```

When symbols are changed:
```
dpkg-gensymbols -eRelease/libcleri.so -plibcleri0
```

In case of a new package, update the changelog.
Skip this step if you just want to rebuild the current deb version.
```
debchange
git commit -am 'Update changelog'
```

Create archive from code
```
git archive -o ../libcleri_0.10.0.orig.tar.gz master
```

Build deb package
```
debuild -us -uc
```



