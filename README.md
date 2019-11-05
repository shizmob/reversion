# THE PROBLEM

macOS has traditionally shipped OpenSSL 0.9.x-compatible versions of two system libraries: `libcrypto.dylib` and `libssl.dylib`.
As starting from macOS Catalina they have switched to LibreSSL, this represents an ABI break. This is a problem!

~~For god knows whatever reason~~ they decided to, instead of symlinking the version-less version to the OpenSSL 0.9.x compatible version they already ship,
to 'boobytrap' the version-less libraries, causing your process to abort when requested with the following message:

```
/usr/lib/libcrypto.dylib
abort() called
Invalid dylib load. Clients should not load the unversioned libcrypto dylib as it does not have a stable ABI.
```

This, of course, assumes everyone can re-compile every single application and/or dependency on their system to do this. This is for the people that can't.

# THE SOLUTION

No problem has ever not been solvable by hooking stuff. We use [rd_route](https://github.com/rodionovd/rd_route) to hook `dlopen()` and redirect the version-less loads to the 0.9.x-compatible ones, `libcrypto.0.9.8.dylib` and `libssl.0.9.8.dylib`. Applications that use the version-less versions would expect this old ABI anyway.


# USING IT

**DON'T USE THIS IN PRODUCTION STUFF IF YOU'RE NOT 100% SURE YOUR APPLICATION TARGETS THE OPENSSL 0.9.8 ABI, IT MAY CAUSE SECURITY LEAKS AND OTHER NASTY STUFF. YOU HAVE BEEN WARNED.**

```
make
export DYLD_INSERT_LIBRARIES=$(pwd)/reversion.dylib
```

After this you can run your old legacy/proprietary/un-updated applications from that terminal session as usual.

# LIMITATIONS

Doesn't work for sudo stuff, as `DYLD_*` variables will be cleaned out. You'll have to find a way to load `reversion.dylib` into the process manually prior to the dynamic load of the dependency. For Python code, you can e.g. do this with:

```python
import ctypes
_ = ctypes.CDLL('/path/to/reversion.dylib')
```

If your target application uses a direct dependency instead of `dlopen()`, you can fix this issue by using `install_name_tool`:

```sh
install_tool -change /usr/lib/libcrypto.dylib /usr/lib/libcrypto.0.9.8.dylib my-proprietary-binary
```

# LICENSE

lol WTFPL
