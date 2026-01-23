# What is ETKSQLite3?
<b>ETKSQLite3</b> is a C++ wrapper around the public domain written in C++ that allow to access SQLite3 database using the wxSQLite3 wrapper library.
This library is used to allow user to quickly make database binding and make easy request with C++ operators and is specifically designed for use in programs based on the wxWidgets library. A workaround has been made to make it works with QT Library or C++ STL.

Since <b>SQLite3</b> stores strings in UTF-8 encoding, the <b>wxSQLite3</b> methods provide automatic conversion between wxString and UTF-8 strings.
The methods ToUTF8 and FromUTF8 of the wxString class (available since <b>wxWidgets</b> 2.8.4) are used for the conversion. Special care has to be taken if external administration tools are used to modify the database contents, since not all of these tools operate in Unicode or UTF-8 mode.

## Version history
- **1.2** - September 2025.
  - Add <b>STL</b> Integration. <b>ETKSQLite3</b> allowed to be used with C++ <b>STL</b> library.
    All needed wxWidgets classes are re-written around Standard Template Libraries. A careful must be done with UTF-8 that are not fully supported like with <b>QT</b> and <b>wxWidgets</b> (UNICODE).
- **1.1** - July 2012.
  - Add <b>QT</b> Integration. <b>ETKSQLite3</b> allowed to be used with <b>QT</b> library.
    All needed wxWidgets classes are re-written around QT library classes (based on macros or directly by implementing these wxWidgets classes by deriving QT ones).
- **1.0** - December 2011
  - First release - Not published.

**Author**
Stéphane Château (Feneck91)

# Compilation
[ETKSQLite3 Compilation](README_Compilation.md)

# Acknowledgements
Kudos to Ulrich Telle (and all wxSQLite3 contributors) for developing the wxSQLite3 library used by ETKSQLite3.
