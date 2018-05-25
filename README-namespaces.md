# Adding namespaces to the Scribus API

Lectures on orgainzing namespaces and APIs

- Namespace Naming: https://abseil.io/tips/130
- Have a look at the Scripter 2 API (`plugins/scripter/doc/reference.html`)

## Proposals

- Create one single flat namespace `ScribusAPI`
- Try to avoid exposing the `Q*` data structures and use the `std` ones instead (also internally in the API).

## The classes

- `ScribusAPI::`
  - `Scribus`
    - `getActiveDocument() -> Document`
    - `getOpenDocument(int) -> Document`
    - `getOpenDocumentCount() -> Document`
    - `createDocument(std::string filename) -> Document`
    - `openDocument(std::string filename) -> Document`
  - `Document`
    - try to avoid `readSections()` and `readItems()`
      - do it in the page constructor?
      - then we need multiple constructors
        - `Document(ScribusDoc* doc)`
        - `Document(ScribusDoc* doc, std::vector<int> pageRange)`
      - keep the functions (as private) but call them from the constructor?
      - also remove `setPageRange()`
    - rename `getCurrentItem()` in `getSelectedItem()`
  - `Metadata`: currently in `scribusAPIDocument.h`
    - move it to an own `.h` file.
  - `Item`
  - `ItemText`
  - `TextRun` from `ScribusAPIDocumentItemTextRuns` defined in `scribusAPIDocumentItemText.h`
  - `TextFormatting`

## Notes

- use "selected", "current" or "active" (used by scripter 2)? try to always use the same one.
  - probably, not "current".
- what about adding a `Debug()` function? (https://stackoverflow.com/questions/49381831/is-there-an-established-alternative-to-cout-in-c-that-behaves-similarly-to-qde)
