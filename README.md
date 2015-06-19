# scribus-plugin-API

A stable, high level and documented interface to the Scribus core that plugin developers can use to access the Scribus features.

## Install

Clone the `scribus-plugin-API` repository and put – or symlink – its `src` directory into `scribus/plugins/` as `scribusAPI`. Then add 

    ADD_SUBDIRECTORY(scribusAPI)

to `CMakeList.txt` file in `scribus/plugins/`.

## Roadmap

- [x] setup the plugin
- [ ] activate a first class and method and try to call it from another plugin
- [ ] use namespaces
- [ ] setup a documentation framework (probably QDoc)
- [ ] move the epub's plugin `ScribusDocument` class to the `ScribusAPI`
- [ ] make more publicity for this project and find out how to improve its architecture / add more coverage of Scribus features.

## Notes

- creating a doc with qdoc:
  - <https://doc-snapshots.qt.io/qdoc/qdoc-guide.html>
  - <https://doc-snapshots.qt.io/qdoc/qdoc-guide-conf.html>
- find a way to allow versioning of the API and, eventually, allow multiple version to be installed side to side. (or provide compatibility calls in new versions of the API, since we want the people to use the current API, if possible)
